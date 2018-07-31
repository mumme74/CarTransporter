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
uint32_t canIdx = 0;
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
        errExit("CAN socket failed to open");



    addr.can_family = AF_CAN;

    memset(&ifr.ifr_name, 0, sizeof(ifr.ifr_name));
    strncpy(ifr.ifr_name, name, strlen(name));

    printf("Using interface name '%s'.\n", ifr.ifr_name);

    // set can interface
    ifr.ifr_ifindex = (int)if_nametoindex(ifr.ifr_name);
    if (!ifr.ifr_ifindex)
        errExit("Failed to open interface\n");

    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    // filter out all other messages
    struct can_filter rcvfilter;
    if (canIdx < 0x800) {
        // 11 bit id
        rcvfilter.can_mask = 0x7F8; // filter in only this id
    } else {
        // 29 bit (extended frame)
        rcvfilter.can_mask = 0x7FF8;
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
    fprintf(stderr, "\nUsage: %s [options] <CAN interface> <action> [arguments]\n", prg);
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
    fprintf(stderr, "CAN ID in hexadecimal values, or nodename  must be given.\n\n");
    fprintf(stderr, "Use interface name '%s' to receive from all CAN interfaces.\n", ANYDEV);
    fprintf(stderr, "\nActions:      arguments:   optional arguments:\n");
    fprintf(stderr, "   bootmode            Sets node i bootloader mode.\n\n");
    fprintf(stderr, "   read         backup.bin   <optional memory region in hex>\n");
    fprintf(stderr, "                       Reads Node flash memory to backup.bin\n");
    fprintf(stderr, "                       If no memory region given, read complete flash\n\n");
    fprintf(stderr, "   write        new.bin      <optional memory region in hex>\n");
    fprintf(stderr, "                       Flashes new.bin to memory region\n");
    fprintf(stderr, "                       If no memory region given, use standard place\n\n");
    fprintf(stderr, "                       Erases flash memory in node.\n");
    fprintf(stderr, "                       If no memory region given, erase complete memory\n\n");
    fprintf(stderr, "   compare      file.bin      <optional memory region in hex>\n");
    fprintf(stderr, "                       Compares memory in node with file.bin.\n");
    fprintf(stderr, "                       If no memory region given, compare from standard startadress\n");
    fprintf(stderr, "                       up to filesize of file.bin\n\n");
    fprintf(stderr, "   checksum                   <optional memory region in hex>\n");
    fprintf(stderr, "                       Checksum node flash memory\n");
    fprintf(stderr, "                       If no memory region given, use complete memory\n");
    fprintf(stderr, "                       Note as it then reads complete flash CRC is different\n\n");
    fprintf(stderr, "   checksumfile file.bin\n");
    fprintf(stderr, "                       Checksum local file given by argument\n\n");
    fprintf(stderr, "   memoryinfo\n");
    fprintf(stderr, "                       Prints node memory regions\n\n");
    fprintf(stderr, "   reset\n");
    fprintf(stderr, "                  T    Trigger a reset in node\n\n");
    fprintf(stderr, " explanation memoryregion:\n");
    fprintf(stderr, "   if given if should be in the <form startaddress>:<endaddress>\n");
    fprintf(stderr, "   Startaddress is where action begins in node memory\n");
    fprintf(stderr, "   Endaddress is where action ends in node memory\n");
    fprintf(stderr, "   example:  0x801800:0x80FFFF\n");
    fprintf(stderr, "   See memoryinfo for the bounds of memory regions\n\n");
    fprintf(stderr, "Examples:\n");
    fprintf(stderr, "%s -N parkbrakeNode can0 checksum\n", prg);
    fprintf(stderr, "%s -n 0 can0 read /path/to/backup.bin\n", prg);
    fprintf(stderr, "%s -n 0 can0 write /path/to/new.bin 0x8003050:0x800FFFF\n", prg);
    fprintf(stderr, "%s -i 6a0 can0 [action] <optional agument> uses canID 0x6A for communication\n", prg);
    fprintf(stderr, "        usefull when we have a new node that isn't hard coded in %s\n", prg);
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
                errExit("Wrong canID given\n");
            }
            canIdx = (unsigned int)idx;
        }   break;
        case 'n': {
            int idx = atoi(optarg);
            if (idx < 0 || idx > MAX_NODENR || !isdigit(optarg[0])) {
                errExit("Wrong nodeNr given\n");
            }
            switch (idx) {
            case 0:
                canIdx = CAN_MSG_TYPE_DIAG | C_parkbrakeDiagSerial | C_displayNode;
                break;
            case 1:
                canIdx = CAN_MSG_TYPE_DIAG | C_suspensionDiagSerial | C_displayNode;
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
                canIdx = CAN_MSG_TYPE_DIAG | C_parkbrakeDiagSerial | C_displayNode;
            else if (strcmp(optarg, "suspensionNode") == 0)
                canIdx = CAN_MSG_TYPE_DIAG | C_suspensionDiagSerial | C_displayNode;
            else {
                fprintf(stderr, "Unknown node %s\n", optarg);
                errExit(0);
            }
            break;
        case '?':
            fprintf(stderr, "unknown option char at index:%d\n", optind);
            errExit(0);
        case 'h':
        default:
            print_usage(basename(argv[0]));
            errExit(0);
        }
    }

    // no options
    if (optind == argc || canIdx <= 0)
        errExit("Must give interface after options, ie. can0 or can1 etc.\n\n");
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

        int argoptionsc = MIN(argc - optind -1, max_arg_options);
        for (int i = 0; i < argoptionsc; ++i)
            argoptions[i] = argv[optind + 1 + i];


        // start determine what to do
        if (strcmp(cmd, "read") == 0) {
            // read memory from node
            // split memory regions ie 0x800000:0x8000400
            memoptions_t mopt = {0, 0};
            if (argoptionsc > 2) { // more arguments given
                if (!parse_memoptions(&mopt, argoptions[1])) {
                    fprintf(stderr, "**Wrong memory regions given %s\n", argoptions[0]);
                    errExit(0);
                }
            } else if (argoptionsc < 1)
                errExit("Must give a filename to save to");
            doReadCmd(&mopt, argoptions[0]);

        } else if (strcmp(cmd, "checksum") == 0) {
            // get memory checksum
            // split memory regions ie 0x800000:0x8000400
            memoptions_t mopt = {0, 0};
            if (argoptionsc > 1) { // more arguments given
                if (!parse_memoptions(&mopt, argoptions[1])) {
                    fprintf(stderr, "**Wrong memory regions given %s\n", argoptions[0]);
                    errExit(0);
                }
            }
            doChecksumCmd(&mopt);

        } else if (strcmp(cmd, "erase") == 0) {
            // split memory regions ie 0x800000:0x8000400
            memoptions_t mopt = {0, 0};
            if (argoptionsc > 1) { // more arguments given
                if (!parse_memoptions(&mopt, argoptions[1])) {
                    fprintf(stderr, "**Wrong memory regions given %s\n", argoptions[0]);
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
                    fprintf(stderr, "**Wrong memory regions given %s\n", argoptions[0]);
                    errExit(0);
                }
            } else if (argoptionsc < 1)
                errExit("Must give a filename to save to");
            doWriteCmd(&mopt, argoptions[0]);

        } else if (strcmp(cmd, "reset") == 0) {
            // reset device
            doResetCmd();

        } else if (strcmp(cmd, "checksumfile") == 0) {
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

        } else if (strcmp(cmd, "bootmode") == 0) {
            // reset node and trigger a cmd set hang it in bootloader mode
            doBootloaderModeCmd();

        } else {
            fprintf(stderr, "Unrecognized command '%s'\n\ntype %s -h for more info", cmd, argv[0]);
            ret = EXIT_FAILURE;
        }
    }

    cleanup();

    return ret;
}
