/*
 * commands.c
 *
 *  Created on: 29 jul 2018
 *      Author: jof
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/uio.h>
#include <net/if.h>
#include <errno.h>

#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/can/bcm.h>

#include "crc32.h"
#include "can_protocol.h"
#include "commands.h"

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

typedef struct can_frame canframe_t;

// memory structure of uC
typedef struct {
    uint32_t bootRomStart;
    uint32_t bootRomEnd;
    uint16_t pageSize;
    uint16_t nrPages;
} memoryinfo_t;

extern int cansock;

static memoryinfo_t memory = { 0 ,0 ,0 ,0 };
static uint32_t nodeCrc,
                binCrc;

static FILE *binFile = NULL;



static char unknownErrbuf[50];

static char *bootloadErrToStr(can_bootloaderErrs_e err)
{
    switch (err) {
    case C_bootloaderErr:                       return "BootloaderErr";
    case C_bootloaderErrResend:                 return "BootloaderErrResend";
    case C_bootloaderErrStartAddressOutOfRange:  return "BootloaderErrStartaddressOutOfRange";
    case C_bootloaderErrEndAddressOutOfRange:    return "BootloaderErrEndaddressOutOfRange";
    case C_bootloaderErrStartPageOutOfRange:    return "BootloaderErrStartPageOutOfRange";
    case C_bootloaderErrPageLenOutOfRange:      return "BootloaderErrPageLenOutOfRange";
    case C_bootloaderErrPageWriteFailed:        return "BootloaderErrWriteFailed";
    case C_bootloaderErrPageEraseFailed:        return "BootloaderErrPageEraseFailed";
    case C_bootloaderErrCanPageOutOfOrder:      return "BootloaderErrCanPageOutOfOrder";
    case C_bootloaderErrOK:                     return "BootloaderErrOK";
    case C_bootloaderErrNoResponse:             return "BootloaderErrNoResponse";
    case C_bootloaderErrSendFailed:             return "BootloaderErrSendFailed";
    default: ;
    }

    // we handle all other including ErrUnknown this way
    sprintf(unknownErrbuf, "Unknown error:%x", err);
    return unknownErrbuf;
}

static void nodeErrExit(const char *str, can_bootloaderErrs_e err) __attribute__((noreturn));
static void nodeErrExit(const char *str, can_bootloaderErrs_e err)
{
    char *errStr = bootloadErrToStr(err);
    char *chbuf = malloc(strlen(str) + strlen(errStr) + 2);
    sprintf(chbuf, "%s %s", str, errStr);
    fprintf(stdout, "%s", chbuf);
    free(chbuf);
    cleanup();
    exit(EXIT_FAILURE);
}

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

    fprintf(stdout, "wait in progress tv={%ld,%ld} ...\n",
            tv.tv_sec, tv.tv_usec);

    if (r) rfds = &fdset; else rfds = NULL;
    if (w) wfds = &fdset; else wfds = NULL;

    TEMP_FAIL_RETRY (n = select (sock+1, rfds, wfds, NULL, &tv));
    switch (n) {
    case 0:
        fprintf(stderr, "**wait timed out\n");
        return -errno;
    case -1:
        fprintf(stderr, "**error during wait\n");
        return -errno;
    default:
        // select tell us that sock is ready, test it
        so_len = sizeof(so_error);
        so_error = 0;
        getsockopt (sock, SOL_SOCKET, SO_ERROR, &so_error, &so_len);
        if (so_error == 0)
            return 0;
        errno = so_error;
        fprintf(stderr, "**wait failed\n");
        return -errno;
    }
}

static bool cansend(canframe_t *msg, int timeoutms)
{
    if (write(cansock, &msg, 0) != 0) {
        perror("write");
        return false;
    }
    return wait_on_sock(cansock, timeoutms, 0, 1) == 0;
}

static bool canrecv(canframe_t *frm, int timeoutms)
{
    fd_set rdfs;
    struct timeval tv;

    /* Watch stdin (fd 0) to see when it has input. */
    FD_ZERO(&rdfs);
    FD_SET(0, &rdfs);

    /* Wait up to five seconds. */
    tv.tv_sec = timeoutms / 1000;
    tv.tv_usec = (timeoutms - tv.tv_sec) * 1000;

    int ret = select(cansock, &rdfs, NULL, NULL, &tv);
    /* Don't rely on the value of tv now! */

    if (ret == -1)
        errExit("Failed to connect to CAN while send");
    else if (ret) {
        int nbytes;
        // Data is available now.
        struct {
            struct bcm_msg_head msg_head;
            struct can_frame frame;
        } bmsg;

        if ((nbytes = read(cansock, &bmsg, sizeof(bmsg))) < 0)
            errExit("bcm read");

        if (bmsg.msg_head.opcode != RX_CHANGED) {
            fprintf(stderr, "**Received strange BCM opcode %d!\n", bmsg.msg_head.opcode);
            return false; /* quit */
        }

        if (nbytes != sizeof(bmsg)) {
            fprintf(stderr, "**Received strange BCM data length %d!\n", nbytes);
            return false; /* quit */
        }

        memcpy(frm, &bmsg.frame, sizeof(canframe_t));

    /* FD_ISSET(0, &rfds) will be true. */
    }
    return false;
}

