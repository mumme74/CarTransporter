/*
 * commands.c
 *
 *  Created on: 5 jul 2019
 *      Author: jof
 */

// This file contains both serialport stuff (Win, Macos and linux)
// as well as a linked list implementation for responses
// and of course also the driver for slcan
//  ie. strings to/from can_frame

#include "slcan.h"
#include "canbridge.h"
#include "buffer.h"
#include <stdio.h>



// these are loop control variables that aborts a deep loop from sighandler
static int _abortVar = 0;
static int *_abortLoop = &_abortVar;


/*
************** BEGIN serialport stuff ************************
*/
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _WIN32
// posix comport here
#include <termios.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include "unix_common.h"

#ifdef __APPLE__
// for iotcl set serial speed to 3000000 baud
# include <IOKit/serial/ioss.h>
#endif

#ifdef __linux__
// to set low latency mode, 1ms
# include <linux/serial.h>
#endif

typedef struct timeval timeval_t;

static uint16_t min_latency = 20; // FTDI chips has a default 16ms latency timer
                             // it waits for either 64bytes or 16ms
                             // it is changable through DLL, but not through serial (unless its a linux)
                             // linux does however let us set 1ms latency in ioctl

static int canfd = 0;



static int open_port(const char *portname)
{
    struct termios tty;

    canfd = open(portname, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (canfd < 0) {
        CANBRIDGE_SET_ERRMSG("Error opening %s: %s\n", portname, strerror(errno))
        return -errno;
    }
    if (!isatty(canfd)) {
        CANBRIDGE_SET_ERRMSG("%s is not a tty device\n", portname)
        return 0;
    }

    // get attributes for this tty
    if (tcgetattr(canfd, &tty) < 0) {
        CANBRIDGE_SET_ERRMSG("Error from tcgetattr: %s\n", strerror(errno))
        return -1;
    }

#ifndef __APPLE__
    // set speed to 1Mbit/s
    cfsetospeed(&tty, B3000000);
    cfsetispeed(&tty, B3000000);
#endif

    tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
    tty.c_cflag &= ~(uint32_t)CSIZE;
    tty.c_cflag |= CS8;         /* 8-bit characters */
    tty.c_cflag &= ~(uint32_t)PARENB;     /* no parity bit */
    tty.c_cflag &= ~(uint32_t)CSTOPB;     /* only need 1 stop bit */
    tty.c_cflag &= ~(uint32_t)CRTSCTS;    /* no hardware flowcontrol */

    /* setup for non-canonical mode */
    tty.c_iflag &= ~(uint32_t)(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(uint32_t)(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~(uint32_t)OPOST;

    /* use polling */
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 0;

    if (tcsetattr(canfd, TCSANOW, &tty) != 0) {
        CANBRIDGE_SET_ERRMSG("Error from tcsetattr: %s\n", strerror(errno))
        return -1;
    }

#ifdef __APPLE__
    speed_t speed = 3000000;
    if (ioctl(canfd, IOSSIOSPEED, &speed)) {
        CANBRIDGE_SET_ERRMSG("Error when setting serial speed to 1Mbaud\n");
        return 0;
    }

#else // linux
    // set latency to max 1ms
    struct serial_struct kSerialSettings;

    if (ioctl(canfd, TIOCGSERIAL, &kSerialSettings) > -1) {
        kSerialSettings.flags |= ASYNC_LOW_LATENCY;
        if (ioctl(canfd, TIOCSSERIAL, &kSerialSettings) < 0) {
            CANBRIDGE_SET_ERRMSG(
                    "Could not set low latency to serial driver\nreson%s\n",
                    strerror(errno))
        } else
            min_latency = 5;
    }
#endif

    return 1;
}

static int write_port(const char *buf, uint32_t len,
                      uint32_t *bytesWritten, int timeoutms)
{
    if (timeoutms < min_latency)
        timeoutms = min_latency;
    *bytesWritten = 0;

    timeval_t timeoutAt, initial, now, add = { 0 , 1000 * timeoutms };
    gettimeofday(&initial, NULL);
    uint32_t retries = 0;

    // calculate when to bail out
    timeradd(&initial, &add, &timeoutAt);
    do {
        ssize_t res = write(canfd, &buf[*bytesWritten], len - *bytesWritten);
        if (res > 0) {
            *bytesWritten += (uint32_t)res;
            retries = 0;
        } else {
            if (++retries > 10 && *bytesWritten > 0)
                break;
        }
        gettimeofday(&now, NULL);
        usleep(1000);
    } while(timercmp(&now, &timeoutAt, < ) &&
            *bytesWritten < len && !*_abortLoop);

    return *bytesWritten == len;
}

static int read_port(char *buf, uint32_t maxLen, uint32_t minLen,
                     uint32_t *bytesRead, uint16_t timeoutms)
{
    if (timeoutms < min_latency)
        timeoutms = min_latency;
    *bytesRead = 0;
    uint32_t timeoutAt = timeGetTime() + timeoutms;

    fd_set set;
    struct timeval timeout;
    int rv;

    FD_ZERO(&set); /* clear the set */
    FD_SET(canfd, &set); /* add our file descriptor to the set */

    timeout.tv_sec = timeoutms / 1000;
    timeout.tv_usec = timeoutms * 1000;

    do {
        rv = select(canfd + 1, &set, NULL, NULL, &timeout);
        if(rv == -1) {
          CANBRIDGE_SET_ERRMSG("select failed while read serial: %s\n", strerror(errno))
          return -errno;
        } else if(rv > 0) {
             ssize_t res = read(canfd, &buf[*bytesRead], maxLen - *bytesRead);
             if (res > 0) {
                 *bytesRead += (uint32_t)res;
             } else if (res == -1 && errno != EAGAIN)
                 return -errno; // read error
         }
     } while((timeoutAt > timeGetTime()) &&
              *bytesRead < minLen && !*_abortLoop);

//    do {
//        ssize_t res = read(canfd, &buf[*bytesRead], maxLen - *bytesRead);
//        if (res > 0) {
//            *bytesRead += (uint32_t)res;
//            retries = 0;
//        } else if (res == -1 && errno != EAGAIN) {
//            return -errno; // read error
//        }else if (*bytesRead > minLen && ++retries > 10) {
//            // can we safely break now?
//            break;
//        }
//        usleep(5000);

//    } while((timeoutAt > timeGetTime()) &&
//            *bytesRead < maxLen && !*_abortLoop);

    return *bytesRead == maxLen;
}

static void flush_port(int rx, int tx)
{
    int flags = 0;
    if (rx && tx) flags |= TCIOFLUSH;
    else if (rx) flags |= TCIFLUSH;
    else if (tx) flags |= TCOFLUSH;
    else
        return;
     tcflush(canfd, flags);
}

static int close_port(void)
{
    if (!canfd) {
        CANBRIDGE_SET_ERRMSG("tty already closed!\n")
        return 0;
    }

    if (close(canfd) < 0) {
        CANBRIDGE_SET_ERRMSG("Error closing tty: %s\n", strerror(errno))
        return 0;
    }

    return 1;
}


#else // _WIN32

// based on https://www.xanthium.in/Serial-Port-Programming-using-Win32-API
#include<Windows.h>

static HANDLE canfd;

// return last error as string
char *ErrorAsString(const DWORD errCode)
{
    enum { BUF_SZ = 256 };
    static char buf[BUF_SZ];
    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                   NULL,
                   errCode,
                   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                   buf, // out string here
                   BUF_SZ, // Size of buf
                   NULL); // optional arguments

    return buf;
}

static int open_port(const char *port)
{
    enum { PORTNAME_SZ = 12 };
    char portname[PORTNAME_SZ]; // allows up to \\.\COM999
    int len = (int)strnlen(port, 20);
    if (len < 5)
        /* ports COM1-9 doesnt need \\.\ */
        snprintf(portname, PORTNAME_SZ, "%s", port);
    else
        snprintf(portname, PORTNAME_SZ, "\\\\.\\%s", port);

    canfd = CreateFileA(portname,                //port name
                        GENERIC_READ | GENERIC_WRITE, //Read/Write
                        0,                            // No Sharing
                        NULL,                         // No Security
                        OPEN_EXISTING,// Open existing port only
                        0,            // Non Overlapped I/O
                        NULL);        // Null for Comm Devices

    if (canfd == INVALID_HANDLE_VALUE) {
        CANBRIDGE_SET_ERRMSG("Error in opening serial port\n");
        return 0;
    }

    // ******* port settings ***********
    // setup speed, controlbits etc.
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    // get current settings
    if (!GetCommState(canfd, &dcbSerialParams)) {
        CANBRIDGE_SET_ERRMSG("Couldn't get com parameters\n");
        return 0;
    }
    // change settings
    dcbSerialParams.BaudRate = 3000000;   // Setting BaudRate
    dcbSerialParams.ByteSize = 8;         // Setting ByteSize = 8
    dcbSerialParams.StopBits = ONESTOPBIT;// Setting StopBits = 1
    dcbSerialParams.Parity   = NOPARITY;  // Setting Parity = None
    // set settings to port
    if (!SetCommState(canfd, &dcbSerialParams)) {
        CANBRIDGE_SET_ERRMSG("Failed to change port settings\n");
        return 0;
    }

    // ******* timeouts **************
    COMMTIMEOUTS timeouts = { 0 };
    // return immediatly from read operations
    timeouts.ReadIntervalTimeout         = 1; // in milliseconds
    timeouts.ReadTotalTimeoutConstant    = 1; // in milliseconds
    timeouts.ReadTotalTimeoutMultiplier  = 1; // in milliseconds
    timeouts.WriteTotalTimeoutConstant   = 2; // in milliseconds
    timeouts.WriteTotalTimeoutMultiplier = 1; // in milliseconds

    if (!SetCommTimeouts(canfd, &timeouts)) {
        CANBRIDGE_SET_ERRMSG("Couldn't set COM timeouts.\n");
        return 0;
    }

    /*if (!SetCommMask(canfd, EV_RXCHAR | EV_TXEMPTY)) {
        CANBRIDGE_SET_ERRMSG("Couldn't set event flag rx (EV_RXCHAR | EV_TXEMPTY)\n");
        return 0;
    }*/

    return 1;
}

// len is how many bytes to write
static int write_port(const char *buf, uint32_t len,
                      uint32_t *written, const int timeoutms)
{
    DWORD nBytesWritten = 0,
          timeoutAt = timeGetTime() + (uint32_t)timeoutms;
    *written = 0;

    // continue until timeout is met
    do {
        // write to COM port
        if (WriteFile(canfd, buf + *written, len - nBytesWritten, &nBytesWritten, NULL))
            *written += nBytesWritten;
        else {
            DWORD errCode = GetLastError();
            CANBRIDGE_SET_ERRMSG("Error writing to to serial, error: %lu %s\n", errCode, ErrorAsString(errCode));
            return -1;
        }
    } while((timeoutAt > timeGetTime()) &&
            (*written < len) && !(*_abortLoop));

    return *written == len;
}

static int read_port(char *buf, uint32_t len,
                     uint32_t *readBytes, int timeoutms)
{
    DWORD nBytesRead = 0,
          timeoutAt = timeGetTime() + (uint32_t)timeoutms;
    *readBytes = 0;

    // continue until timeout is met
    do {
        // read from COM port
        while(ReadFile(canfd, (LPVOID)buf, 1, &nBytesRead, NULL) &&
              nBytesRead > 0)
        {
            *readBytes += nBytesRead;
        }
    } while(timeoutAt > timeGetTime() &&
            *readBytes < len && !(*_abortLoop));

    return *readBytes == len;
}

static int close_port(void)
{
    if (!CloseHandle(canfd)) {
        CANBRIDGE_SET_ERRMSG("Error closing port\n");
        return 0;
    }
    return 1;
}

static void port_flush(int rx, int tx)
{
    DWORD flags = 0;
    if (!rx && !tx) return;
    if (rx) flags |= PURGE_RXCLEAR;
    if (tx) flags |= PURGE_TXCLEAR;
    PurgeComm(canfd, flags);
}

#endif

/*
 ********************* END serialport stuff ***********************
*/

static int state = CANBRIDGE_CLOSED;
static const char CR = '\r', BELL = '\a';
static int hwVersion = 0, swVersion = 0;



// ----------------------- Responses list -------------------------
//  maximum nr of chars in a response
#define RESPONSE_MAX_SZ 32 // with timestamps (cmd Z1) it can be pretty long

typedef struct _response {
    struct _response *prev, *next;
    char *data ;
    uint8_t len;

    // pad struct (squelsh warnings)
#ifdef __SIZEOF_POINTER__
#if __SIZEOF_POINTER__  == 8u
    uint8_t __pad[7];
#elif __SIZEOF_POINTER__ == 4u
    uint8_t __pad[3];
#endif
#elif defined(_WIN32) && defined(_W64)
    uint8_t __pad[7];
#elif defined(_WIN32)
    uint8_t __pad[3];
#endif

} Response_t;

typedef struct {
    Response_t *first, *last;
    uint32_t len;
} ResponseList_t;

static ResponseList_t responses;

void responselist_init(ResponseList_t *lst)
{
    lst->first = lst->last = NULL;
    lst->len = 0;
}

/**
 * @brief response_create, create a new response obj
 *         must be deallocated using response_free(obj)
 * @param data, the data to store
 * @param len, how many bytes of data
 * @return a newly malloc-ed response obj, must freed using response_free
 */
Response_t *response_create(const char *data, uint8_t len)
{
    if (len < 1)
        return NULL;

    Response_t *r = (Response_t*)malloc(sizeof (Response_t));
    r->len = len;
    r->prev = r->next = NULL;
    r->data = (char*)malloc(sizeof (char) * (len +1));
    memcpy(r->data, data, len +1);
    r->data[len] = 0; // terminate str
    return r;
}

/**
 * @brief response_insert, insert itm into lst
 * @param lst, the list to store itm in
 * @param prev, node directly previous to itm
 * @param itm, a response obj created by response_create
 * @return 1 on success else 0
 */
int response_insert(ResponseList_t *lst, Response_t *prev, Response_t *itm)
{
    if (!prev && !lst->first && itm) {
        // very first insert
        lst->first = lst->last = itm;
        ++lst->len;
    } else if (!prev && !itm) {
        return 0; // invalid

    } else if (prev->next == NULL) {
        // insert as last element
        prev->next = itm;
        if (itm) {
            itm->next = NULL;
            itm->prev = prev;
            lst->last = itm;
            ++lst->len;
        }
    } else if (!prev) {
        // insert as first element
        lst->first->prev = itm;
        lst->first = itm;
        ++lst->len;
    } else if(itm != NULL) {
        // insert in the middle
        itm->next = prev->next;
        prev->next = itm;
        itm->prev = prev;
        ++lst->len;
    } else {
        return 0;
    }

    return 1;
}

/**
 * @brief response_get_at, get the list item at index pos
 * @param lst, the list to store it in
 * @param idx, the place in list, same as lst[idx]
 * @return the item or NULL
 */
Response_t *response_get_at(ResponseList_t *lst, uint32_t idx)
{
    if (idx >= lst->len)
        return NULL;

    Response_t *itm = NULL;
    uint32_t stepsToMove = idx;
    if (stepsToMove > lst->len / 2) {
        // look from the end
        stepsToMove = lst->len - stepsToMove;
        itm = lst->last;

        while (stepsToMove--)
            itm = itm->prev;

    } else {
        // look from start
        itm = lst->first;

        while (stepsToMove--)
            itm = itm->next;
    }

    return itm;
}

/**
 * @brief response_take_at, same as response_get_at but also removes from list
 * @param lst, the list to operate on
 * @param idx, the indes pos
 * @return the newly taken item on success or NULL
 */
Response_t *response_take_at(ResponseList_t *lst, uint32_t idx)
{
    Response_t *itm = response_get_at(lst, idx);
    if (!itm)
        return NULL;

    // update lst pointers
    if (itm == lst->first) {
        lst->first = lst->first->next;
    }
    if (itm == lst->last) {
        lst->last = itm->prev;
    }

    // re-wire other nodes in list
    if (itm->prev)
        itm->prev->next = itm->next;
    if (itm->next)
        itm->next->prev = itm->prev;

    itm->prev = itm->next = NULL;

    --lst->len;

    return itm;
}

void response_remove_item(ResponseList_t *lst, Response_t *itm)
{
    if (!itm)
        return;

    // update lst pointers
    if (itm == lst->first)
        lst->first = lst->first->next;
    if (itm == lst->last)
        lst->last = itm->prev;
    --lst->len;

    if (itm->next)
        itm->next->prev = itm->prev;
    if (itm->prev)
        itm->prev->next = itm->next;

    itm->next = itm->prev = NULL;
}

/**
 * @brief response_free, deallocate data
 * @param r, response struct to deallocate
 */
void response_free(Response_t *r)
{
    if (r == NULL)
        return;

    free(r->data);
    free(r);
}

void response_clear_all(ResponseList_t *lst)
{
    if (!lst->first)
        return;

    Response_t *itm, *next;
    next = lst->first;
    do {
        itm = next;
        next = itm->next;
        response_free(itm);
    } while(next != NULL);

    lst->len = 0;
    lst->last = lst->first = NULL;
}

/**
 * @brief response_read_serial, reads from serialport, inserts responses into list
 * @param lst, responses list to insert into
 * @param timeoutms, timeout serialread in ms
 * @return 1 on success or 0
 */
int response_read_serial(ResponseList_t *lst, uint32_t minLen, const uint16_t timeoutms)
{
    uint32_t timeoutAt = timeGetTime() + (uint32_t)timeoutms;

    enum { BUF_SZ = 0xFFFFFF };
    static char buf[BUF_SZ];
    static uint32_t bufPos = 0; // position of previous unfinished read

    uint32_t nBytes = 0,
            start = bufPos,
            tmpBufPos = bufPos,
            initialLen = lst->len;
    Response_t *itm = NULL;

    int res = read_port(&buf[bufPos], BUF_SZ - bufPos, minLen, &nBytes, timeoutms);
    if (res < 0)
        CANBRIDGE_SET_ERRMSG("Failed to read from serial: %s\n", strerror(-res))

    if (nBytes > 0) {
        // parse the responses and insert into responses list
        for(uint32_t i = bufPos; i < tmpBufPos + nBytes; ++i) {
            char ch = buf[i];

            if (ch == CR || ch == BELL) {
                // we have a complete response
                uint8_t len = (uint8_t)(i - (start - bufPos)) + 1;
                itm = response_create(&buf[start - bufPos], len);
                bufPos = 0; // we have now gotten the trailing chars from last read
                if (!itm) {
                    CANBRIDGE_SET_ERRMSG("Failed to allocate memory\n")
                    goto cleanup;
                }
                if (!response_insert(lst, lst->last, itm)) {
                    CANBRIDGE_SET_ERRMSG("Failed to insert item into list\n")
                    goto cleanup;
                } else
                    itm = NULL; // derefernce to avoid freeing a inserted itm

                start = i+1;
            }
        }

        // we might have trailing bytes we need to store til next read_serial
        // move to beginning of buffer
        if (start < tmpBufPos + nBytes) {
            // sometimes we get erronious null chars as back?
            while (start < tmpBufPos + nBytes && buf[start] == 0 &&
                   start < BUF_SZ)
            {
                ++start;
            }

            if (start < tmpBufPos + nBytes) {
                uint32_t len = (tmpBufPos + nBytes) - start;
                memmove(buf, &buf[start], len);
                bufPos = len;
            }
        } else {
            bufPos = 0;
        }
    }

    // we might have more to recieve
    if (timeGetTime() < timeoutAt && nBytes > 0) {
        // if we have found any chars in this invokation wewant to return true
        // sub recurse should eventually end up in false.
        int res = nBytes > 0,
            res2 = response_read_serial(lst, minLen, (uint16_t)(timeoutAt - timeGetTime()));
        return res ? res : res2;
    }

cleanup:
    //response_free(itm); // we should NOT free itm here asits deletes it from the list

    return initialLen != lst->len;
}

/**
 * @brief response_find, looks up a certain response based on filter
 * @param lst, list to search in
 * @param cmdsFilter, filter commands against these, might be up to 20 cmds
 * @param reversed, search from the back if non zero
 * @param restrct, if > 0 onlky go back this many responses
 * @param timeoutms, if > 0 and we hant found any we read serial port
 * @return
 */
Response_t *response_find(ResponseList_t *lst, const char *cmdsFilter,
                          uint8_t reversed, int restrct, uint16_t timeoutms)
{
    uint32_t timeoutAt = timeGetTime() + (uint32_t)timeoutms,
             minLen = 2;
    switch(cmdsFilter[0]){
    case 't': case 'r': minLen = 27; break;
    case 'T': case 'R': minLen = 32; break;
    case 'F': minLen = 4; break;
    default: if (cmdsFilter[0] <= '\r') minLen = 1;
    }

    // if we are empty, read from serial, if none read then bail
    if (!lst->len && timeoutms > 0)
        if (!response_read_serial(lst, minLen, timeoutms))
            return NULL;

    Response_t *itm;
    int filterLen = (int)strnlen(cmdsFilter, 20);

    if (reversed) {
        itm = lst->last;
        do {
            for(int i = 0; i < filterLen; ++i) {
                if (itm->len && itm->data[0] == cmdsFilter[i]) {
                    response_remove_item(lst, itm);
                    return itm;
                }
            }
            if (restrct && --restrct < 1)
                break;
        } while((itm = itm->prev));
    } else {
        // non reversed
        itm = lst->first;
        do {
            for(int i = 0; i < filterLen; ++i) {
                if (itm->len && itm->data[0] == cmdsFilter[i]) {
                    response_remove_item(lst, itm);
                    return itm;
                }
            }
            if (restrct && --restrct < 1)
                break;
        } while((itm = itm->next));
    }

    // we have not found it, read from serial again until we timeout
    if (timeGetTime() < timeoutAt) {
        timeoutms = (uint16_t)(timeoutAt - timeGetTime());
        if (timeoutms > 0) {
            response_read_serial(lst, minLen, timeoutms);
            return response_find(lst, cmdsFilter, reversed, restrct, timeoutms);
        }
    }

    return NULL;
}

// ----------------------- END responses list ------------------------

#define FUNC_HEADER int res = 1; Response_t *resp = NULL
#define GOTO_CLEANUP_ERR { res = 0; goto cleanup; }
#define GOTO_CLEANUP_OK { res = 1; goto cleanup; }

static int clear_pipeline(void) {
    // canusb.com states:
    // Always start each session (when your program starts) with sending 2-3 [CR]
    response_clear_all(&responses);
    // discard old chars from serial buffer
    flush_port(1, 1);

    for(int i = 0; i < 3; ++i) {
        char ch = CR;
        uint32_t nBytes;
        int res = write_port(&ch, 1, &nBytes, 10);
        if (res < 0) {
            // errmsg already set
            return 0;
        } else if (res == 0) {
            CANBRIDGE_SET_ERRMSG("Failed to clear transmit buffer\n")
            return 0;
        }

        // clear response buffer, 1char at a time until CR is recieved
        // we might have old responses in pipeline
        Response_t *resp = response_find(&responses, "\r\a", 1, 1, 5);
        if (!resp) {
            CANBRIDGE_SET_ERRMSG("Failed to get response while clearing serial buffer\n")
            return 0;
        }
        response_free(resp);
    }

    response_clear_all(&responses);

    return 1;
}


// ----------------------------------------------------------------
// begin exported functions
/**
 * @brief slcan_init, initializes, bur not open, slcan
 * @param name, Interface to use, ie /dev/ttyACM0 or COM10
 * @param speed, the bitrate to use
 * @return 0 on error, 1 when ok
 */
int slcan_init(const char *name, CAN_Speeds_t speed)
{
    responselist_init(&responses);
    FUNC_HEADER;

    // this driver needs a open
    if (open_port(name) < 1)
        GOTO_CLEANUP_ERR

    state = CANBRIDGE_INIT;

    // clear old messages
    if (!clear_pipeline())
        GOTO_CLEANUP_ERR

    // canusb states we must get version before anything else
    char buf[3] = { 'V', CR , 0 };
    uint32_t nBytes;
    int sRes = write_port(buf, 2, &nBytes, 5);
    if (sRes < 0) {
        // err msg already set
        GOTO_CLEANUP_ERR
    } else if (sRes == 0) {
        CANBRIDGE_SET_ERRMSG("Failed to send version request\n")
        GOTO_CLEANUP_ERR
    }

    resp = response_find(&responses, "V", 0, 1, 5);
    if (!resp) {
        CANBRIDGE_SET_ERRMSG("Failed to read version response\n")
        goto no_version;
    }
    if (resp->data[0] == BELL) {
        CANBRIDGE_SET_ERRMSG("Version request responded with error\n")
        goto no_version;
    }
    const int versionSz = 6;
    if (resp->len < versionSz) {
        CANBRIDGE_SET_ERRMSG(
              "Wrong response from slcan interface version request '%s' expected 'Vxxxx\\r'\n",
              resp->data)
        goto no_version;
    }
    // read the version
    char hwStr[3] = { resp->data[1], resp->data[2], 0 },
         swStr[3] = { resp->data[3], resp->data[4], 0 };
    hwVersion = atoi(hwStr);
    swVersion = atoi(swStr);

// other slcan slcan interfaces such as CANTin does not handle 'V' cmd
// we should be able to continue anyway
no_version:
    response_free(resp);

    // set the speed
    buf[0] = 'S'; buf[2] = CR;
    switch (speed) {
    case CAN_speed_10Kbit:  buf[1] = '0'; break;
    case CAN_speed_20Kbit:  buf[1] = '1'; break;
    case CAN_speed_50Kbit:  buf[1] = '2'; break;
    case CAN_speed_100Kbit: buf[1] = '3'; break;
    case CAN_speed_125Kbit: buf[1] = '4'; break;
    case CAN_speed_250Kbit: buf[1] = '5'; break;
    case CAN_speed_500Kbit: buf[1] = '6'; break;
    case CAN_speed_800Kbit: buf[1] = '7'; break;
    case CAN_speed_1Mbit:   buf[1] = '8'; break;
    case CAN_speed_socketspeed: // fallthrough
    default:
        CANBRIDGE_SET_ERRMSG("Unsupported CAN speed\n")
        GOTO_CLEANUP_ERR
    }

    sRes = write_port(buf, 3, &nBytes, 5);
    if (sRes < 0) {
        // err msg already set
        GOTO_CLEANUP_ERR
    } else if (sRes == 0) {
        CANBRIDGE_SET_ERRMSG("Failed to write speed cmd to serial\n")
        GOTO_CLEANUP_ERR
    }

    resp = response_find(&responses, "\r\a", 1, 1, 5);
    if (!resp) {
        CANBRIDGE_SET_ERRMSG("Failed to read from speed cmd response from serial\n")
        GOTO_CLEANUP_ERR
    }
    if (resp->data[0] != CR) {
        // maybe its just open
        static uint8_t retryInit = 0xFF;
        if (++retryInit == 0) {
            if (slcan_close()) {
                if (slcan_init(name, speed))
                    GOTO_CLEANUP_OK
            }
        }

        // else we failed with retry
        CANBRIDGE_SET_ERRMSG("Set speed command failed in slcan slcan interface\n")
        GOTO_CLEANUP_ERR
    }

cleanup:
    response_free(resp);

    return res;
}

/**
 * @brief slcan_set_filter, sets a CAN filter
 * @param mask, use mask to filter against
 * @param id, use this can msg ID
 * @return 0 on error, 1 when ok
 */
int slcan_set_filter(uint32_t mask, uint32_t id)
{
    FUNC_HEADER;

    if (state != CANBRIDGE_INIT) {
        CANBRIDGE_SET_ERRMSG("Can only set filter in init mode\n")
        GOTO_CLEANUP_ERR
    }

    char cmds[] = {
        'M', // Acceptance code
        'm'  // Acceptance mask
    };

    // should send LSB first
    byte4_t code[] = { {id}, {mask} };
    char buf[RESPONSE_MAX_SZ];
    uint32_t nBytes = 0, j;

    for(int i = 0; i < 2; ++i) {
        j = 0;
        buf[j++] = cmds[i]; // insert command
        snprintf(&buf[j], 3,"%02X", code[i].b0); // lsb
        j += 2; // adds 2 chars above ie: FF
        snprintf(&buf[j], 3, "%02X", code[i].b1);
        j += 2;
        snprintf(&buf[j], 3, "%02X", code[i].b2);
        j += 2;
        snprintf(&buf[j], 3, "%02X", code[i].b3); // msb
        j += 2;
        buf[j++] = CR; // end marker

        int sRes = write_port(buf, j, &nBytes, 10);
        if (sRes < 0) {
            // err msg already set
            GOTO_CLEANUP_ERR
        } else if (sRes == 0) {
            CANBRIDGE_SET_ERRMSG(
                    "Failed to send CAN filter to serial\nwritten:%d of %d",
                    nBytes, j)
            GOTO_CLEANUP_ERR
        }

        // get response
        resp = response_find(&responses, "\r\a", 1, 1, 5);
        if (!resp) {
            CANBRIDGE_SET_ERRMSG("Failed to set CAN filter, response timeout\n")
            GOTO_CLEANUP_ERR
        }
        if (resp->data[0] != CR) {
            CANBRIDGE_SET_ERRMSG("Failed to set CAN filter\n")
            GOTO_CLEANUP_ERR
        }
    }

cleanup:
    response_free(resp);

    return res;
}

/**
 * @brief slcan_open, open a previously initialized
 * @return 0 on error, 1 when ok, -1 when already was opened
 */
int slcan_open(void)
{
    FUNC_HEADER;

    response_clear_all(&responses);
    flush_port(1, 1);

    if (state == CANBRIDGE_CLOSED) {
        CANBRIDGE_SET_ERRMSG("Can't open a closed comport\n")
        GOTO_CLEANUP_ERR
    } else if (state == CANBRIDGE_OPEN) {
        CANBRIDGE_SET_ERRMSG("Already open comport\n")
        res = -1;
        goto cleanup;
    }

    // open CAN channel
    uint32_t nBytes;
    char buf[RESPONSE_MAX_SZ] = { 'O', CR };
    int sRes = write_port(buf, 2, &nBytes, 10);
    if (sRes < 0) {
        // err msg already set
        GOTO_CLEANUP_ERR
    } else if (sRes == 0) {
        CANBRIDGE_SET_ERRMSG("Failed to send open cmd to serial")
        GOTO_CLEANUP_ERR
    }

    // get response
    resp = response_find(&responses, "\r\a", 1, 0, 10);
    if (!resp) {
        CANBRIDGE_SET_ERRMSG("Failed to open CAN channel, response timeout\n")
        GOTO_CLEANUP_ERR
    }
    if (resp->data[0] != CR) {
        CANBRIDGE_SET_ERRMSG("Failed to open CAN channel\n")
        GOTO_CLEANUP_ERR
    }

    // success!
    state = CANBRIDGE_OPEN;

    // clear any possible errors
    slcan_geterrors();

cleanup:
    response_free(resp);

    return res;
}

uint8_t slcan_geterrors(void)
{
    Response_t *resp = NULL;
    uint8_t retVlu = 0;

    if (state == CANBRIDGE_OPEN) {
        char txChs[] = { 'F', CR };
        uint32_t nBytes;
        int sRes = write_port(txChs, 2, &nBytes, 10);
        if (sRes < 0) {
            // err msg already set
            goto cleanup;
        } else if (sRes == 0) {
            CANBRIDGE_SET_ERRMSG("Could not send to serial when reading error message\n")
            goto cleanup;
        }

        resp = response_find(&responses, "F\a", 1, 1, 10);
        if (!resp) {
            CANBRIDGE_SET_ERRMSG("No response from slcan interface when reading error message\n")
            goto cleanup;
        }
        if (resp->len > 1 && resp->data[0] == 'F') {
            // TODO should we set errormsg to user here?
            retVlu = (uint8_t)resp->data[1];
            goto cleanup;
        }
    }

cleanup:
    response_free(resp);

    return retVlu;
}


/**
 * @brief slcan_status
 * @return CANBRIDGE_CLOSED, CANBRIDGE_INIT (but not open), CANBRIDGE_OPEN
 */
int slcan_status(void)
{
    return state;
}

/**
 * @brief slcan_set_fd_mode, tries to set FD mode,
 *          not all CAN controllers support this faster mode
 * @return 0 on error, 1 when ok
 */
int slcan_set_fd_mode(void)
{
    return 0; // not implemented!
}

/**
 * @brief slcan_close
 * @return 0 on error, 1 when ok
 */
int slcan_close(void)
{
    FUNC_HEADER;

    if (state == CANBRIDGE_CLOSED) {
        CANBRIDGE_SET_ERRMSG("Already closed!\n")
        GOTO_CLEANUP_ERR
    }

    // close possible CAN channel, we must be able to do this even in INIT state
    // to release a stuck slcan slcan interface
    if (state == CANBRIDGE_OPEN || state == CANBRIDGE_INIT) {
        // close CAN channel
        // start with purging response buffer
        response_clear_all(&responses);

        uint32_t nBytes;
        char buf[RESPONSE_MAX_SZ] = { 'C', CR };
        int sRes = write_port(buf, 2, &nBytes, 5);
        if (sRes < 0) {
            // err msg already set
            GOTO_CLEANUP_ERR
        } else if (sRes == 0) {
            CANBRIDGE_SET_ERRMSG("Failed to send close cmd to serial")
            GOTO_CLEANUP_ERR
        }

        // get response
        resp = response_find(&responses, "\r\a", 1, 1, 5);
        if (!resp) {
            CANBRIDGE_SET_ERRMSG("Failed to close CAN channel, no response from slcan slcan interface\n")
            GOTO_CLEANUP_ERR
        }
        if (resp->data[0] != CR) {
            CANBRIDGE_SET_ERRMSG("Failed to close CAN channel, slcan interface responded with error\n")
            GOTO_CLEANUP_ERR
        }
    }

cleanup:
    response_free(resp);

    // close serial
    if (close_port())
        state = CANBRIDGE_CLOSED;
    else
        res = 0;

    return res;
}

/**
 * @brief slcan_set_abortvariable, abort deep loop if variable is set
 *          slcan re-tries to send message, program can get stuck
 * @param abortVar, a variable that can get set from a sighandler funtion
 *                  such as SIGINT
 */
void slcan_set_abortvariable(int *abortVar)
{
    _abortLoop = abortVar;
}

/**
 * @brief slcan_send, send on can
 * @param frm, frame to send
 * @param timeoutms, timeout if not send within
 * @return true on success
 */
int slcan_send(canframe_t *frm, int timeoutms)
{
    FUNC_HEADER;

    if (state != CANBRIDGE_OPEN) {
        CANBRIDGE_SET_ERRMSG("Can't send on a non open driver\n")
        GOTO_CLEANUP_ERR
    }

    uint32_t pos = 0, nBytes;
    enum { SEND_BUF_MAXSZ = 30 }; // should be enough, extended frame 8bytes should be 27 with CR
    char buf[RESPONSE_MAX_SZ];
    if (frm->can_id & CAN_RTR_FLAG) {
        // is a rtr frame
        buf[pos++] = (frm->can_id & CAN_EFF_FLAG) ? 'R' : 'r'; // 29bits vs 11bits
    } else {
        // is a payload frame
        buf[pos++] = (frm->can_id & CAN_EFF_FLAG) ? 'T' : 't'; // 29bits vs 11bits
    }

    // first ID
    byte4_t id = { frm->can_id & CAN_EFF_MASK };
    if (frm->can_id & CAN_EFF_FLAG) {
        snprintf(&buf[pos], 2, "%01X", id.b3);
        pos += 1;
        snprintf(&buf[pos], 3, "%02X", id.b2);
        pos += 2;
        snprintf(&buf[pos], 3, "%02X", id.b1);
        pos += 2;
    } else {
        // 11bits should only print 3chars
        snprintf(&buf[pos], 2, "%01X", id.b1);
        pos += 1;
    }
    snprintf(&buf[pos], 3, "%02X", id.b0);
    pos += 2;

    // length of frame
    snprintf(&buf[pos], 2, "%01X", frm->can_dlc);
    pos += 1;

    // no payload on rtr frame
    if ((frm->can_id & CAN_RTR_FLAG) == 0) {
        // then payload
        byte8_t payload = { 0 };
        for(int i = 0; i < frm->can_dlc; ++i)
            payload.arr[i] = frm->data[i];

        if (frm->can_dlc > 0) {
            snprintf(&buf[pos], 3, "%02X", payload.b0);
            pos += 2;
        }
        if (frm->can_dlc > 1) {
            snprintf(&buf[pos], 3, "%02X", payload.b1);
            pos += 2;
        }
        if (frm->can_dlc > 2) {
            snprintf(&buf[pos], 3, "%02X", payload.b2);
            pos += 2;
        }
        if (frm->can_dlc > 3) {
            snprintf(&buf[pos], 3, "%02X", payload.b3);
            pos += 2;
        }
        if (frm->can_dlc > 4) {
            snprintf(&buf[pos], 3, "%02X", payload.b4);
            pos += 2;
        }
        if (frm->can_dlc > 5) {
            snprintf(&buf[pos], 3, "%02X", payload.b5);
            pos += 2;
        }
        if (frm->can_dlc > 6) {
            snprintf(&buf[pos], 3, "%02X", payload.b6);
            pos += 2;
        }
        if (frm->can_dlc > 7) {
            snprintf(&buf[pos], 3, "%02X", payload.b7);
            pos += 2;
        }
    }

    // end string
    buf[pos++] = CR;

    int sRes = write_port(buf, pos, &nBytes, timeoutms);
    if (sRes < 0) {
        // err msg already set
        GOTO_CLEANUP_ERR
    } else if (sRes == 0) {
        CANBRIDGE_SET_ERRMSG("Failed to send to serial\nwritten:%d of %d", nBytes, pos)
        GOTO_CLEANUP_ERR // send_cmd sets canbridge_errmsg
    }

    // get response
    char okCmds[3] = { (frm->can_id & CAN_EFF_FLAG) ? 'Z' : 'z', BELL, 0 };
    resp = response_find(&responses, okCmds, 1, 0, timeoutms);
    if (!resp) {
        CANBRIDGE_SET_ERRMSG("Failed to send to CAN channel, slcan interface response timeout\n")
        GOTO_CLEANUP_ERR // continue anyway?
    }
    if (resp->data[1] != CR) {
        CANBRIDGE_SET_ERRMSG("Failed to send to CAN channel, slcan interface returned error\n")
        GOTO_CLEANUP_ERR
    }

cleanup:
    response_free(resp);

    return res;
}

/**
 * @brief slcan_recv, recive fromcan
 * @param frm, frame to fill when recv occurs
 * @param timeoutms, timeout if no recieved frm within timeout
 * @return true if we have a frm
 */
int slcan_recv(canframe_t *frm, int timeoutms)
{
    FUNC_HEADER;

    if (state != CANBRIDGE_OPEN) {
        CANBRIDGE_SET_ERRMSG("Can't send on a non open driver\n")
        return 0;
    }

    uint32_t pos = 0;
    resp = response_find(&responses, "tTrR", 0, 0, timeoutms);
    if (!resp)
        GOTO_CLEANUP_ERR
    //printf("rcv:%s\n", resp->data);

    if (resp->len < 5)
        GOTO_CLEANUP_ERR

    memset(frm, 0, sizeof (*frm));

    // parse id
    if (resp->data[0] == 't' || resp->data[0] == 'r') {
        // 11 bit ID
        char id11[] = {
            resp->data[1], resp->data[2], resp->data[3], 0
        };
        char lenStr[] = { resp->data[4], 0 };
        frm->can_id = (uint32_t)strtol(id11, NULL, 16);
        frm->can_dlc = (uint8_t)strtol(lenStr, NULL, 16);
        pos = 5;
    } else {
        // 29 bit id
        char id29[] = {
            resp->data[1], resp->data[2], resp->data[3], resp->data[4],
            resp->data[5], resp->data[6], resp->data[7], resp->data[8], 0
        };
        frm->can_id = (uint32_t)strtol(id29, NULL, 16) | CAN_EFF_FLAG;
        frm->can_dlc = (uint8_t)atoi(&resp->data[9]);
        pos = 10;
    }

    // remote frame
    if (resp->data[0] == 'r' || resp->data[0] == 'R')
        frm->can_id |= CAN_RTR_FLAG;

    char byteStr[3] = { 0 };
    if (frm->can_dlc > 0) {
        byteStr[0] = resp->data[pos++]; byteStr[1] = resp->data[pos++];
        frm->data[0] = (uint8_t)strtol(byteStr, NULL, 16);
    }
    if (frm->can_dlc > 1) {
        byteStr[0] = resp->data[pos++]; byteStr[1] = resp->data[pos++];
        frm->data[1] = (uint8_t)strtol(byteStr, NULL, 16);
    }
    if (frm->can_dlc > 2) {
        byteStr[0] = resp->data[pos++]; byteStr[1] = resp->data[pos++];
        frm->data[2] = (uint8_t)strtol(byteStr, NULL, 16);
    }
    if (frm->can_dlc > 3) {
        byteStr[0] = resp->data[pos++]; byteStr[1] = resp->data[pos++];
        frm->data[3] = (uint8_t)strtol(byteStr, NULL, 16);
    }
    if (frm->can_dlc > 4) {
        byteStr[0] = resp->data[pos++]; byteStr[1] = resp->data[pos++];
        frm->data[4] = (uint8_t)strtol(byteStr, NULL, 16);
    }
    if (frm->can_dlc > 5) {
        byteStr[0] = resp->data[pos++]; byteStr[1] = resp->data[pos++];
        frm->data[5] = (uint8_t)strtol(byteStr, NULL, 16);
    }
    if (frm->can_dlc > 6) {
        byteStr[0] = resp->data[pos++]; byteStr[1] = resp->data[pos++];
        frm->data[6] = (uint8_t)strtol(byteStr, NULL, 16);
    }
    if (frm->can_dlc > 7) {
        byteStr[0] = resp->data[pos++]; byteStr[1] = resp->data[pos++];
        frm->data[7] = (uint8_t)strtol(byteStr, NULL, 16);
    }

    // TODO Timestamp code

cleanup:
    response_free(resp);

    return res;
}
