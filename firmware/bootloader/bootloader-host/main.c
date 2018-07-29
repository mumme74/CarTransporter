 /*
  * crated by Fredrik Johansson 2018-07-29
  *
  * Based on candump from VW
  */
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

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>
#include <getopt.h>
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
#include <sys/select.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include "can_protocol.h"
#include "commands.h"

// global vars
int running = 0;
int cansock = 0;
uint32_t canIdx;
struct sockaddr_can addr;

// file specific vars
static struct ifreq ifr;
static const int canfd_on = 1;

#define ANYDEV "any"  /* name of interface to receive from any CAN interface */
#define MAX_NODENR 2
#define DEBUG 1
#define MIN(x, y) (((x) < (y)) ? (x) : (y))


void setup_can_iface(char *name)
{
    cansock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (cansock < 0)
        errExit("socket");



    addr.can_family = AF_CAN;

    memset(&ifr.ifr_name, 0, sizeof(ifr.ifr_name));
    strncpy(ifr.ifr_name, name, strlen(name));

#ifdef DEBUG
    printf("using interface name '%s'.\n", ifr.ifr_name);
#endif
    // set can interface
    ifr.ifr_ifindex = (int)if_nametoindex(ifr.ifr_name);
    if (!ifr.ifr_ifindex)
        errExit("if_nametoindex");

    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    // filter out all other messages
    struct can_filter rcvfilter;
    if (canIdx < 0x800) {
        // 11 bit id
        rcvfilter.can_mask = 0x7FF; // filter in only this id
    } else {
        // 29 bit (extended frame)
        rcvfilter.can_mask = 0x7FFF;
    }
    rcvfilter.can_id = canIdx;
    setsockopt(cansock, SOL_CAN_RAW, CAN_RAW_FILTER, &rcvfilter, sizeof (struct can_filter));

    /* try to switch the socket into CAN FD mode */
    setsockopt(cansock, SOL_CAN_RAW, CAN_RAW_FD_FRAMES, &canfd_on, sizeof(canfd_on));

    if (bind(cansock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        errExit("bind");
}

bool parse_memoptions(memoptions_t *mopt, char *argoption)
{
    char *ptok;
    mopt->lowerbound = mopt->upperbound = 0;
    if ((ptok = strtok(argoption, ":")) != NULL)
        mopt->lowerbound = (uint32_t)strtol(ptok, NULL, 0);
    else
        return false;
    if ((ptok = strtok(argoption, ":")) != NULL)
        mopt->upperbound = (uint32_t)strtol(ptok, NULL, 0);
    else
        return false;

    if (mopt->lowerbound == 0 && mopt->upperbound == 0)
        return false;

    if (mopt->lowerbound > mopt->upperbound) {
        uint32_t bound = mopt->lowerbound;
        mopt->lowerbound = mopt->upperbound;
        mopt->upperbound = bound;
    }
    return true;
}

void print_usage(char *prg)
{
    fprintf(stderr, "\nUsage: %s [options] <CAN interface>+\n", prg);
    fprintf(stderr, "  (use CTRL-C to terminate %s)\n\n", prg);
    fprintf(stderr, "Options: -n <nodeNr>  nodenr [0-7] ie: 0=parkbrakeNode\n");
    fprintf(stderr, "                                       1=suspensionNode\n");
    fprintf(stderr, "                                       2=displayNode <currently inactive>\n");
    fprintf(stderr, "                                       3-7=currently not used nodes\n");
    fprintf(stderr, "         -N <node>    nodename: parkbrakeNode | suspensionNode\n");
    fprintf(stderr, "         -i <ID>      CAN frameid to listen to in HEX\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "A CAN interface must be specified\n");
    fprintf(stderr, "on the commandline in the form: <ifname>\n");
    fprintf(stderr, "\nCAN ID in hexadecimal values, or nodename  must be given.\n");
    fprintf(stderr, "\nUse interface name '%s' to receive from all CAN interfaces.\n", ANYDEV);
    fprintf(stderr, "\nExamples:\n");
    fprintf(stderr, "%s -N parkbrakeNode can0\n", prg);
    fprintf(stderr, "%s -n 0 can0\n", prg);
    fprintf(stderr, "%s -i 6a0 can0 for can serial on CANid 6a0\n", prg);
    fprintf(stderr, "\n");
}

void sigterm(int signo)
{
    (void)signo;
    running = 0;
}

int main(int argc, char *argv[])
{
    int opt;
    unsigned int canIdx = 0;
    int ret = EXIT_SUCCESS;


    signal(SIGTERM, sigterm);
    signal(SIGHUP, sigterm);
    signal(SIGINT, sigterm);

    running = 1;

    while ((opt = getopt(argc, argv, "i:n:N:h?")) != -1) {
        switch (opt) {
        case 'i': {
            long idx = strtol(optarg, NULL, 16);
            if (idx < 0 || idx > 0x7FFF) {
                print_usage(basename(argv[0]));
                exit(1);
            }
            canIdx = (unsigned int)idx;
        }   break;
        case 'n': {
            int idx = atoi(optarg);
            if (idx < 0 || idx > MAX_NODENR || !isdigit(optarg[0])) {
                print_usage(basename(argv[0]));
                exit(1);
            }
            switch (idx) {
            case 0:
                canIdx = CAN_MSG_TYPE_DIAG | C_parkbrakeDiagSerial | C_parkbrakeNode;
                break;
            case 1:
                canIdx = CAN_MSG_TYPE_DIAG | C_suspensionDiagSerial | C_suspensionNode;
                break;
            case 2:
                canIdx = CAN_MSG_TYPE_DIAG | C_displayDiag_LAST | C_displayNode;
                break;
            case 3:
                canIdx = CAN_MSG_TYPE_DIAG;
                break;
            case 4:
                canIdx = CAN_MSG_TYPE_DIAG;
                break;
            case 5:
                canIdx = CAN_MSG_TYPE_DIAG;
                break;
            case 6:
                canIdx = CAN_MSG_TYPE_DIAG;
                break;
            case 7:
                canIdx = CAN_MSG_TYPE_DIAG;
                break;
            default:
                break;
            }
        }   break;
        case 'N':
            if (strcmp(optarg, "parkbrakeNode") == 0)
                canIdx = CAN_MSG_TYPE_DIAG | C_parkbrakeDiagSerial | C_parkbrakeNode;
            else if (strcmp(optarg, "suspensionNode") == 0)
                canIdx = CAN_MSG_TYPE_DIAG | C_suspensionDiagSerial | C_suspensionNode;
            break;
        case '?':
            fprintf(stderr, "unknown option char at index:%d", optind);
            exit(1);
        case 'h':
        default:
            print_usage(basename(argv[0]));
            exit(1);
        }
    }

    // no options
    if (optind == argc || canIdx <= 0)
        errExit("Must give interface after options, ie. can0 or can1 etc\n\n");
    else {
        // parse arguments
        // first argument should be interface ie. can0
        char *cansockname = argv[optind];
        setup_can_iface(cansockname);

        // next comes the verb (what we should do)
        if (optind + 1 == argc)
            errExit("Must give a command after interface, ie read or chksum etc...");

        char *cmd = argv[optind + 1];

        // get rest of agrguments
        enum {max_arg_options = 2};
        char *argoptions[max_arg_options];

        int argoptionsc = MIN(argc - optind, max_arg_options);
        for (int i = 0; i < argoptionsc; ++i)
            argoptions[i] = argv[optind + 1 + i];


        // start determine what to do
        if (strcmp(cmd, "read") == 0) {
            // read memory from node
            // split memory regions ie 0x800000:0x8000400
            memoptions_t mopt = {0, 0};
            if (argoptionsc > 1) { // more arguments given
                if (!parse_memoptions(&mopt, argoptions[1])) {
                    fprintf(stderr, "Wrong memory regions given %s", argoptions[0]);
                    errExit(0);
                }
            } else if (argoptionsc < 1)
                errExit("Must give a filename to save to");
            doReadCmd(&mopt, argoptions[0]);

        } else if (strcmp(cmd, "chksum") == 0) {
            // get memory checksum
            // split memory regions ie 0x800000:0x8000400
            memoptions_t mopt = {0, 0};
            if (argoptionsc > 0) { // more arguments given
                if (!parse_memoptions(&mopt, argoptions[0])) {
                    fprintf(stderr, "Wrong memory regions given %s", argoptions[0]);
                    errExit(0);
                }
            }
            doChecksumCmd(&mopt);

        } else if (strcmp(cmd, "erase") == 0) {
            // split memory regions ie 0x800000:0x8000400
            memoptions_t mopt = {0, 0};
            if (argoptionsc > 0) { // more arguments given
                if (!parse_memoptions(&mopt, argoptions[0])) {
                    fprintf(stderr, "Wrong memory regions given %s", argoptions[0]);
                    errExit(0);
                }
            }
            doEraseCmd(&mopt);

        } else if (strcmp(cmd, "write") == 0) {
            // write a bin file to node
            // split memory regions ie 0x800000:0x8000400
            memoptions_t mopt = {0, 0};
            if (argoptionsc > 1) { // more arguments given
                if (!parse_memoptions(&mopt, argoptions[1])) {
                    fprintf(stderr, "Wrong memory regions given %s", argoptions[0]);
                    errExit(0);
                }
            } else if (argoptionsc < 1)
                errExit("Must give a filename to save to");
            doWriteCmd(&mopt, argoptions[0]);

        } else if (strcmp(cmd, "reset") == 0) {
            // reset device
            doResetCmd();

        } else if (strcmp(cmd, "chksumfile") == 0) {
            // get the checksum of local file
            if (argoptionsc < 1)
                errExit("Must give a binary filepath as argument");
            doChecksumLocalCmd(argoptions[0]);

        } else if (strcmp(cmd, "comparefile") == 0) {
            // compares local bin with remote file
            if (argoptionsc < 1)
                errExit("Must give a binary file path as argument");
            doCompareCmd(argoptions[0]);

        } else if (strcmp(cmd, "memoryinfo") == 0) {
            doPrintMemorySetupCmd();

        } else {
            fprintf(stderr, "Unrecognized command '%s'\n\ntype %s -h for more info", cmd, argv[0]);
            ret = EXIT_FAILURE;
        }
    }

    cleanup();

    return ret;
}