// recives a specific frame with a specific data at pos
static bool filteredRecv(canframe_t *recvFrm, int timeout,
                  uint8_t dataPos, uint8_t isvlu)
{
    // get result
    int res = 0;
    // we might get another queued frame, discard that...
    while ((res = canrecv(recvFrm, timeout)))  {
        if ((recvFrm->can_dlc > 0) &&
            (recvFrm->data[dataPos] == isvlu))
        {
            break;
        }
        if (!running)
            cleanExit();
    }
    return res;
}

static void initFrame(canframe_t *frm)
{
    memset(frm, 0, sizeof(canframe_t));
    frm->can_id = canIdx;
}

static void initMemoryInfo(canframe_t *sendFrm, canframe_t *recvFrm)
{
    initFrame(sendFrm);
    initFrame(recvFrm);

    // send request
    sendFrm->can_dlc = 1;
    sendFrm->data[0] = C_bootloaderStartAddress;
    if (!cansend(sendFrm, 1000))
        errExit("cant send");

    // get result
    if (!filteredRecv(recvFrm, 1000, 0, C_bootloaderStartAddress))
        errExit("No response from node");

    memory.bootRomStart = (recvFrm->data[2] >> 8 | recvFrm->data[1]);

    // get the rest
    sendFrm->can_dlc = 1;
    sendFrm->data[0] = C_bootloaderMemPageInfo;
    if (!cansend((sendFrm), 1000))
        errExit("cant send");


    if (!filteredRecv(recvFrm, 1000, 0, C_bootloaderMemPageInfo))
        errExit("No response from node");

    memory.pageSize = recvFrm->data[2] >> 8 | recvFrm->data[1];
    memory.nrPages  = recvFrm->data[4] >> 8 | recvFrm->data[3];

    memory.bootRomEnd = memory.bootRomStart +
                            (memory.pageSize * memory.nrPages);
}

static can_bootloaderErrs_e resetNode(canframe_t *sendFrm, canframe_t *recvFrm)
{
    sendFrm->can_dlc = 1;
    sendFrm->data[0] = C_bootloaderReset;
    if (cansend(sendFrm, 1000))
        return C_bootloaderErrSendFailed;

    if (!filteredRecv(recvFrm, 2000, 0, C_bootloaderWait))
        return C_bootloaderErrNoResponse;

    return C_bootloaderErrOK;
}

