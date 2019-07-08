/*
 * commands.c
 *
 *  Created on: 5 jul 2019
 *      Author: jof
 */

#include "slcan.h"
#include "canbridge.h"
#include "buffer.h"
#include <stdio.h>



// these are loop control variables that aborts a deep loop from sighandler
static int _abortVar = 0;
static int *_abortLoop = &_abortVar;

static int canfd = 0;

/*
************** BEGIN serialport stuff ************************
*/
#ifndef _WIN32
// posix comport here
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/time.h>
# include <sys/ioctl.h>

#ifdef __APPLE__
// for iotcl set serial speed to 1000000 baud
# include <IOKit/serial/ioss.h>
#endif

#ifdef __linux__
// to set low latency mode, 1ms
# include <linux/serial.h>
#endif

typedef struct timeval timeval_t;

static int max_latency = 20; // FTDI chips has a default 16ms latency timer
                             // it waits for either 64bytes or 16ms
                             // it is changable through DLL, but not through serial (unless its a linux)
                             // linux does however let us set 1ms latency in ioctl


// https://stackoverflow.com/questions/6947413/how-to-open-read-and-write-from-serial-port-in-c
static int open_port(const char *portname)
{
    struct termios tty;

    canfd = open(portname, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (canfd < 0) {
        sprintf(canbridge_errmsg, "Error opening %s: %s\n", portname, strerror(errno));
        return -errno;
    }
    if (!isatty(canfd)) {
        sprintf(canbridge_errmsg, "%s is not a tty device\n", portname);
        return 0;
    }

    // get attributes for this tty
    if (tcgetattr(canfd, &tty) < 0) {
        sprintf(canbridge_errmsg, "Error from tcgetattr: %s\n", strerror(errno));
        return -1;
    }

#ifndef __APPLE__
    // set speed to 1Mbit/s
    cfsetospeed(&tty, B1000000);
    cfsetispeed(&tty, B1000000);
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
        sprintf(canbridge_errmsg, "Error from tcsetattr: %s\n", strerror(errno));
        return -1;
    }

#ifdef __APPLE__
    speed_t speed = 1000000;
    if (ioctl(canfd, IOSSIOSPEED, &speed)) {
        sprintf(canbridge_errmsg, "Error when setting serial speed to 1Mbaud\n");
        return 0;
    }

#else // linux
    // set latency to max 1ms
    struct serial_struct kSerialSettings;

    if (ioctl(canfd, TIOCGSERIAL, &kSerialSettings) > -1) {
        kSerialSettings.flags |= ASYNC_LOW_LATENCY;
        if (ioctl(canfd, TIOCSSERIAL, &kSerialSettings) < 0) {
            sprintf(canbridge_errmsg,
                    "Could not set low latency to serial driver\nreson%s\n",
                    strerror(errno));
        }
        max_latency = 1;
    }
#endif

    return 1;
}

static int write_port(const char *buf, uint32_t len,
                      uint32_t *bytesWritten, int timeoutms)
{
    if (timeoutms < max_latency)
        timeoutms = max_latency;
    *bytesWritten = 0;

    timeval_t timeoutAt, initial, now, add = { 0 , 1000 * timeoutms };
    gettimeofday(&initial, NULL);

    // calculate when to bail out
    timeradd(&initial, &add, &timeoutAt);
    do {
        ssize_t res = write(canfd, buf, len);
        if (res > 0)
            *bytesWritten += (uint32_t)res;
        gettimeofday(&now, NULL);
    } while(timercmp(&now, &timeoutAt, < ) &&
            *bytesWritten < len && !*_abortLoop);

    return *bytesWritten == len;
}

