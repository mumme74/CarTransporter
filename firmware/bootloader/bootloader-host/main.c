 /*
  * crated by Fredrik Johansson 2018-07-29
  *
  * Based on candump from VW
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

#ifdef _WIN32
# include "win_getopt.h"
#else
// unix headers
# include <getopt.h>
# include <libgen.h>
#endif

// project headers
#include "can_protocol.h"
#include "commands.h"
#include "canbridge.h"

#define MAX_NODENR 2
#define DEBUG 1
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

// global vars
int abortVar = 0;
uint32_t canIdx = 0;
static uint32_t canFilterMask = 0;
static int extendedCanId = 0;

static CAN_Speeds_t speed = CAN_speed_socketspeed;

typedef enum {
    _CMDs_start_marker,
    CMD_bootmode,
    CMD_read,
    CMD_write,
    CMD_erase,
    CMD_comparewithfile,
    CMD_checksum,
    CMD_checksumfile,
    CMD_memoryinfo,
    CMD_reset,
    _CMDs_end_marker,
} CMDs_t;
#define CMDS_NAME_MAX_SZ 20
static const char CMDs_name[_CMDs_end_marker][CMDS_NAME_MAX_SZ] = {
    "invalid",
    "bootmode",
    "read",
    "write",
    "erase",
    "comparewithfile",
    "checksum",
    "checksumfile",
    "memoryinfo",
    "reset"
};
CMDs_t cmd_from_str(const char *cmd) {
    for (CMDs_t i = _CMDs_start_marker +1; i < _CMDs_end_marker; ++i) {
        if (strncmp(cmd, CMDs_name[i], CMDS_NAME_MAX_SZ) == 0)
            return i;
    }

    return _CMDs_start_marker;
}


#ifdef _WIN32
# include "win_getopt.h"

char *basename(const char* filepath) {
    enum { BNAME_SZ = 100, EXT_SZ = 10 };
    static char bname[BNAME_SZ], ext[EXT_SZ];
    _splitpath_s(filepath,
            NULL, 0, // drive
            NULL, 0, // dir
            bname, 88, // filename
            ext, 10);
    strcat_s(bname, BNAME_SZ, ".");
    strcat_s(bname, BNAME_SZ, ext);
    return bname;
}
#endif

void setup_can_iface(char *name)
{
    if (!canbridge_init(name, speed))
        errExit(canbridge_errmsg);

    // filter out all other messages
    uint32_t mask;
    if (canFilterMask) {
        mask = canFilterMask;
    } else if (canIdx < 0x800) {
        // 11 bit id
        mask = 0x7F8; // filter in only this id
    } else {
        // 29 bit (extended frame)
        mask = 0x1FFFFFF8;
    }
    if (!canbridge_set_filter(mask, canIdx, extendedCanId))
        errExit(canbridge_errmsg);

    canbridge_set_abortvariable(&abortVar);

    if (!canbridge_open())
        errExit(canbridge_errmsg);
}

bool parse_memoptions(memoptions_t *mopt, char *argoption)
{
    char *ptok;
    mopt->lowerbound = mopt->upperbound = 0;
#ifdef _WIN32
    char *ctx = NULL;
    if ((ptok = strtok_s(argoption, ":", &ctx)) != NULL) {
        mopt->lowerbound = (uint32_t)strtol(ptok, NULL, 0);
        if ((ptok = strtok_s(NULL, ":", &ctx)) != NULL)
#else
    if ((ptok = strtok(argoption, ":")) != NULL) {
        mopt->lowerbound = (uint32_t)strtol(ptok, NULL, 0);
        if ((ptok = strtok(NULL, ":")) != NULL)
#endif
            mopt->upperbound = (uint32_t)strtol(ptok, NULL, 0);
        else
            return false;
    } else
        return false;

    if (mopt->lowerbound == 0 || mopt->upperbound == 0)
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
    fprintf(stderr, "         -m <mask>    Use mask  as filterMask, must be hex value\n");
    fprintf(stderr, "                        default filterMask is to match against canID, must be hex value\n");
    fprintf(stderr, "                       0 turns off filterMask\n");
    fprintf(stderr, "         -b <baudID>  Use baudrate, some drivers need this\n");
    fprintf(stderr, "                        baudID can be:\n");
    for (CAN_Speeds_t s = _CAN_speed_start_marker +1; s < _CAN_speed_end_marker -1; ++s)
        fprintf(stderr, "                           %s\n", canbridge_get_speed_name_from_id(s));
    fprintf(stderr, "         -d <driver>  what driver to use, ie:\n");
    for (CAN_Drivers_t d = _CAN_driver_start_marker +1; d < _CAN_driver_end_marker; ++d)
        fprintf(stderr, "                            %s\n", canbridge_get_driver_name_for_id(d));

    fprintf(stderr, "\n");
    fprintf(stderr, "A CAN interface must be specified on the commandline in the form:\n");
#ifdef BUILD_SOCKETCAN
    fprintf(stderr, " <ifname> (driver=socketcan)\n");
#endif
    fprintf(stderr, " <serialport> (driver=slcan)\n");
    fprintf(stderr, "CAN ID in hexadecimal values, or nodename  must be given.\n\n");
    //fprintf(stderr, "Use interface name '%s' to receive from all CAN interfaces.\n", ANYDEV);
    fprintf(stderr, "\nActions:      arguments:   optional arguments:\n");
    fprintf(stderr, "   bootmode            Sets node i bootloader mode.\n\n");
    fprintf(stderr, "   read         backup.bin   <optional memory region in hex>\n");
    fprintf(stderr, "                       Reads Node flash memory to backup.bin\n");
    fprintf(stderr, "                       If no memory region given, read complete flash\n\n");
    fprintf(stderr, "   write        new.bin      <optional memory region in hex>\n");
    fprintf(stderr, "                       Flashes new.bin to memory region\n");
    fprintf(stderr, "                       If no memory region given, use standard place\n\n");
    fprintf(stderr, "   erase       <optional memory region in hex>\n");
    fprintf(stderr, "                       Erases flash memory in node.\n");
    fprintf(stderr, "                       If no memory region given, erase complete memory\n\n");
    fprintf(stderr, "   comparewithfile\n");
    fprintf(stderr, "               file.bin      <optional memory region in hex>\n");
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
    fprintf(stderr, "                       Trigger a reset in node\n\n");
    fprintf(stderr, " explanation memoryregion:\n");
    fprintf(stderr, "   if given if should be in the <form startaddress>:<endaddress>\n");
    fprintf(stderr, "   Startaddress is where action begins in node memory\n");
    fprintf(stderr, "   Endaddress is where action ends in node memory\n");
    fprintf(stderr, "   example:  0x801800:0x80FFFF\n");
    fprintf(stderr, "   See memoryinfo for the bounds of memory regions\n\n");
    fprintf(stderr, "Examples:\n");
    fprintf(stderr, "%s -s socketcan -N parkbrakeNode can0 checksum\n", prg);
    fprintf(stderr, "%s -s slcan -n 0 can0 read /path/to/backup.bin\n", prg);
    fprintf(stderr, "%s -s slcan -n 0 can0 write /path/to/new.bin 0x8003050:0x800FFFF\n", prg);
    fprintf(stderr, "%s -s socketcan -i 6a0 -m FF8 can0 [action] <optional agument> uses canID 0x6A for communication\n", prg);
    fprintf(stderr, "        matches canID 6a0 to 6a7\n");
    fprintf(stderr, "        usefull when we have a new node that isn't hard coded in %s\n", prg);
    fprintf(stderr, "\n");
}

void sigterm(int signo)
{
    (void)signo;
    abortVar = 1;
}

int main(int argc, char *argv[])
{
    int opt;
    int ret = EXIT_SUCCESS;

    signal(SIGTERM, sigterm);
    signal(SIGINT, sigterm);
#ifndef _WIN32
    signal(SIGHUP, sigterm);
#endif

    abortVar = 0;

    while ((opt = getopt(argc, argv, "i:m:n:N:d:b:h?")) != -1) {
        switch (opt) {
        case 'i': {
            char *nxt = NULL;
            long idx = strtol(optarg, &nxt, 16);
            if (nxt != &optarg[strnlen(optarg, 20)])
                errExit("Invalid canID\n");
            if (idx < 0 || idx > 0x7FFFFF)
                errExit("Wrong canID given\n");

            canIdx = (unsigned int)idx;
            if (!extendedCanId && strnlen(optarg, 10) > 4)
                extendedCanId = 1;
        }   break;
        case  'm': {
            char *nxt = NULL;
            long filter = strtol(optarg, &nxt, 16);
            if (nxt != &optarg[strnlen(optarg, 20)])
                errExit("Invalid can filter mask\n");
            if (filter < 0 || filter > 0x7FFFFFF)
                errExit("Wrong filter given\n");
            canFilterMask = (uint32_t)filter;
            if (!extendedCanId && strnlen(optarg, 10) > 4)
                extendedCanId = 1;
        }   break;
        case 'n': {
            int idx = atoi(optarg);
            if (idx < 0 || idx > MAX_NODENR || !isdigit(optarg[0])) {
                errExit("Wrong nodeNr given. Only 11bits is ok\n");
            }
            switch (idx) {
            case 0:
                canIdx = CAN_MSG_TYPE_COMMAND | C_parkbrakeCmdBootloader | C_displayNode;
                break;
            case 1:
                canIdx = CAN_MSG_TYPE_COMMAND | C_suspensionCmdBootloader | C_displayNode;
                break;
            case 2:
                canIdx = CAN_MSG_TYPE_COMMAND; // | C_displayDiag_LAST | C_displayNode;
                break;
            case 3:
                canIdx = CAN_MSG_TYPE_COMMAND;
                break;
            case 4:
                canIdx = CAN_MSG_TYPE_COMMAND;
                break;
            case 5:
                canIdx = CAN_MSG_TYPE_COMMAND;
                break;
            case 6:
                canIdx = CAN_MSG_TYPE_COMMAND;
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
                canIdx = CAN_MSG_TYPE_COMMAND | C_parkbrakeCmdBootloader | C_displayNode;
            else if (strcmp(optarg, "suspensionNode") == 0)
                canIdx = CAN_MSG_TYPE_COMMAND | C_suspensionCmdBootloader | C_displayNode;
            else {
                fprintf(stderr, "Unknown node %s\n", optarg);
                errExit(0);
            }
            break;
        case 'd':
            if (!canbridge_set_driver_from_name(optarg)) {
                fprintf(stderr, "**%s\n avaliable:", canbridge_errmsg);
                for (CAN_Drivers_t d = _CAN_driver_start_marker +1; d < _CAN_driver_end_marker; ++d)
                    fprintf(stderr, " %s", canbridge_get_driver_name_for_id(d));
                fprintf(stderr, "\n\n");
                errExit(0);
            }
            break;
        case 'b':
            speed = canbridge_get_speed_id_from_speed_name(optarg);
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

    // driver check
    if (canbridge_get_driver_id() == _CAN_driver_start_marker)
        errExit("Must specify a driver to use\n\n");

    if (speed <= _CAN_speed_start_marker || speed >= _CAN_speed_end_marker) {
        fprintf(stderr, "**Invalid speed given\n");
        for (CAN_Speeds_t i = _CAN_speed_start_marker +1; i < _CAN_speed_end_marker; ++i)
            fprintf(stderr, "  %s\n", canbridge_get_speed_name_from_id(i));
        errExit(0);
    }

    // no options
    if (optind == argc) {
        fprintf(stderr, "Must give interface after options, ie.");
#ifdef BUILD_SOCKETCAN
        fprintf(stderr, " can0 (driver=socketcan) or");
#endif
        fprintf(stderr, " /dev/ttySerial (driver=slcan) etc.\n\n");
        errExit(0);
    } else if (canIdx <= 0) {
        fprintf(stderr, "Node not selected, see: %s -h\n\n", basename(argv[0]));
        errExit(0);
    } else {
        // parse arguments
        // first argument should be interface ie. can0
        char *cansockname = argv[optind];

        // next comes the verb (what we should do)
        if (optind + 1 == argc) {
            fprintf(stderr, "Must give a command after interface, ie read or chksum etc...\navailable:\n");
            for(CMDs_t i = _CMDs_start_marker +1; i < _CMDs_end_marker; ++i)
                fprintf(stderr, "  %s\n", CMDs_name[i]);
            errExit(0);
        }

        setup_can_iface(cansockname);
        char *cmd = argv[optind + 1];

        // get rest of agrguments
        enum {max_arg_options = 3};
        char *argoptions[max_arg_options];

        int argoptionsc = MIN(argc - optind -1, max_arg_options);
        for (int i = 0; i < argoptionsc; ++i)
            argoptions[i] = argv[optind + 1 + i];

        // start determine what to do
        switch(cmd_from_str(cmd)) {
        case CMD_read: {
            // read memory from node
            // split memory regions ie 0x800000:0x8000400
            memoptions_t mopt = {0, 0};
            if (argoptionsc > 2) { // more arguments given
                if (!parse_memoptions(&mopt, argoptions[2])) {
                    fprintf(stderr, "**Wrong memory regions given %s\n", argoptions[0]);
                    errExit(0);
                }
            } else if (argoptionsc < 2)
                errExit("Must give a filename to save to\n");
            doReadCmd(&mopt, argoptions[1]);

        } break;
        case CMD_checksum: {
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

        } break;
        case CMD_erase: {
            // split memory regions ie 0x800000:0x8000400
            memoptions_t mopt = {0, 0};
            if (argoptionsc > 1) { // more arguments given
                if (!parse_memoptions(&mopt, argoptions[1])) {
                    fprintf(stderr, "**Wrong memory regions given %s\n", argoptions[0]);
                    errExit(0);
                }
            }
            doEraseCmd(&mopt);

        } break;
        case CMD_write: {
            // write a bin file to node
            // split memory regions ie 0x800000:0x8000400
            memoptions_t mopt = {0, 0};
            if (argoptionsc > 2) { // more arguments given
                if (!parse_memoptions(&mopt, argoptions[2])) {
                    fprintf(stderr, "**Wrong memory regions given %s\n", argoptions[0]);
                    errExit(0);
                }
            } else if (argoptionsc < 2)
                errExit("Must give a filename to save to");
            doWriteCmd(&mopt, argoptions[1]);

        } break;
        case CMD_reset:
            // reset device
            doResetCmd();
            break;
        case CMD_checksumfile:
            // get the checksum of local file
            if (argoptionsc < 2)
                errExit("Must give a binary filepath as argument");
            doChecksumLocalCmd(argoptions[1]);
            break;
        case CMD_comparewithfile:
            // compares local bin with remote file
            if (argoptionsc < 2)
                errExit("Must give a binary file path as argument");
            doCompareCmd(argoptions[1]);
            break;
        case CMD_memoryinfo:
            doPrintMemorySetupCmd();
             break;
        case CMD_bootmode:
            // reset node and trigger a cmd set hang it in bootloader mode
            doBootloaderModeCmd();
            break;
        default:
            fprintf(stderr, "Unrecognized command '%s'\navailable:\n", cmd);
            for(CMDs_t i = _CMDs_start_marker +1; i < _CMDs_end_marker; ++i)
                fprintf(stderr, "  %s\n", CMDs_name[i]);
            fprintf(stderr, " \ntype %s -h for more info\n", basename(argv[0]));
            ret = EXIT_FAILURE;
        }
    }

    if (canbridge_errmsg[0] != 0)
        fprintf(stdout, "\n %s\n", canbridge_errmsg);
    cleanup();

    return ret;
}