static can_bootloaderErrs_e getRemoteChecksum(memoptions_t *mopt,
                                             canframe_t *sendFrm,
                                             canframe_t *recvFrm)
{
    sendFrm->can_dlc = 8;
    sendFrm->data[sendFrm->can_dlc++] = C_bootloaderChecksum;
    // start address
    sendFrm->data[sendFrm->can_dlc++] = (mopt->lowerbound & 0x000000FF);
    sendFrm->data[sendFrm->can_dlc++] = (mopt->lowerbound & 0x0000FF00) >> 8;
    sendFrm->data[sendFrm->can_dlc++] = (mopt->lowerbound & 0x00FF0000) >> 16;
    sendFrm->data[sendFrm->can_dlc++] = (mopt->lowerbound & 0xFF000000) >> 24;
    // length in bytes
    uint32_t len = mopt->upperbound - mopt->lowerbound;
    sendFrm->data[sendFrm->can_dlc++] = (len & 0x000000FF) >> 0;
    sendFrm->data[sendFrm->can_dlc++] = (len & 0x0000FF00) >> 8;
    sendFrm->data[sendFrm->can_dlc++] = (len & 0x00FF0000) >> 16;
    cansend(sendFrm, 100);

    if (!filteredRecv(recvFrm, 100, 0, C_bootloaderChecksum))
        return C_bootloaderErrNoResponse;

    nodeCrc = (recvFrm->data[1] & 0x000000FF) |
              (recvFrm->data[1] & 0x0000FF00) >> 8 |
              (recvFrm->data[1] & 0x00FF0000) >> 16 |
              (recvFrm->data[1] & 0xFF000000) >> 24;

    return C_bootloaderErrOK;
}

static void checkMemOptWithinBounds(memoptions_t *mopt)
{
    if (memory.bootRomEnd == 0) {
        canframe_t sendFrm, recvFrm;
        initFrame(&sendFrm);
        initFrame(&recvFrm);

        // get memory setup from Node uC
        initMemoryInfo(&sendFrm, &recvFrm);
    }

    // if memory bounds were given
    if (mopt->lowerbound == 0)
        mopt->lowerbound = memory.bootRomStart;
    else if (mopt->lowerbound < memory.bootRomStart)
        errExit("Address lower bound to low\n");

    if (mopt->upperbound == 0)
        mopt->upperbound = memory.bootRomEnd;
    else if (mopt->upperbound > memory.bootRomEnd)
        errExit(("Address upper bound to high\n"));
}

// reads a binfile into memory
// it doesnt return a empty pointer it just exits the program
// NOTE caller must free the returned memorybuffer
static uint8_t *readLocalFile(const char *binName, long *sz)
{

    binFile = fopen(binName, "r");
    if (binName == NULL) {
        fprintf(stderr, "**Couldn't open binfile '%s'", binName);
        errExit(0);
    }

    // get size
    fseek(binFile, 0L, SEEK_END);
    *sz = ftell(binFile);
    rewind(binFile);

    if (*sz < 0)
        errExit("Error when determining filesize");

    if (*sz > 3072000) {
        fclose(binFile);
        binFile = NULL;
        errExit("Binfile over 3Mb, aborting....");
     }

    // create buffer
    uint8_t *fileBuf = (uint8_t*)malloc(sizeof(uint8_t) * (size_t)*sz);
    if (fileBuf == NULL)
        errExit("Couldnt read file into RAM memory. any space left?");

    // read file into ram
    uint8_t *fileIt = fileBuf;
    int ch;
    while ((ch = fgetc(binFile)) != EOF) {
        *fileIt = (uint8_t)ch;
        ++fileIt;
    }

    fclose(binFile);
    binFile = NULL;

    return fileBuf; // NOTE! caller must free this memory
}