static int read_port(char *buf, uint32_t len,
                     uint32_t *bytesRead, int timeoutms)
{
    if (timeoutms < max_latency)
        timeoutms = max_latency;
    *bytesRead = 0;

    timeval_t timeoutAt, initial, now, add = { 0 , 1000 * timeoutms };
    gettimeofday(&initial, NULL);

    // calculate when to bail out
    timeradd(&initial, &add, &timeoutAt);
    do {
        ssize_t res = read(canfd, buf, len);
        if (res > 0)
            *bytesRead += (uint32_t)res;
        else if (res == -1 && errno != EAGAIN)
            return 0; // read error
        gettimeofday(&now, NULL);
    } while(timercmp(&now, &timeoutAt, < ) &&
            *bytesRead < len && !*_abortLoop);

    return *bytesRead == len;
}

static int close_port(void)
{
    if (!canfd) {
        sprintf(canbridge_errmsg, "tty already closed!\n");
        return 0;
    }

    if (close(canfd) < 0) {
        sprintf(canbridge_errmsg, "Error closing tty: %s\n", strerror(errno));
        return 0;
    }

    return 1;
}

#else // _WIN32

// based on https://www.xanthium.in/Serial-Port-Programming-using-Win32-API
#include<windows.h>
#include<stdio.h>

HANDLE canfd

static int open_port(const char *port)
{
    char portname[12]; // allows up to \\.\COM999
    int len = strnlen(port, 20);
    if (len < 5)
        /* ports COM1-9 doesnt need \\.\ */
        sprintf(portname, "%s", port);
    else
        sprintf(portname, "\\\\.\\%s", port);

    canfd = CreateFileA(portname,                //port name
                        GENERIC_READ | GENERIC_WRITE, //Read/Write
                        0,                            // No Sharing
                        NULL,                         // No Security
                        OPEN_EXISTING,// Open existing port only
                        0,            // Non Overlapped I/O
                        NULL);        // Null for Comm Devices

    if (canfd == INVALID_HANDLE_VALUE) {
        sprintf(canbridge_errmsg, “Error in opening serial port\n”);
        return 0;
    }

    // ******* port settings ***********
    // setup speed, controlbits etc.
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    // get current settings
    if (!GetCommState(canfd, &dcbSerialParams)) {
        sprintf(canbridge_errmsg, "Couldn't get com parameters\n");
        return 0;
    }
    // change settings
    dcbSerialParams.BaudRate = 1000000;  // Setting BaudRate
    dcbSerialParams.ByteSize = 8;         // Setting ByteSize = 8
    dcbSerialParams.StopBits = ONESTOPBIT;// Setting StopBits = 1
    dcbSerialParams.Parity   = NOPARITY;  // Setting Parity = None
    // set settings to port
    if (!SetCommState(canfd, &dcbSerialParams)) {
        sprintf(canbridge_errmsg, "Failed to change port settings\n");
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
        sprintf(canbridge_errmsg, "Couldn't set COM timeouts.\n");
        return 0;
    }

    /*if (!SetCommMask(canfd, EV_RXCHAR | EV_TXEMPTY)) {
        sprintf(canbridge_errmsg, "Couldn't set event flag rx (EV_RXCHAR | EV_TXEMPTY)\n");
        return 0;
    }*/

    return 1;
}

// len is how many bytes to write
static int write_port(const char *buf, uint32_t len,
                      int *written, int timeoutms)
{
    DWORD nBytesWritten = 0,
          timeoutAt = timeGetTime() + (uint32_t)timeoutms;

    // continue until timeout is met
    do {
        // write to COM port
        WriteFile(canfd, buf + written, len - nBytesWritten, NULL);
        *written += nBytesWritten
    } while((timeoutAt > timeGetTime()) &&
            (*written < len) && !(*_abortLoop));

    return *written == len;
}

