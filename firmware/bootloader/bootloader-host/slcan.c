/*
 * commands.c
 *
 *  Created on: 5 jul 2019
 *      Author: jof
 */

#include "slcan.h"
#include "canbridge.h"
#include <stdio.h>

static int canfd = 0;


#ifndef _WIN32
// posix comport here
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

// https://stackoverflow.com/questions/6947413/how-to-open-read-and-write-from-serial-port-in-c
int set_interface_attribs(int fd, uint32_t speed)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        sprintf(canbridge_errmsg, "Error from tcgetattr: %s\n", strerror(errno));
        return -1;
    }

    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);

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

    /* fetch bytes as they become available */
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        sprintf(canbridge_errmsg, "Error from tcsetattr: %s\n", strerror(errno));
        return -1;
    }
    return 1;
}

int set_mincount(int fd, int mcount)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        sprintf(canbridge_errmsg, "Error tcgetattr: %s\n", strerror(errno));
        return -errno;
    }

    tty.c_cc[VMIN] = mcount ? 1 : 0;
    tty.c_cc[VTIME] = 5;        /* half second timer */

    if (tcsetattr(fd, TCSANOW, &tty) < 0) {
        sprintf(canbridge_errmsg, "Error tcsetattr: %s\n", strerror(errno));
        return -errno;
    }
    return 1;
}


int main()
{
    char *portname = "/dev/ttyUSB0";
    int wlen;

    canfd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (canfd < 0) {
        sprintf(canbridge_errmsg, "Error opening %s: %s\n", portname, strerror(errno));
        return -1;
    }
    /*baudrate 115200, 8 bits, no parity, 1 stop bit */
    set_interface_attribs(canfd, B115200);
    //set_mincount(fd, 0);                /* set to pure timed read */

    /* simple output */
    wlen = write(canfd, "Hello!\n", 7);
    if (wlen != 7) {
        printf("Error from write: %d, %d\n", wlen, errno);
    }
    tcdrain(canfd);    /* delay for output */


    /* simple noncanonical input */
    do {
        unsigned char buf[80];
        int rdlen;

        rdlen = read(canfd, buf, sizeof(buf) - 1);
        if (rdlen > 0) {
#ifdef DEBUG
            buf[rdlen] = 0;
            printf("Read %d: \"%s\"\n", rdlen, buf);
#else /* display hex */
            unsigned char   *p;
            printf("Read %d:", rdlen);
            for (p = buf; rdlen-- > 0; p++)
                printf(" 0x%x", *p);
            printf("\n");
#endif
        } else if (rdlen < 0) {
            printf("Error from read: %d: %s\n", rdlen, strerror(errno));
        } else {  /* rdlen == 0 */
            printf("Timeout from read\n");
        }
        /* repeat read to get full message */
    } while (1);
}
#else // _WIN32

// based on https://www.xanthium.in/Serial-Port-Programming-using-Win32-API
#include<windows.h>
#include<stdio.h>

HANDLE canfd

static int init_port(const char *port, uint32_t speed)
{
    char portname[12]; // allows up to \\.\COM999
    int len = strnlen(port, 20);
    if (len < 5)
        // ports COM1-9 doesnt need \\.\
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
    if (speed > 1000000)
        speed = 1000000;
    dcbSerialParams.BaudRate = speed;  // Setting BaudRate
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
static int write_port(const char *buf, int len,
                      int *written, int timeoutms)
{
    DWORD nBytesWritten = 0,
          timeoutAt = timeGetTime() + (uint32_t)timeoutms;

    // continue until timeout is met
    do {
        // write to COM port
        WriteFile(canfd, buf + written, len - nBytesWritten, NULL);
        written += nBytesWritten
    } while((timeoutAt > timeGetTime()) && (nByteWritten < len));

    return written == len;
}

static int read_port(const char *buf, int *len, int timeoutms)
{
    DWORD nBytesRead = 0,
          timeoutAt = timeGetTime() + (uint32_t)timeoutms;

    // continue until timeout is met
    do {
        // read from COM port
        while(ReadFile(canfd, buf, 1, &nBytesRead, NULL) &&
              nBytesRead > 0)
        {
            len += nBytesRead;
        }
    } while(timeoutAt > timeGetTime());

    return len > 0 ? 1: 0;
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



// ----------------------------------------------------------------
// begin exported functions
/**
 * @brief slcan_init, initializes, bur not open, slcan
 * @param name, Interface to use, ie can0
 * @return 0 on error, 1 when ok
 */
int slcan_init(const char *name)
{

}

/**
 * @brief slcan_set_filter, sets a CAN filter
 * @param mask, use mask to filter against
 * @param id, use this can msg ID
 * @return 0 on error, 1 when ok
 */
int slcan_set_filter(uint32_t mask, uint32_t id)
{

}

/**
 * @brief slcan_open, open a previously initialized
 * @return 0 on error, 1 when ok, -1 when already was opened
 */
int slcan_open(void)
{

}

/**
 * @brief slcan_status
 * @return CANBRIDGE_CLOSED, CANBRIDGE_INIT (but not open), CANBRIDGE_OPEN
 */
int slcan_status(void)
{

}

/**
 * @brief slcan_set_fd_mode, tries to set FD mode,
 *          not all CAN controllers support this faster mode
 * @return 0 on error, 1 when ok
 */
int slcan_set_fd_mode(void)
{

}

/**
 * @brief slcan_close
 * @return 0 on error, 1 when ok
 */
int slcan_close(void)
{

}

/**
 * @brief slcan_set_abortvariable, abort deep loop if variable is set
 *          slcan re-tries to send message, program can get stuck
 * @param abortVar, a variable that can get set from a sighandler funtion
 *                  such as SIGINT
 */
void slcan_set_abortvariable(int *abortVar)
{

}

/**
 * @brief slcan_send, send on can
 * @param frm, frame to send
 * @param timeoutms, timeout if not send within
 * @return true on success
 */
int slcan_send(canframe_t *frm, int timeoutms)
{

}

/**
 * @brief slcan_recv, recive fromcan
 * @param frm, frame to fill when recv occurs
 * @param timeoutms, timeout if no recieved frm within timeout
 * @return true if we have a frm
 */
int slcan_recv(canframe_t *frm, int timeoutms)
{

}