static can_bootloaderErrs_e writeFileToNode(memoptions_t *mopt, uint8_t *fileCache,
                                            canframe_t *sendFrm, canframe_t *recvFrm)
{
    uint8_t frameNr = 0,
            frames = 0,
            *addr = fileCache,
            *endAddr = fileCache + (mopt->upperbound - mopt->lowerbound);

//    if (!getAndCheckAddress(msg, addr, endAddr))
//      break;
    uint16_t canPageNr = 0;
    uint32_t crc;

writePageLoop:
    // begin header
    // on top of the 889 bytes restriction in CAN pages, BOOTLOADER_PAGE_SIZE,
    // it must also send in chunks divisible by mempage.pageSize (ie 2048)
    frameNr = 0;
    frames = MIN(endAddr - addr, BOOTLOADER_PAGE_SIZE) / 7 +
             MIN(endAddr - addr, BOOTLOADER_PAGE_SIZE) % 7;
    crc = crc32(0, addr, MIN(endAddr - addr, BOOTLOADER_PAGE_SIZE));
    sendFrm->can_dlc = 8;
    sendFrm->data[0] = C_bootloaderReadFlash;
    sendFrm->data[1] = (crc & 0x000000FF);
    sendFrm->data[2] = (crc & 0x0000FF00) >> 8;
    sendFrm->data[3] = (crc & 0x00FF0000) >> 16;
    sendFrm->data[4] = (crc & 0xFF000000) >> 24;
    // len of page
    sendFrm->data[5] = frames;
    sendFrm->data[6] = (canPageNr & 0x0000FF00) >> 8;
    sendFrm->data[7] = (canPageNr & 0x00FF0000) >> 16;
    if (!cansend(sendFrm, 1000))
        return C_bootloaderErrSendFailed;
    ++canPageNr;

    // begin payload frames
    while (frameNr < frames) {
      sendFrm->data[0] = frameNr++;
      for (uint8_t i = 1; i < 8; ++i) {
        sendFrm->data[i] = *addr;
        if (addr++ >= endAddr)
          break; // frameId should now be frameId == frames
      }
      if (!cansend(sendFrm, 1000))
          return C_bootloaderErrSendFailed;;
    }

    // wait for remote to Ack this page
    do {
        if (!cansend(sendFrm, 1000))
            return C_bootloaderErrSendFailed;
        if (!running)
            return C_bootloaderErrUnknown;
    } while(recvFrm->can_dlc != 2 &&
            recvFrm->data[0] != C_bootloaderReadFlash);

    // check if we should resend
    if (recvFrm->data[1] == 0xAA)
      ++canPageNr;
    if (addr >= endAddr)
      return C_bootloaderErrOK;; // finished!

    goto writePageLoop;
}


// handles reception of binfile, communicates with node, fills file in filecache
static can_bootloaderErrs_e recvFileFromNode(uint8_t *fileCache, canframe_t *sendFrm,
                                            canframe_t *recvFrm, uint32_t startAddr,
                                            uint32_t endAddr)
{
    uint16_t canPageNr;
    uint8_t frames, frameNr; //, *addr, *endAddr;
    uint32_t crc = 0;

    uint32_t memPages = ((endAddr - startAddr) / memory.pageSize) +
                        ((endAddr - startAddr) % memory.pageSize);

    uint16_t memPagesWritten = 0;


readMemPageLoop:
    canPageNr = 0;

readCanPageLoop:
    frameNr = frames = 0;

    // loop to receive a complete canPage
    do {
      if (!canrecv(recvFrm, 1000))
          return C_bootloaderErrNoResponse;

      if ((recvFrm->data[0] & 0x80) && frames == 0) {
        // get header
        crc = (recvFrm->data[4] >> 24 | recvFrm->data[3] >> 16 |
               recvFrm->data[2] >> 8  | recvFrm->data[1]);
        frames = recvFrm->data[5];
        if (canPageNr != (recvFrm->data[7] << 8 | recvFrm->data[6])) {
          return C_bootloaderErrCanPageOutOfOrder;
        }
      } else {
        // it's a payload frame
        for (uint8_t i = 1; i < 8; ++i) {
          if (recvFrm->can_dlc == i)
            break; // frameNr == frames should be true now
          fileCache[(memPagesWritten * memory.pageSize)
                    + (canPageNr * 0x7F)
                    + (recvFrm->data[0] & 0x7F) + i] = recvFrm->data[i];
        }
      }

      ++frameNr;
    } while (frameNr < frames);

    // check canPage
    // pointer buf[0] advance to start of canPage.
    // last frame might be less than 7 bytes
    if (!crc32(0, fileCache + (memPagesWritten * memory.pageSize)
                  + (canPageNr * 0x7F), (frames * 7)
                  - (7 + recvFrm->can_dlc)))
    {
      // notify node that we need this canPage retransmitted
      sendFrm->can_dlc = 2;
      sendFrm->data[0] = C_bootloaderReadFlash;
      sendFrm->data[1] = C_bootloaderErrResend;
      if (cansend(sendFrm, 1000))
          return C_bootloaderErrSendFailed;

      goto readCanPageLoop;
    }

    ++canPageNr;

    // if we haven't received a complete pageSize yet
    // and we are not at end of bin
    // (last memPage sent from invoker might be less than pageSize)
    if ((canPageNr + frames) < memory.pageSize &&
        (memPagesWritten + 1) < memPages)
    {
      sendFrm->can_dlc = 2;
      sendFrm->data[0] = C_bootloaderReadFlash;
      sendFrm->data[1] = C_bootloaderErrOK;
      if (cansend(sendFrm, 1000))
          return C_bootloaderErrSendFailed;
      goto readCanPageLoop;
    }

    // here we are at the last canPage of this memPage
    // a *.bin might span over many memPages
    if (++memPagesWritten < memPages)
      goto readMemPageLoop;

    return C_bootloaderErrOK;
}