static int read_port(const char *buf, uint32_t len,
                     uint32_t *readBytes, int timeoutms)
{
    DWORD nBytesRead = 0,
          timeoutAt = timeGetTime() + (uint32_t)timeoutms;

    // continue until timeout is met
    do {
        // read from COM port
        while(ReadFile(canfd, buf, 1, &nBytesRead, NULL) &&
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
        sprintf(canbridge_errmsg, "Error closing port\n");
        return 0;
    }
    return 1;
}

#endif

/*
 ********************* END serialport stuff ***********************
*/

static int state = CANBRIDGE_CLOSED;
static const char CR = '\r', BELL = '\a';
static int hwVersion = 0, swVersion = 0;


// ----------------------- Responses Buffer ----------------------------------
// it is a circular buffer
#define RESPONSE_BUF_INITIAL_SZ 1024
#define RESPONSE_MAX_SZ 20
static Buffer_t readBuf;


/**
 * @brief response_get, gets a complete response from buffer
 * @param b, ptr to buffer obj
 * @param resp, store response into here, must be of size RESPONSE_MAX_SZ
 * @param cmds, Look for the first response matching cmds, might be up to 20 chrs
 * @param len, get set to number of chars in response
 * @param timeoutms, timeout a fetch in ms
 * @return 1 if a response could be found, 0 otherwise
 */
static int response_get(Buffer_t *b, char *resp, const char *cmds,
                                uint32_t *len, int timeoutms)
{
    int crPos, bellPos, peek;
    size_t cmdsLen = strnlen(cmds, 20);
    uint32_t startpos = 0;

    do {
        peek = 0;

        // first check if we have any response available
        crPos   = buffer_index_of(b, CR, startpos);
        bellPos = buffer_index_of(b, BELL, startpos);
        if (crPos < 0 && bellPos < 0) {
            // refresh
            char buf[20];
            uint32_t nBytes;
            read_port(buf, 20, &nBytes, timeoutms);
            if (nBytes == 0)
                return 0; // nothing recived

            buffer_put(b, buf, nBytes);

            // read from a potentially newly filled buffer
            crPos   = buffer_index_of(b, CR, startpos);
            bellPos = buffer_index_of(b, BELL, startpos);
        }

        // have we found a response? CR or BELL
        if (crPos > -1 &&
            (bellPos == -1 || (bellPos > -1 && bellPos > crPos)))
        {
            *len = (uint32_t)crPos +1;
            peek = buffer_get(b, resp, *len);

        } else if (bellPos > -1) {
            *len = (uint32_t)bellPos +1;
            peek = buffer_get(b, resp, *len);
        }

        // if peek then we have a line end
        if (peek > 0) {
            // should we look for a specific response?
            if (cmdsLen > 0) {
                for (size_t i = 0; i < cmdsLen; ++i) {
                    if (cmds[i] == resp[0]) {
                        return 1; // found it!
                    }
                }
                // loop again, try next response in buffer
                startpos += (uint32_t)peek;
            } else {
                // don't filter on cmds, ie. we found it now!
                return 1;
            }
        }
    } while(startpos < buffer_available(b) && peek > 0);

    return 0;
}

// ---------------------end Responses Buffer ---------------------------------



static int clear_pipeline(void) {
    // canusb.com states:
    // Always start each session (when your program starts) with sending 2-3 [CR]
    buffer_clear(&readBuf);

    for(int i = 0; i < 3; ++i) {
        char ch = CR;
        uint32_t nBytes;
        if (!write_port(&ch, 1, &nBytes, 10)) {
            sprintf(canbridge_errmsg, "Failed to clear tranmitt buffer\n");
            return 0;
        }

        // clear reponse buffer, 1char at a time until CR is recieved
        // we might have old responses in pipeline
        char buf[RESPONSE_MAX_SZ];
        if (!response_get(&readBuf, buf, "\r\a", &nBytes, 5)) {
            sprintf(canbridge_errmsg, "Failed to get response while clearing serial buffer\n");
            return 0;
        }
    }

    buffer_clear(&readBuf);

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
    if (!buffer_init(&readBuf, RESPONSE_BUF_INITIAL_SZ))
        return 0;

    // this driver needs a open
    if (open_port(name) < 1)
        return 0;

    state = CANBRIDGE_INIT;

    // clear old messages
    if (!clear_pipeline())
        return 0;

    // canusb states we must get version before anything else
    char buf[RESPONSE_MAX_SZ] = { 'V', CR };
    uint32_t nBytes;
    if (!write_port(buf, 2, &nBytes, 5)) {
        sprintf(canbridge_errmsg, "Failed to send version request\n");
        return 0;
    }

    if (!response_get(&readBuf, buf, "V", &nBytes, 5)) {
        sprintf(canbridge_errmsg, "Failed to read version response\n");
        return 0;
    }
    if (buf[0] == BELL) {
        sprintf(canbridge_errmsg, "Version request responded with error\n");
        return 0;
    }
    const int versionSz = 6;
    if (nBytes < versionSz) {
        sprintf(canbridge_errmsg, "Wrong response from device version request\n");
        return 0;
    }
    // read the version
    char hwStr[3] = { buf[1], buf[2], 0 },
         swStr[3] = { buf[3], buf[4], 0 };
    hwVersion = atoi(hwStr);
    swVersion = atoi(swStr);


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
        sprintf(canbridge_errmsg, "Unsupported CAN speed\n");
        return 0;
    }

    if (!write_port(buf, 3, &nBytes, 5)) {
        sprintf(canbridge_errmsg, "Failed to write speed cmd to serial\n");
        return 0;
    }

    if (!response_get(&readBuf, buf, "\r\a", &nBytes, 5)) {
        sprintf(canbridge_errmsg, "Failed to read from speed cmd response from serial\n");
        return 0;
    }
    if (buf[0] != CR) {
        sprintf(canbridge_errmsg, "Set speed command failed in device\n");
        return 0;
    }

    return 1;
}

