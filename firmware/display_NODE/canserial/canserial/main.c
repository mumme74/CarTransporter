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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <libgen.h>
#include <time.h>
#include <errno.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <net/if.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include "terminal.h"

/* for hardware timestamps - since Linux 2.6.30 */
#ifndef SO_TIMESTAMPING
#define SO_TIMESTAMPING 37
#endif

/* from #include <linux/net_tstamp.h> - since Linux 2.6.30 */
#define SOF_TIMESTAMPING_SOFTWARE (1<<4)
#define SOF_TIMESTAMPING_RX_SOFTWARE (1<<3)
#define SOF_TIMESTAMPING_RAW_HARDWARE (1<<6)

#define MAXSOCK 16    /* max. number of CAN interfaces given on the cmdline */
#define MAXIFNAMES 30 /* size of receive name index to omit ioctls */
#define MAXCOL 6      /* number of different colors for colorized output */
#define ANYDEV "any"  /* name of interface to receive from any CAN interface */
#define ANL "\r\n"    /* newline in ASC mode */

#define SILENT_INI 42 /* detect user setting on commandline */
#define SILENT_OFF 0  /* no silent mode */
#define SILENT_ANI 1  /* silent mode with animation */
#define SILENT_ON  2  /* silent mode (completely silent) */

#define BOLD    ATTBOLD
#define RED     ATTBOLD FGRED
#define GREEN   ATTBOLD FGGREEN
#define YELLOW  ATTBOLD FGYELLOW
#define BLUE    ATTBOLD FGBLUE
#define MAGENTA ATTBOLD FGMAGENTA
#define CYAN    ATTBOLD FGCYAN

const char col_on [MAXCOL][19] = {BLUE, RED, GREEN, BOLD, MAGENTA, CYAN};
const char col_off [] = ATTRESET;

static char *cmdlinename[MAXSOCK];
static __u32 dropcnt[MAXSOCK];
static __u32 last_dropcnt[MAXSOCK];
static char devname[MAXIFNAMES][IFNAMSIZ+1];
static int  dindex[MAXIFNAMES];
static int  max_devname_len; /* to prevent frazzled device name output */
const int canfd_on = 1;

#define MAXANI 4
const char anichar[MAXANI] = {'|', '/', '-', '\\'};
const char extra_m_info[4][4] = {"- -", "B -", "- E", "B E"};

extern int optind, opterr, optopt;

static volatile int running = 1;

// gets all frames for this string
typedef struct {
    uint8_t len;
    uint8_t written;
    union {
        char bufAll[7 * 0x7F + 1]; // +1 for \0 char
        char bufFr[18][7];
    };
} complete_t;

void print_usage(char *prg)
{
    fprintf(stderr, "\nUsage: %s [options] <CAN interface>+\n", prg);
    fprintf(stderr, "  (use CTRL-C to terminate %s)\n\n", prg);
    fprintf(stderr, "\n");
    fprintf(stderr, "A CAN interface with optional filter sets can be specified\n");
    fprintf(stderr, "on the commandline in the form: <ifname>[,filter]*\n");
    fprintf(stderr, "\nComma separated filters can be specified for each given CAN interface:\n");
    fprintf(stderr, " <can_id>:<can_mask> (matches when <received_can_id> & mask == can_id & mask)\n");
    fprintf(stderr, " <can_id>~<can_mask> (matches when <received_can_id> & mask != can_id & mask)\n");
    fprintf(stderr, " #<error_mask>       (set error frame filter, see include/linux/can/error.h)\n");
    fprintf(stderr, " [j|J]               (join the given CAN filters - logical AND semantic)\n");
    fprintf(stderr, "\nCAN IDs, masks and data content are given and expected in hexadecimal values.\n");
    fprintf(stderr, "When can_id and can_mask are both 8 digits, they are assumed to be 29 bit EFF.\n");
    fprintf(stderr, "Without any given filter all data frames are received ('0:0' default filter).\n");
    fprintf(stderr, "\nUse interface name '%s' to receive from all CAN interfaces.\n", ANYDEV);
    fprintf(stderr, "\nExamples:\n");
    fprintf(stderr, "%s -c -c -ta can0,123:7FF,400:700,#000000FF can2,400~7F0 can3 can8\n", prg);
    fprintf(stderr, "%s -l any,0~0,#FFFFFFFF    (log only error frames but no(!) data frames)\n", prg);
    fprintf(stderr, "%s -l any,0:0,#FFFFFFFF    (log error frames and also all data frames)\n", prg);
    fprintf(stderr, "%s vcan2,92345678:DFFFFFFF (match only for extended CAN ID 12345678)\n", prg);
    fprintf(stderr, "%s vcan2,123:7FF (matches CAN ID 123 - including EFF and RTR frames)\n", prg);
    fprintf(stderr, "%s vcan2,123:C00007FF (matches CAN ID 123 - only SFF and non-RTR frames)\n", prg);
    fprintf(stderr, "\n");
}