// --------------------------------------------------------------------
// public functions and variables

void cleanup(void)
{
    close(cansock);
    fclose(binFile);
}

void cleanExit(void)
{
    cleanup();
    exit(EXIT_SUCCESS);
}


void errExit(char *errStr)
{
    char *warnChrs = "";
    if (errStr != 0)
        warnChrs = "**";
    fprintf(stderr, "%s%s", warnChrs, errStr);
    cleanup();
    exit(EXIT_FAILURE);
}

// reads parts or all of uC flash memory
void doReadCmd(memoptions_t *mopt, char *storeName)
{
    uint8_t *fileCache = NULL;
    char *fileName = NULL;
    canframe_t sendFrm, recvFrm;
    initFrame(&sendFrm);
    initFrame(&recvFrm);

    checkMemOptWithinBounds(mopt);

    fileName = (char*)0xFFFFFFFF; // point to nonsence
    // should we append .bin?
    if (strlen(storeName) < 4)
        fileName = malloc((strlen(storeName) + 4) * sizeof(char));
    else if (strcmp(storeName + strlen(storeName) -4, ".bin") != 0)
        fileName = malloc((strlen(storeName) + 4) * sizeof(char));

    if (fileName == NULL)
        errExit("Couldnt alllocate ram, space left?");

    // we already hava .bin on the end of file
    if (fileName == (char*)0xFFFFFFFF)
        fileName = storeName;

    fprintf(stdout, "Using filename '%s'\n", fileName);
    bool errOccured = false; // used to notify cleanup label that we should exit

    binFile = fopen(fileName, "w"); // implicitly overwrite by design
    if (binFile == NULL) {
        fprintf(stderr, "**Couldn't open '%s' for write operations\n", fileName);
        errOccured = true;
        goto readCleanup;
    }

    // now that we have successfully opened the file we retrive memory from node
    sendFrm.can_dlc = 0;
    sendFrm.data[sendFrm.can_dlc++] = C_bootloaderReadFlash;
    // startaddress
    sendFrm.data[sendFrm.can_dlc++] = (mopt->lowerbound & 0x000000FF) >> 0;
    sendFrm.data[sendFrm.can_dlc++] = (mopt->lowerbound & 0x0000FF00) >> 8;
    sendFrm.data[sendFrm.can_dlc++] = (mopt->lowerbound & 0x00FF0000) >> 16;
    sendFrm.data[sendFrm.can_dlc++] = (mopt->lowerbound & 0xFF000000) >> 24;

    // length
    uint32_t len = (mopt->upperbound - mopt->lowerbound);
    sendFrm.data[sendFrm.can_dlc++] = (len & 0x000000FF) >> 0;
    sendFrm.data[sendFrm.can_dlc++] = (len & 0x0000FF00) >> 8;
    sendFrm.data[sendFrm.can_dlc++] = (len & 0x00FF0000) >> 16;

    if (!cansend(&sendFrm, 100))
        errExit("Failed to send flash command");

    if (!filteredRecv(&recvFrm, 100, 0, C_bootloaderReadFlash))
        errExit("No ack for CAN read\n");

    if (recvFrm.data[0] != C_bootloaderErrOK)
        nodeErrExit("Node gives error when init read mode:", recvFrm.data[1]);

    // allocate a filecache for received bytes
    fileCache = (uint8_t*)malloc(len * sizeof(uint8_t));
    if (fileCache == NULL) {
        fprintf(stderr, "**Failed to allocate memory in RAM for filereception\n");
        errOccured = true;
        goto readCleanup;
    }

    // initiate reception sequence
    can_bootloaderErrs_e err = recvFileFromNode(fileCache, &sendFrm, &recvFrm,
                                                mopt->lowerbound, mopt->upperbound);

    if (err == C_bootloaderErrOK) {
        // successfull
        if (fwrite(fileCache, sizeof(uint8_t), len, binFile) != len) {
            fprintf(stderr, "**Error when saving file to disk\n");
            errOccured = true;
            goto readCleanup;
        }

        fprintf(stdout, "Successfully written to '%s'\n\n", fileName);
    } else {
        // some error during reception?
        fprintf(stderr, "Error when recieving bin from node: %s\n", bootloadErrToStr(err));
        errOccured = true;
    }

    // fallthrough to cleanup

readCleanup:
    if (fileName !=  NULL && fileName != storeName)
        free(fileName);
    if (fileCache)
        free(fileCache);
    fclose(binFile);
    if (errOccured)
        errExit(0);

}