/**
 * @brief slcan_set_filter, sets a CAN filter
 * @param mask, use mask to filter against
 * @param id, use this can msg ID
 * @return 0 on error, 1 when ok
 */
int slcan_set_filter(uint32_t mask, uint32_t id)
{
    if (state != CANBRIDGE_INIT) {
        sprintf(canbridge_errmsg, "Can only set filter in init mode\n");
        return 0;
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
        sprintf(&buf[j],"%02X", code[i].b0); // lsb
        j += 2; // adds 2 chars above ie: FF
        sprintf(&buf[j],"%02X", code[i].b1);
        j += 2;
        sprintf(&buf[j],"%02X", code[i].b2);
        j += 2;
        sprintf(&buf[j],"%02X", code[i].b3); // msb
        j += 2;
        buf[j++] = CR; // end marker

        if (!write_port(buf, j, &nBytes, 10)) {
            sprintf(canbridge_errmsg,
                    "Failed to send CAN filter to serial\nwritten:%d of %d",
                    nBytes, j);
            return 0;
        }

        // get response
        if (!response_get(&readBuf, buf, "\r\a", &nBytes, 5)) {
            sprintf(canbridge_errmsg, "Failed to set CAN filter, reponse timeout\n");
            return 0;
        }
        if (buf[0] != CR) {
            sprintf(canbridge_errmsg, "Failed to set CAN filter\n");
            return 0;
        }
    }

    return 1;
}

/**
 * @brief slcan_open, open a previously initialized
 * @return 0 on error, 1 when ok, -1 when already was opened
 */
