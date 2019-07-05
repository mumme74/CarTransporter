/*
 * modified from candump.c
 *
 * Copyright (c) 2002-2009 Volkswagen Group Electronic Research
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of Volkswagen nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * Alternatively, provided that this notice is retained in full, this
 * software may be distributed under the terms of the GNU General
 * Public License ("GPL") version 2, in which case the provisions of the
 * GPL apply INSTEAD OF those given above.
 *
 * The provided data structures and external interfaces from this code
 * are not restricted to be used by modules with a GPL compatible license.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * Send feedback to <linux-can@vger.kernel.org>
 *
 */
// std headers
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <signal.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>

// unix headers
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <net/if.h>
#include <sys/select.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include "socketcan.h"
#include "canbridge.h"


#define TEMP_FAIL_RETRY( expression) { \
  int var;  \
  do { \
    var = (expression); \
  } while (var == -1 && errno == EINTR && *_abortLoop == 0); \
}

// file specific vars
static int cansock = 0,
           is_opened = 0;
static struct sockaddr_can addr;
static struct ifreq ifr;
static const int canfd_on = 1;

// these are loop control variables that aborts a deep loop from sighandler
static int _abortVar = 0;
static int *_abortLoop = &_abortVar;

// got this from https://stackoverflow.com/questions/4181784/
//  how-to-set-socket-timeout-in-c-when-making-multiple-connections
static int wait_on_sock(int sock, long timeout, int r, int w)
{
    struct timeval tv = {0,0};
    fd_set fdset;
    fd_set *rfds, *wfds;
    int n, so_error;
    unsigned so_len;

    FD_ZERO (&fdset);
    FD_SET  (sock, &fdset);
    tv.tv_sec = timeout;
    tv.tv_usec = 0;

#ifdef DEBUG
    fprintf(stdout, "wait in progress tv={%ld,%ld} ...\n",
            tv.tv_sec, tv.tv_usec);
#endif

    if (r) rfds = &fdset; else rfds = NULL;
    if (w) wfds = &fdset; else wfds = NULL;

    TEMP_FAIL_RETRY (n = select (sock+1, rfds, wfds, NULL, &tv));
    switch (n) {
    case 0:
        sprintf(canbridge_errmsg, "wait timed out\n");
        return -errno;
    case -1:
        sprintf(canbridge_errmsg, "error during wait\n");
        return -errno;
    default:
        // select tell us that sock is ready, test it
        so_len = sizeof(so_error);
        so_error = 0;
        getsockopt (sock, SOL_SOCKET, SO_ERROR, &so_error, &so_len);
        if (so_error == 0)
            return 0;
        errno = so_error;
        sprintf(canbridge_errmsg, "wait failed\n");
        return -errno;
    }
}

// -----------------------------------------------------------
// exported functions

/**
 * @brief socketcan_init, initializes, bur not open, socketcan
 * @param name, Interface to use, ie can0
 * @return 0 on error, 1 when ok
 */
int socketcan_init(const char *name)
{
    if (cansock != 0) {
        sprintf(canbridge_errmsg, "Already initialized\n");
        return 0;
    }

    cansock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (cansock < 0) {
        sprintf(canbridge_errmsg, "CAN socket failed to open");
        return 0;
    }

    addr.can_family = AF_CAN;

    memset(&ifr.ifr_name, 0, sizeof(ifr.ifr_name));
    strncpy(ifr.ifr_name, name, strlen(name));

    sprintf(canbridge_errmsg, "Using interface name '%s'.\n", ifr.ifr_name);

    // set can interface
    ifr.ifr_ifindex = (int)if_nametoindex(ifr.ifr_name);
    if (!ifr.ifr_ifindex) {
        sprintf(canbridge_errmsg, "Failed to open interface\n");
        close(cansock);
        cansock = 0;
        return 0;
    }

    return 1;
}

/**
 * @brief socketcan_set_filter, sets a CAN filter
 * @param mask, use mask to filter against
 * @param id, use this can msg ID
 * @return 0 on error, 1 when ok
 */
int socketcan_set_filter(uint32_t mask, uint32_t id)
{
    if (socketcan_status() != 1) {
        sprintf(canbridge_errmsg, "Not in initialized state\n");
        return 0;
    }

    // filter out all other messages
    struct can_filter rcvfilter;
    /*if (canIdx < 0x800) {
        // 11 bit id
        rcvfilter.can_mask = 0x7F8; // filter in only this id
    } else {
        // 29 bit (extended frame)
        rcvfilter.can_mask = 0x1FFFFFF8;
    }
    rcvfilter.can_id = canIdx;
    */
    rcvfilter.can_id = id;
    rcvfilter.can_mask = mask;
    setsockopt(cansock, SOL_CAN_RAW, CAN_RAW_FILTER, &rcvfilter, sizeof (struct can_filter));

    return 1;
}

