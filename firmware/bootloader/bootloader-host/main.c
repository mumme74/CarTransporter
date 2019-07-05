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

// unix headers
#include <getopt.h>
//#include <unistd.h>
#include <libgen.h>

//#include <sys/time.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <sys/ioctl.h>
//#include <sys/uio.h>
#include <net/if.h>
//#include <sys/select.h>

//#include <linux/can.h>
#include <linux/can/raw.h>

#include "can_protocol.h"
#include "commands.h"
#include "canbridge.h"


#define ANYDEV "any"  /* name of interface to receive from any CAN interface */
#define MAX_NODENR 2
#define DEBUG 1
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

// global vars
int abortVar = 0;
uint32_t canIdx = 0;


#ifdef _WIN32
char *basename(const char* filepath) {
    static char bname[100], ext[10];
    _splitpath_s(filepath,
            NULL, 0, // drive
            NULL, 0, // dir
            bname, 88, // filename
            ext, 10);
    strcat(bname, ".");
    strcat(bname, ext);
    return bname;
}
#endif

void setup_can_iface(char *name)
{
    if (!canbridge_init(name))
        errExit(canbridge_errmsg);

    // filter out all other messages
    uint32_t mask, id;
    if (canIdx < 0x800) {
        // 11 bit id
        mask = 0x7F8; // filter in only this id
    } else {
        // 29 bit (extended frame)
        mask = 0x1FFFFFF8;
    }
    id = canIdx;
    if (!canbridge_set_filter(mask, id))
        errExit(canbridge_errmsg);

    if (!canbridge_open())
        errExit(canbridge_errmsg);
}

bool parse_memoptions(memoptions_t *mopt, char *argoption)
{
    char *ptok;
    mopt->lowerbound = mopt->upperbound = 0;
    if ((ptok = strtok(argoption, ":")) != NULL) {
        mopt->lowerbound = (uint32_t)strtol(ptok, NULL, 0);

        if ((ptok = strtok(NULL, ":")) != NULL)
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
    fprintf(stderr, "         -d <driver>  what driver to use, ie:\n");
    for(CAN_Drivers_t d = CAN_driver_invalid +1; d < _CAN_driver_end; ++d)
        fprintf(stderr, "                            %s", canbridge_get_driverNameForId(d));

    fprintf(stderr, "\n");
    fprintf(stderr, "A CAN interface must be specified on the commandline in the form:\n");
#ifdef __linux__
    fprintf(stderr, " <ifname> (driver=socketcan)\n");
#endif
    fprintf(stderr, " <serialport> (driver=slcan)\n");
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
    abortVar = 1;
}

int main(int argc, char *argv[])
{
    int opt;
    int ret = EXIT_SUCCESS;

    signal(SIGTERM, sigterm);
    signal(SIGHUP, sigterm);
    signal(SIGINT, sigterm);

    abortVar = 0;

    while ((opt = getopt(argc, argv, "i:n:N:d:h?")) != -1) {
        switch (opt) {
        case 'i': {
            long idx = strtol(optarg, NULL, 16);
            if (idx < 0 || idx > 0x7FF) {
                errExit("Wrong canID given\n");
            }
            canIdx = (unsigned int)idx;
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
            if (!canbridge_set_driverFromName(optarg)) {
                fprintf(stderr, "**%s\navaliable:", canbridge_errmsg);
                for (CAN_Drivers_t d = CAN_driver_invalid +1; d < _CAN_driver_end; ++d)
                    fprintf(stderr, " %s", canbridge_get_driverNameForId(d));
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

    // driver check
    if (canbridge_get_driverId() == CAN_driver_invalid)
        errExit("Must specify a driver to use\n\n");

    // no options
    if (optind == argc || canIdx <= 0)
        errExit("Must give interface after options, ie. can0 (driver=socketcan) or /dev/ttySerial (driver=slcan) etc.\n\n");
    else {
        // parse arguments
        // first argument should be interface ie. can0
        char *cansockname = argv[optind];
        setup_can_iface(cansockname); // FIXME implement code to select driver

        // next comes the verb (what we should do)
        if (optind + 1 == argc)
            errExit("Must give a command after interface, ie read or chksum etc...");

        char *cmd = argv[optind + 1];

        // get rest of agrguments
        enum {max_arg_options = 3};
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
                if (!parse_memoptions(&mopt, argoptions[2])) {
                    fprintf(stderr, "**Wrong memory regions given %s\n", argoptions[0]);
                    errExit(0);
                }
            } else if (argoptionsc < 2)
                errExit("Must give a filename to save to");
            doReadCmd(&mopt, argoptions[1]);

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
            if (argoptionsc > 2) { // more arguments given
                if (!parse_memoptions(&mopt, argoptions[2])) {
                    fprintf(stderr, "**Wrong memory regions given %s\n", argoptions[0]);
                    errExit(0);
                }
            } else if (argoptionsc < 2)
                errExit("Must give a filename to save to");
            doWriteCmd(&mopt, argoptions[1]);

        } else if (strcmp(cmd, "reset") == 0) {
            // reset device
            doResetCmd();

        } else if (strcmp(cmd, "checksumfile") == 0) {
            // get the checksum of local file
            if (argoptionsc < 2)
                errExit("Must give a binary filepath as argument");
            doChecksumLocalCmd(argoptions[1]);

        } else if (strcmp(cmd, "comparewithfile") == 0) {
            // compares local bin with remote file
            if (argoptionsc < 2)
                errExit("Must give a binary file path as argument");
            doCompareCmd(argoptions[1]);

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