int slcan_open(void)
{
    if (state == CANBRIDGE_CLOSED) {
        sprintf(canbridge_errmsg, "Can't open a closed comport\n");
        return 0;
    } else if (state == CANBRIDGE_OPEN) {
        sprintf(canbridge_errmsg, "Already open comport\n");
        return -1;
    }

    // open CAN channel
    uint32_t nBytes;
    char buf[RESPONSE_MAX_SZ] = { 'O', CR };
    if (!write_port(buf, 2, &nBytes, 10)) {
        sprintf(canbridge_errmsg, "Failed to send open cmd to serial");
        return 0;
    }

    // get response
    if (!response_get(&readBuf, buf, "\r\a", &nBytes, 5)) {
        sprintf(canbridge_errmsg, "Failed to open CAN channel, reponse timeout\n");
        return 0;
    }
    if (buf[0] != CR) {
        sprintf(canbridge_errmsg, "Failed to open CAN channel\n");
        return 0;
    }

    // success!
    state = CANBRIDGE_OPEN;
    return 1;
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
    int res = 1;
    if (state == CANBRIDGE_CLOSED) {
        sprintf(canbridge_errmsg, "Already closed!\n");
        res = 0;
        goto cleanup;
    }

    // close possible CAN channel
    if (state == CANBRIDGE_OPEN) {
        // close CAN channel
        // start with purging response buffer
        buffer_clear(&readBuf);

        uint32_t nBytes;
        char buf[RESPONSE_MAX_SZ] = { 'C', CR };
        if (!write_port(buf, 2, &nBytes, 5)) {
            sprintf(canbridge_errmsg, "Failed to send close cmd to serial");
            res = 0;
            goto cleanup;
        }

        // get response
        if (!response_get(&readBuf, buf, "\r\a", &nBytes, 5)) {
            sprintf(canbridge_errmsg, "Failed to close CAN channel, no response from device\n");
            res = 0;
            goto cleanup;
        }
        if (buf[0] != CR) {
            sprintf(canbridge_errmsg, "Failed to close CAN channel, device responded with error\n");
            res = 0;
            goto cleanup;
        }
    }

cleanup:
    buffer_free(&readBuf);

    res = close_port(); // close serial
    if (res)
        state = CANBRIDGE_CLOSED;

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
    if (state != CANBRIDGE_OPEN) {
        sprintf(canbridge_errmsg, "Can't send on a non open driver\n");
        return 0;
    }

    uint32_t pos = 0, nBytes;
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
        sprintf(&buf[pos++], "%01X", id.b3);
        sprintf(&buf[pos++], "%02X", id.b2);
        sprintf(&buf[pos++], "%02X", id.b1);
    } else
        // 11bits should only print 3chars
        sprintf(&buf[pos++], "%01X", id.b1);
    sprintf(&buf[pos++], "%02X", id.b0);

    // length of frame
    sprintf(&buf[pos++], "%01X", frm->can_dlc);

    // no payload on rtr frame
    if (frm->can_id & CAN_RTR_FLAG) {
        // then payload
        byte8_t payload = { 0 };
        for(int i = 0; i < frm->can_dlc; ++i)
            payload.arr[i] = frm->data[i];

        if (frm->can_dlc > 7)
            sprintf(&buf[pos++], "%02X", payload.b7);
        if (frm->can_dlc > 6)
            sprintf(&buf[pos++], "%02X", payload.b6);
        if (frm->can_dlc > 5)
            sprintf(&buf[pos++], "%02X", payload.b5);
        if (frm->can_dlc > 4)
            sprintf(&buf[pos++], "%02X", payload.b4);
        if (frm->can_dlc > 3)
            sprintf(&buf[pos++], "%02X", payload.b3);
        if (frm->can_dlc > 2)
            sprintf(&buf[pos++], "%02X", payload.b2);
        if (frm->can_dlc > 1)
            sprintf(&buf[pos++], "%02X", payload.b1);
        if (frm->can_dlc > 0)
            sprintf(&buf[pos++], "%02X", payload.b0);
    }

    // end string
    buf[pos++] = CR;

    if (!write_port(buf, pos, &nBytes, timeoutms)) {
        sprintf(canbridge_errmsg, "Failed to send to serial\nwritten:%d of %d", nBytes, pos);
        return 0; // send_cmd sets canbridge_errmsg
    }

    // get response
    char okCmds[2] = { (frm->can_id & CAN_EFF_FLAG) ? 'Z' : 'z', 0 };
    if (!response_get(&readBuf, buf, okCmds, &nBytes, timeoutms)) {
        sprintf(canbridge_errmsg, "Failed to send to CAN channel, reponse timeout\n");
        return 0;
    }
    if (buf[1] != CR) {
        sprintf(canbridge_errmsg, "Failed to send to CAN channel\n");
        return 0;
    }

    return 1;
}