/**
 * @brief socketcan_open, open a previously initialized
 * @return 0 on error, 1 when ok, -1 when already was opened
 */
int socketcan_open(void)
{
    int status = socketcan_status();
    if (status == 0) {
        sprintf(canbridge_errmsg, "cansock not initialized\n");
        return 0;
    } else if (status == 2) {
        sprintf(canbridge_errmsg, "Already open!\n");
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(cansock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        sprintf(canbridge_errmsg, "Cant't bind to socket");
        close(cansock);
        cansock = 0;
        return 0;
    }
    is_opened = 1;

    return 1; // success!
}

/**
 * @brief socketcan_status
 * @return CANBRIDGE_CLOSED, CANBRIDGE_INIT (but not open), CANBRIDGE_OPEN
 */
int socketcan_status(void)
{
    if (cansock != 0) {
        return is_opened != 0 ? CANBRIDGE_OPEN : CANBRIDGE_INIT;
    }
    return CANBRIDGE_CLOSED;
}

/**
 * @brief socketcan_set_fd_mode, tries to set FD mode,
 *          not all CAN controllers support this faster mode
 * @return 0 on error, 1 when ok
 */
int socketcan_set_fd_mode(void)
{
    int status = socketcan_status();
    if (status != 1) {
        sprintf(canbridge_errmsg, "Must be in init state\n");
        return 0;
    }

    /* try to switch the socket into CAN FD mode */
    int res = setsockopt(cansock, SOL_CAN_RAW, CAN_RAW_FD_FRAMES,
                         &canfd_on, sizeof(canfd_on));

    if (res != 0) {
        sprintf(canbridge_errmsg, "Couldn't set FD mode\n");
        return 0;
    }

    return 1;
}

/**
 * @brief socketcan_close
 * @return 0 on error, 1 when ok
 */
int socketcan_close()
{
    int ret = 1;
    if (cansock) {
        if (!close(cansock)) {
            if (errno == EIO)
                sprintf(canbridge_errmsg, "An I/O error occurred while reading from or writing to socket");
            else if (errno == EINTR)
                sprintf(canbridge_errmsg, "Close was interupted by signal");
            else {
                sprintf(canbridge_errmsg, "Invalid can socket when close");
            }
            ret = 0;
        }
    }

    cansock = 0;
    is_opened = 0;

    return ret;
}

/**
 * @brief socketcan_set_abortvariable, abort deep loop if variable is set
 *          socketcan re-tries to send message, program can get stuck
 * @param abortVar, a variable that can get set from a sighandler funtion
 *                  such as SIGINT
 */
void socketcan_set_abortvariable(int *abortVar)
{
    _abortLoop = abortVar;
}


int socketcan_send(canframe_t *frm, int timeoutms)
{
    ssize_t e = write(cansock, frm, CAN_MTU);
    if (e == -1 &&
        (errno == EWOULDBLOCK || errno == EAGAIN || errno == ENOBUFS))
    {
        // wait for 50ms then retry
        struct timespec time = { 0 /*sec*/, 50000000 /*nanosecs*/ };
        nanosleep(&time, NULL);
        e = write(cansock, frm, CAN_MTU);
    }

    if (e != CAN_MTU) {
        sprintf(canbridge_errmsg, "write to CAN failed errno:%d\n", errno);
        return -e;
    }
    int res = wait_on_sock(cansock, timeoutms, 0, 1);
    return res == 0 ? 1 : res;
}

int socketcan_recv(canframe_t *frm, int timeoutms)
{
    struct timeval tv;

    /* Watch stdin (fd 0) to see when it has input. */
    fd_set rdfs;
    FD_ZERO(&rdfs);
    FD_SET(cansock, &rdfs);

    /* Wait up to five seconds. */
    tv.tv_sec = timeoutms / 1000;
    tv.tv_usec = (timeoutms - tv.tv_sec) * 1000;

    int ret;
    TEMP_FAIL_RETRY (ret = select(cansock +1, &rdfs, NULL, NULL, &tv));
    /* Don't rely on the value of tv now! */
    if (ret == -1) {
        sprintf(canbridge_errmsg, "Failed to connect to CAN while receive");
        return 0;
    } else if (ret) {
        int nbytes;
        // Data is available now.
        if ((nbytes = read(cansock, frm, sizeof(*frm))) < 0) {
            sprintf(canbridge_errmsg, "CAN read from socket failed");
            return 0;
        }
        return 1;
    }
    return ret;
}