// print out all memory in uC
void doPrintMemorySetupCmd(void)
{
    canframe_t sendFrm, recvFrm;
    initFrame(&sendFrm);
    initFrame(&recvFrm);

    // get memory setup from Node uC
    initMemoryInfo(&sendFrm, &recvFrm);

    fprintf(stdout, "\nStartAddress:  %x\n"
                      "Endaddress:    %x"
                      "PageSize:     %u\n"
                      "Nr pages:     %u\n"
                      "Totalsize:    %u\n\n",
                       memory.bootRomStart,
                       memory.bootRomEnd,
                       memory.pageSize,
                       memory.nrPages,
                       memory.nrPages * memory.pageSize);

}

void doChecksumCmd(memoptions_t *mopt)
{
    canframe_t sendFrm, recvFrm;
    initFrame(&sendFrm);
    initFrame(&recvFrm);

    checkMemOptWithinBounds(mopt);

    can_bootloaderErrs_e err = getRemoteChecksum(mopt, &sendFrm, &recvFrm);
    if (err != C_bootloaderErrOK) {
        fprintf(stderr, "**Error when doing checksum remote: %s\n",
                bootloadErrToStr(err));
        errExit(0);
    }

    fprintf(stdout, "Flash in node @ %x to %x\ncrc32 = %u\n",
            mopt->lowerbound, mopt->upperbound, nodeCrc);
}

void doResetCmd(void)
{
    canframe_t sendFrm, recvFrm;
    initFrame(&sendFrm);
    initFrame(&recvFrm);

    can_bootloaderErrs_e err = resetNode(&sendFrm, &recvFrm);
    if (err != C_bootloaderErrOK)
        nodeErrExit("Error while reset:", err);

    fprintf(stdout, "Reset done succesfully!");
}

// this function prints crc of a local file
void doChecksumLocalCmd(const char *binName)
{
    long sz;
    uint8_t *cache = readLocalFile(binName, &sz);
    binCrc = crc32(0, cache, sizeof(uint8_t) * (size_t)sz);
    free(cache);

    fprintf(stdout, "\nBinfile crc32 = %u\n\n", binCrc);
}

void doCompareCmd(const char *binName)
{
    canframe_t sendFrm, recvFrm;
    initFrame(&sendFrm);
    initFrame(&recvFrm);

    // get memory setup from Node uC
    initMemoryInfo(&sendFrm, &recvFrm);

    // get local file crc and size
    long sz;
    uint8_t *buf = readLocalFile(binName, &sz);
    binCrc = crc32(0, buf, (size_t)sz);
    free(buf);

    fprintf(stdout, "\nBinfile crc32 = %u\n\n", binCrc);

    // get remote crc up to size of our localfile
    memoptions_t mopt = {
         memory.bootRomStart,
         memory.bootRomStart + (uint32_t)sz
    };

    // request crc from node
    doChecksumCmd(&mopt);

    if (binCrc == nodeCrc)
        fprintf(stdout, "Files match!!!\n\n");
    else
        fprintf(stdout, "*********** MISMATCH ***********\n\n");
}