/**
 * @brief slcan_recv, recive fromcan
 * @param frm, frame to fill when recv occurs
 * @param timeoutms, timeout if no recieved frm within timeout
 * @return true if we have a frm
 */
int slcan_recv(canframe_t *frm, int timeoutms)
{
    if (state != CANBRIDGE_OPEN) {
        sprintf(canbridge_errmsg, "Can't send on a non open driver\n");
        return 0;
    }

    uint32_t pos = 0, nBytes;
    char buf[RESPONSE_MAX_SZ];
    if (!response_get(&readBuf, buf, "tTrR", &nBytes, timeoutms))
        return 0;

    if (nBytes < 5)
        return 0;

    memset(frm, 0, sizeof (*frm));

    // parse id
    if (buf[0] == 't' || buf[0] == 'r') {
        // 11 bit ID
        char id11[] = { buf[1], buf[2], buf[3], 0 };
        frm->can_id = (uint32_t)strtol(id11, NULL, 16);
        frm->can_dlc = (uint8_t)atoi(&buf[4]);
        pos = 5;
    } else {
        // 29 bit id
        char id29[] = { buf[1], buf[2], buf[3], buf[4],
                        buf[5], buf[6], buf[7], buf[8], 0 };
        frm->can_id = (uint32_t)strtol(id29, NULL, 16) | CAN_EFF_FLAG;
        frm->can_dlc = (uint8_t)atoi(&buf[9]);
        pos = 10;
    }

    // remote frame
    if (buf[0] == 'r' || buf[0] == 'R')
        frm->can_id |= CAN_RTR_FLAG;

    char byteStr[3] = { 0 };
    if (frm->can_dlc > 7) {
        byteStr[0] = buf[pos++]; byteStr[1] = buf[pos++];
        frm->data[7] = (uint8_t)strtol(byteStr, NULL, 16);
    }
    if (frm->can_dlc > 6) {
        byteStr[0] = buf[pos++]; byteStr[1] = buf[pos++];
        frm->data[6] = (uint8_t)strtol(byteStr, NULL, 16);
    }
    if (frm->can_dlc > 5) {
        byteStr[0] = buf[pos++]; byteStr[1] = buf[pos++];
        frm->data[5] = (uint8_t)strtol(byteStr, NULL, 16);
    }
    if (frm->can_dlc > 4) {
        byteStr[0] = buf[pos++]; byteStr[1] = buf[pos++];
        frm->data[4] = (uint8_t)strtol(byteStr, NULL, 16);
    }
    if (frm->can_dlc > 3) {
        byteStr[0] = buf[pos++]; byteStr[1] = buf[pos++];
        frm->data[3] = (uint8_t)strtol(byteStr, NULL, 16);
    }
    if (frm->can_dlc > 2) {
        byteStr[0] = buf[pos++]; byteStr[1] = buf[pos++];
        frm->data[2] = (uint8_t)strtol(byteStr, NULL, 16);
    }
    if (frm->can_dlc > 1) {
        byteStr[0] = buf[pos++]; byteStr[1] = buf[pos++];
        frm->data[1] = (uint8_t)strtol(byteStr, NULL, 16);
    }
    if (frm->can_dlc > 0) {
        byteStr[0] = buf[pos++]; byteStr[1] = buf[pos++];
        frm->data[0] = (uint8_t)strtol(byteStr, NULL, 16);
    }

    return 1;
}