void sigterm(int signo)
{
    running = 0;
}

int idx2dindex(int ifidx, int socket) {

    int i;
    struct ifreq ifr;

    for (i=0; i < MAXIFNAMES; i++) {
        if (dindex[i] == ifidx)
            return i;
    }

    /* create new interface index cache entry */

    /* remove index cache zombies first */
    for (i=0; i < MAXIFNAMES; i++) {
        if (dindex[i]) {
            ifr.ifr_ifindex = dindex[i];
            if (ioctl(socket, SIOCGIFNAME, &ifr) < 0)
                dindex[i] = 0;
        }
    }

    for (i=0; i < MAXIFNAMES; i++)
        if (!dindex[i]) /* free entry */
            break;

    if (i == MAXIFNAMES) {
        fprintf(stderr, "Interface index cache only supports %d interfaces.\n",
               MAXIFNAMES);
        exit(1);
    }

    dindex[i] = ifidx;

    ifr.ifr_ifindex = ifidx;
    if (ioctl(socket, SIOCGIFNAME, &ifr) < 0)
        perror("SIOCGIFNAME");

    if (max_devname_len < strlen(ifr.ifr_name))
        max_devname_len = strlen(ifr.ifr_name);

    strcpy(devname[i], ifr.ifr_name);

#ifdef DEBUG
    printf("new index %d (%s)\n", i, devname[i]);
#endif

    return i;
}