void doEraseCmd(memoptions_t *mopt)
{
    canframe_t sendFrm, recvFrm;
    initFrame(&sendFrm);
    initFrame(&recvFrm);

    checkMemOptWithinBounds(mopt);

    sendFrm.can_dlc = 0;
    sendFrm.data[sendFrm.can_dlc++] = C_bootloaderEraseFlash;

    // startpage to erase
    uint32_t startPage = (mopt->lowerbound - memory.bootRomStart) / memory.pageSize;
    sendFrm.data[sendFrm.can_dlc] =  (startPage & 0x000000FF);
    sendFrm.data[sendFrm.can_dlc] =  (startPage & 0x0000FF00) >> 8;

    // nr of pages
    uint32_t nrPages = (mopt->upperbound - memory.bootRomEnd) / memory.pageSize;
    sendFrm.data[sendFrm.can_dlc] =  (nrPages & 0x000000FF);
    sendFrm.data[sendFrm.can_dlc] =  (nrPages & 0x0000FF00) >> 8;

    if (!cansend(&sendFrm, 1000))
        errExit("Couldnt send on can network\n");

    const uint32_t timeout = 30000;
    if (!canrecv(&recvFrm, timeout)) {
        fprintf(stderr, "No repsonse from node, timeout after:%dms", timeout);
        errExit(0);
    }

    if (recvFrm.can_dlc < 1)
        errExit("Malformed response fron Node\n");

    if (recvFrm.data[0] != C_bootloaderEraseFlash)
        errExit("Node reponded with wrong command\n");

    if (recvFrm.data[1] != C_bootloaderErrOK)
        nodeErrExit("An error occured in node %s\n", recvFrm.data[1]);
}


void doWriteCmd(memoptions_t *mopt, char *binName)
{
    bool errorOccured = false;
    canframe_t sendFrm, recvFrm;
    initFrame(&sendFrm);
    initFrame(&recvFrm);

    checkMemOptWithinBounds(mopt);

    // get local file crc and size
    long sz;
    uint8_t *fileCache = readLocalFile(binName, &sz);
    binCrc = crc32(0, fileCache, (size_t)sz);

    // does it fit within given space?
    if ((uint32_t)sz > mopt->upperbound - mopt->lowerbound) {
        fprintf(stderr, "**Binfile is bigger than given memory, available:%ubytes  bin is:%libytes\n", (mopt->upperbound - mopt->lowerbound), sz);
        errorOccured = true;
        goto writeCleanup;
    }

    fprintf(stdout, "\n--Writing binfile '%s' with crc32 = %u\n\n", binName, binCrc);

    can_bootloaderErrs_e err = writeFileToNode(mopt, fileCache,
                                               &sendFrm, &recvFrm);
    if (err == C_bootloaderErrOK) {
        fprintf(stdout, "Write process completed successfully!\nChecking checksum...\n");

        err = getRemoteChecksum(mopt, &sendFrm, &recvFrm);
        if (err != C_bootloaderErrOK) {
            fprintf(stderr, "**Error while getting checksum %s\n", bootloadErrToStr(err));
            errorOccured = true;
            goto writeCleanup;
        }

        if (nodeCrc != binCrc) {
            fprintf(stderr, "**Error checksum doesnt match! binfile:%u node:%u\n", binCrc, nodeCrc);
            errorOccured = true;
            goto writeCleanup;
        }

        fprintf(stdout, "CRC match!! bincrc:%u  nodecrc:%u\n\n", binCrc, nodeCrc);
        fprintf(stdout, "You can now reset node to make it run normally on the new firmware\n  Do you want to reset? [Y] or [N]");
        int ch = getc(stdin);
        if (ch == 'Y' || ch == 'y' || ch == 'j' || ch == 'J') {
            err = resetNode(&sendFrm, &recvFrm);
            if (err != C_bootloaderErrOK) {
                fprintf(stderr, "**Error during reset: %s\n\n", bootloadErrToStr(err));
                errorOccured = true;
                goto writeCleanup;
            }
        }

        // fallthrough to writeCleanup

    } else {
        fprintf(stderr, "**Write process error %s\n", bootloadErrToStr(err));
        errorOccured = true;
        goto writeCleanup;
    }

writeCleanup:
    free(fileCache);
    if (errorOccured)
        errExit(0);
}