int main(int argc, char **argv)
{
    fd_set rdfs;
    int s[MAXSOCK];
    int bridge = 0;
    useconds_t bridge_delay = 0;
    unsigned char down_causes_exit = 1;
    int count = 0;
    int rcvbuf_size = 0;
    int ret;
    int currmax, numfilter;
    int join_filter;
    char *ptr, *nptr;
    struct sockaddr_can addr;
    char ctrlmsg[CMSG_SPACE(sizeof(struct timeval) + 3*sizeof(struct timespec) + sizeof(__u32))];
    struct iovec iov;
    struct msghdr msg;
    struct cmsghdr *cmsg;
    struct can_filter *rfilter;
    can_err_mask_t err_mask;
    struct canfd_frame frame;
    int nbytes, i, maxdlen;
    struct ifreq ifr;
    struct timeval tv, last_tv;
    struct timeval timeout_config = { 0, 0 }, *timeout_current = NULL;

    signal(SIGTERM, sigterm);
    signal(SIGHUP, sigterm);
    signal(SIGINT, sigterm);

    last_tv.tv_sec  = 0;
    last_tv.tv_usec = 0;

    if (optind == argc) {
        print_usage(basename(argv[0]));
        exit(0);
    }

    currmax = argc - optind; /* find real number of CAN devices */

    if (currmax > MAXSOCK) {
        fprintf(stderr, "More than %d CAN devices given on commandline!\n", MAXSOCK);
        return 1;
    }

    for (i=0; i < currmax; i++) {

        ptr = argv[optind+i];
        nptr = strchr(ptr, ',');

#ifdef DEBUG
        printf("open %d '%s'.\n", i, ptr);
#endif

        s[i] = socket(PF_CAN, SOCK_RAW, CAN_RAW);
        if (s[i] < 0) {
            perror("socket");
            return 1;
        }

        cmdlinename[i] = ptr; /* save pointer to cmdline name of this socket */

        if (nptr)
            nbytes = nptr - ptr;  /* interface name is up the first ',' */
        else
            nbytes = strlen(ptr); /* no ',' found => no filter definitions */

        if (nbytes >= IFNAMSIZ) {
            fprintf(stderr, "name of CAN device '%s' is too long!\n", ptr);
            return 1;
        }

        if (nbytes > max_devname_len)
            max_devname_len = nbytes; /* for nice printing */

        addr.can_family = AF_CAN;

        memset(&ifr.ifr_name, 0, sizeof(ifr.ifr_name));
        strncpy(ifr.ifr_name, ptr, nbytes);

#ifdef DEBUG
        printf("using interface name '%s'.\n", ifr.ifr_name);
#endif

        if (strcmp(ANYDEV, ifr.ifr_name)) {
            if (ioctl(s[i], SIOCGIFINDEX, &ifr) < 0) {
                perror("SIOCGIFINDEX");
                exit(1);
            }
            addr.can_ifindex = ifr.ifr_ifindex;
        } else
            addr.can_ifindex = 0; /* any can interface */

        if (nptr) {

            /* found a ',' after the interface name => check for filters */

            /* determine number of filters to alloc the filter space */
            numfilter = 0;
            ptr = nptr;
            while (ptr) {
                numfilter++;
                ptr++; /* hop behind the ',' */
                ptr = strchr(ptr, ','); /* exit condition */
            }

            rfilter = malloc(sizeof(struct can_filter) * numfilter);
            if (!rfilter) {
                fprintf(stderr, "Failed to create filter space!\n");
                return 1;
            }

            numfilter = 0;
            err_mask = 0;
            join_filter = 0;

            while (nptr) {

                ptr = nptr+1; /* hop behind the ',' */
                nptr = strchr(ptr, ','); /* update exit condition */

                if (sscanf(ptr, "%x:%x",
                       &rfilter[numfilter].can_id,
                       &rfilter[numfilter].can_mask) == 2) {
                    rfilter[numfilter].can_mask &= ~CAN_ERR_FLAG;
                    numfilter++;
                } else if (sscanf(ptr, "%x~%x",
                          &rfilter[numfilter].can_id,
                          &rfilter[numfilter].can_mask) == 2) {
                    rfilter[numfilter].can_id |= CAN_INV_FILTER;
                    rfilter[numfilter].can_mask &= ~CAN_ERR_FLAG;
                    numfilter++;
                } else if (*ptr == 'j' || *ptr == 'J') {
                    join_filter = 1;
                } else if (sscanf(ptr, "#%x", &err_mask) != 1) {
                    fprintf(stderr, "Error in filter option parsing: '%s'\n", ptr);
                    return 1;
                }
            }

            if (err_mask)
                setsockopt(s[i], SOL_CAN_RAW, CAN_RAW_ERR_FILTER,
                       &err_mask, sizeof(err_mask));

            if (join_filter && setsockopt(s[i], SOL_CAN_RAW, CAN_RAW_JOIN_FILTERS,
                              &join_filter, sizeof(join_filter)) < 0) {
                perror("setsockopt CAN_RAW_JOIN_FILTERS not supported by your Linux Kernel");
                return 1;
            }

            if (numfilter)
                setsockopt(s[i], SOL_CAN_RAW, CAN_RAW_FILTER,
                       rfilter, numfilter * sizeof(struct can_filter));

            free(rfilter);

        } /* if (nptr) */

        /* try to switch the socket into CAN FD mode */
        setsockopt(s[i], SOL_CAN_RAW, CAN_RAW_FD_FRAMES, &canfd_on, sizeof(canfd_on));

        if (rcvbuf_size) {

            int curr_rcvbuf_size;
            socklen_t curr_rcvbuf_size_len = sizeof(curr_rcvbuf_size);

            /* try SO_RCVBUFFORCE first, if we run with CAP_NET_ADMIN */
            if (setsockopt(s[i], SOL_SOCKET, SO_RCVBUFFORCE,
                       &rcvbuf_size, sizeof(rcvbuf_size)) < 0) {
#ifdef DEBUG
                printf("SO_RCVBUFFORCE failed so try SO_RCVBUF ...\n");
#endif
                if (setsockopt(s[i], SOL_SOCKET, SO_RCVBUF,
                           &rcvbuf_size, sizeof(rcvbuf_size)) < 0) {
                    perror("setsockopt SO_RCVBUF");
                    return 1;
                }

                if (getsockopt(s[i], SOL_SOCKET, SO_RCVBUF,
                           &curr_rcvbuf_size, &curr_rcvbuf_size_len) < 0) {
                    perror("getsockopt SO_RCVBUF");
                    return 1;
                }

                /* Only print a warning the first time we detect the adjustment */
                /* n.b.: The wanted size is doubled in Linux in net/sore/sock.c */
                if (!i && curr_rcvbuf_size < rcvbuf_size*2)
                    fprintf(stderr, "The socket receive buffer size was "
                        "adjusted due to /proc/sys/net/core/rmem_max.\n");
            }
        }

        if (bind(s[i], (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            perror("bind");
            return 1;
        }
    }


    /* these settings are static and can be held out of the hot path */
    iov.iov_base = &frame;
    msg.msg_name = &addr;
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = &ctrlmsg;

    complete_t lineBuf;
    lineBuf.bufAll[0x80] = '\0';
    lineBuf.len = lineBuf.written = 0;

    while (running) {

        FD_ZERO(&rdfs);
        for (i=0; i<currmax; i++)
            FD_SET(s[i], &rdfs);

        if (timeout_current)
            *timeout_current = timeout_config;

        if ((ret = select(s[currmax-1]+1, &rdfs, NULL, NULL, timeout_current)) <= 0) {
            //perror("select");
            running = 0;
            continue;
        }

//        for (i=0; i<currmax; i++) {  /* check all CAN RAW sockets */
            i = 0;
            if (FD_ISSET(s[i], &rdfs)) {

                int idx;

                /* these settings may be modified by recvmsg() */
                iov.iov_len = sizeof(frame);
                msg.msg_namelen = sizeof(addr);
                msg.msg_controllen = sizeof(ctrlmsg);
                msg.msg_flags = 0;

                nbytes = recvmsg(s[i], &msg, 0);
                idx = idx2dindex(addr.can_ifindex, s[i]);

                if (nbytes < 0) {
                    if ((errno == ENETDOWN) && !down_causes_exit) {
                        fprintf(stderr, "%s: interface down\n", devname[idx]);
                        continue;
                    }
                    perror("read");
                    return 1;
                }

                if ((size_t)nbytes == CAN_MTU)
                    maxdlen = CAN_MAX_DLEN;
                else if ((size_t)nbytes == CANFD_MTU)
                    maxdlen = CANFD_MAX_DLEN;
                else {
                    fprintf(stderr, "read: incomplete CAN frame\n");
                    return 1;
                }

                if (count && (--count == 0))
                    running = 0;

                if (bridge) {
                    if (bridge_delay)
                        usleep(bridge_delay);

                    nbytes = write(bridge, &frame, nbytes);
                    if (nbytes < 0) {
                        perror("bridge write");
                        return 1;
                    } else if ((size_t)nbytes != CAN_MTU && (size_t)nbytes != CANFD_MTU) {
                        fprintf(stderr,"bridge write: incomplete CAN frame\n");
                        return 1;
                    }
                }

                for (cmsg = CMSG_FIRSTHDR(&msg);
                     cmsg && (cmsg->cmsg_level == SOL_SOCKET);
                     cmsg = CMSG_NXTHDR(&msg,cmsg)) {
                    if (cmsg->cmsg_type == SO_TIMESTAMP) {
                        memcpy(&tv, CMSG_DATA(cmsg), sizeof(tv));
                    } else if (cmsg->cmsg_type == SO_TIMESTAMPING) {

                        struct timespec *stamp = (struct timespec *)CMSG_DATA(cmsg);

                        /*
                         * stamp[0] is the software timestamp
                         * stamp[1] is deprecated
                         * stamp[2] is the raw hardware timestamp
                         * See chapter 2.1.2 Receive timestamps in
                         * linux/Documentation/networking/timestamping.txt
                         */
                        tv.tv_sec = stamp[2].tv_sec;
                        tv.tv_usec = stamp[2].tv_nsec/1000;
                    } else if (cmsg->cmsg_type == SO_RXQ_OVFL)
                        memcpy(&dropcnt[i], CMSG_DATA(cmsg), sizeof(__u32));
                }

                /* check for (unlikely) dropped frames on this specific socket */
                if (dropcnt[i] != last_dropcnt[i]) {

                    __u32 frames = dropcnt[i] - last_dropcnt[i];

                   printf("DROPCOUNT: dropped %d CAN frame%s on '%s' socket (total drops %d)\n",
                          frames, (frames > 1)?"s":"", devname[idx], dropcnt[i]);


                    last_dropcnt[i] = dropcnt[i];
                }

                int sendIdx;
                if (frame.data[0] & 0x80) {
                    // new message
                    // first send old
                    if (lineBuf.len) {
                        printf("%s", lineBuf.bufAll);
                        fflush(stdout);
                    }
                    sendIdx = 0;
                    lineBuf.len = frame.data[0] & 0x7F;
                    lineBuf.written = frame.len -1;
                } else {
                    // continued msg
                    sendIdx = frame.data[0] & 0x7F;
                    lineBuf.written += frame.len -1;
                }

                // save this msg to buffer
                if (lineBuf.written <= lineBuf.len) {
                    for (int n = 1; n < frame.len; ++n) {
                        lineBuf.bufAll[(sendIdx*7) + n -1] = frame.data[n];
                    }
                }
                //printf("\nframeid:%x d[0]:%x new:%d f.len:%d", frame.can_id, frame.data[0], frame.data[0] & 0x80, frame.len);
                //printf(" sendIdx:%d lineBuf.len:%d linebuf.written:%d\n", sendIdx, lineBuf.len, lineBuf.written);


                // print this string
                if (lineBuf.written >= lineBuf.len) {
                    printf("%s", lineBuf.bufAll);
                    //for(int n = 0; n < lineBuf.len;++n)
                    //    printf("%x,", lineBuf.bufAll[n]);
                    fflush(stdout);
                    lineBuf.written = lineBuf.len = 0;
                }
            }

    } // running loop

    for (i=0; i<currmax; i++)
        close(s[i]);

    if (bridge)
        close(bridge);


    return 0;
}
