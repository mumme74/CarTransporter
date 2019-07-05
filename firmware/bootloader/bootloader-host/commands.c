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
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/uio.h>
#include <net/if.h>

#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/can/bcm.h>
#include <linux/errno.h> // must be included before errno.h
#include <errno.h>

#include "crc32.h"
#include "can_protocol.h"
#include "commands.h"
#include "canbridge.h"

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

// for progressbar
#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60


// memory structure of uC
typedef struct {
    uint32_t bootRomStart;
    uint32_t bootRomEnd;
    uint16_t pageSize;
    uint16_t nrPages;
} memoryinfo_t;

extern uint32_t canIdx;

static memoryinfo_t memory = { 0 ,0 ,0 ,0 };
static uint32_t nodeCrc,
                binCrc;


void printProgress (double percentage)
{
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf ("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush (stdout);
}

static char unknownErrbuf[50];

static FILE *binFile = NULL;

static char *bootloadErrToStr(can_bootloaderErrs_e err)
{
    switch (err) {
    case C_bootloaderErr:                       return "BootloaderErr";
    case C_bootloaderErrResend:                 return "BootloaderErrResend";
    case C_bootloaderErrStartAddressOutOfRange:  return "BootloaderErrStartAddressOutOfRange";
    case C_bootloaderErrEndAddressOutOfRange:    return "BootloaderErrEndAddressOutOfRange";
    case C_bootloaderErrStartPageOutOfRange:    return "BootloaderErrStartPageOutOfRange";
    case C_bootloaderErrPageLenOutOfRange:      return "BootloaderErrPageLenOutOfRange";
    case C_bootloaderErrPageWriteFailed:        return "BootloaderErrWriteFailed";
    case C_bootloaderErrPageEraseFailed:        return "BootloaderErrPageEraseFailed";
    case C_bootloaderErrCanPageOutOfOrder:      return "BootloaderErrCanPageOutOfOrder";
    case C_bootloaderErrOK:                     return "BootloaderErrOK";
    case C_bootloaderErrNoResponse:             return "BootloaderErrNoResponse";
    case C_bootloaderErrSendFailed:             return "BootloaderErrSendFailed";
    case C_bootloaderErrReceiveTimeout:         return "BootloaderErrReceiveTimeout";
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

// recives a specific frame with a specific data at pos
static bool filteredRecv(canframe_t *recvFrm, int timeout,
                  uint8_t dataPos, uint8_t isvlu)
{
    // get result
    int res = 0;
    // we might get another queued frame, discard that...
    while ((res = canbridge_recv(recvFrm, timeout)) > 0)  {
        if ((recvFrm->can_dlc > 0) &&
            (recvFrm->data[dataPos] == isvlu))
        {
            break;
        }
        res = 0;
        if (abortVar)
            cleanExit();
    }
    if (res < 1)
        printCanError();

    return res > 0;
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
    if (canbridge_send(sendFrm, 1000) < 1) {
        printCanError();
        errExit("can't send\n");
    }

    // get result
    if (!filteredRecv(recvFrm, 1000, 0, C_bootloaderStartAddress))
        errExit("No response from node\n");

    memory.bootRomStart = (uint32_t)(recvFrm->data[4] << 24 | recvFrm->data[3] << 16 |
                                     recvFrm->data[2] << 8  | recvFrm->data[1]);

    // get the rest
    sendFrm->can_dlc = 1;
    sendFrm->data[0] = C_bootloaderMemPageInfo;
    if (canbridge_send((sendFrm), 1000) < 1) {
        printCanError();
        errExit("cant send\n");
    }


    if (!filteredRecv(recvFrm, 1000, 0, C_bootloaderMemPageInfo))
        errExit("No response from node\n");

    memory.pageSize = (uint16_t)(recvFrm->data[2] << 8 | recvFrm->data[1]);
    memory.nrPages  = (uint16_t)(recvFrm->data[4] << 8 | recvFrm->data[3]);

    memory.bootRomEnd = memory.bootRomStart +
                            (memory.pageSize * memory.nrPages);
}

static can_bootloaderErrs_e resetNode(canframe_t *sendFrm, canframe_t *recvFrm)
{
    sendFrm->can_dlc = 1;
    sendFrm->data[0] = C_bootloaderReset;
    if (canbridge_send(sendFrm, 1000) < 1) {
        printCanError();
        return C_bootloaderErrSendFailed;
    }

    if (!filteredRecv(recvFrm, 2000, 0, C_bootloaderReset))
        return C_bootloaderErrNoResponse;

    return C_bootloaderErrOK;
}

static can_bootloaderErrs_e getRemoteChecksum(memoptions_t *mopt,
                                             canframe_t *sendFrm,
                                             canframe_t *recvFrm)
{
    sendFrm->can_dlc = 0;
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
    if (canbridge_send(sendFrm, 100) < 1) {
        printCanError();
        return C_bootloaderErr;
    }

    if (!filteredRecv(recvFrm, 2000, 0, C_bootloaderChecksum))
        return C_bootloaderErrNoResponse;

    nodeCrc = (uint32_t)(recvFrm->data[4] << 24 | recvFrm->data[3] << 16 |
                         recvFrm->data[2] << 8  | recvFrm->data[1]);

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
    if (binFile == NULL) {
        fprintf(stderr, "**Couldn't open binfile '%s'\n", binName);
        errExit(0);
    }

    // get size
    fseek(binFile, 0L, SEEK_END);
    *sz = ftell(binFile);
    rewind(binFile);

    fprintf(stdout, "--Using file: %s\n", binName);

    if (*sz < 0)
        errExit("Error when determining filesize\n");

    if (*sz > 3072000) {
        fclose(binFile);
        binFile = NULL;
        errExit("Binfile over 3Mb, aborting....\n");
     }

    // create buffer
    uint8_t *fileBuf = (uint8_t*)malloc(sizeof(uint8_t) * (size_t)*sz);
    if (fileBuf == NULL)
        errExit("Couldnt read file into RAM memory. any space left?\n");

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

    uint16_t canPageNr = 0,
             memPageGuard = 0;
    uint32_t crc;
    uint8_t *addrAtCanPageStart;
    uint8_t *addrAtMemPageStart;

writeMemPageLoop:
    addrAtMemPageStart = addr;
    memPageGuard = 0;
    canPageNr = 0;

writeCanPageLoop:
    // begin header
    // on top of the 889 bytes restriction in CAN pages, BOOTLOADER_PAGE_SIZE,
    // it must also send in chunks divisible by mempage.pageSize (ie 2048)
    addrAtCanPageStart = addr;

    // concept here is that addr advances in send loop with nr bytes written
    // we use addrAtMemPageStart to make sure we transmitt in pageSize chunks
    // we will eventually reach endAddr
    frameNr = 0;
    uint8_t *eAddr = MIN((addrAtMemPageStart + memory.pageSize +1), endAddr+1);
    frames = MIN((eAddr - addr -1), ((BOOTLOADER_PAGE_SIZE+1) * 7)) / 7;
    if ((MIN((eAddr - addr -1), ((BOOTLOADER_PAGE_SIZE+1) * 7)) % 7) > 0)
        frames += 1;

    crc = crc32(0, addr, MIN(MIN((eAddr - addr -1), (frames * 7)),
                             ((BOOTLOADER_PAGE_SIZE+1) * 7)));
    sendFrm->can_dlc = 8;
    sendFrm->data[0] = C_bootloaderWriteFlash;
    sendFrm->data[1] = (crc & 0x000000FF);
    sendFrm->data[2] = (crc & 0x0000FF00) >> 8;
    sendFrm->data[3] = (crc & 0x00FF0000) >> 16;
    sendFrm->data[4] = (crc & 0xFF000000) >> 24;
    // len of page
    sendFrm->data[5] = frames;
    sendFrm->data[6] = (canPageNr & 0x000000FF);
    sendFrm->data[7] = (canPageNr & 0x0000FF00) >> 8;
    if (canbridge_send(sendFrm, 1000) < 1) {
        printCanError();
        return C_bootloaderErrSendFailed;
    }

    // begin payload frames
    while (frameNr < frames) {
      sendFrm->data[0] = frameNr++;
      uint8_t end = MIN(7, memory.pageSize -
                            (addr - addrAtMemPageStart)) + 1;
      sendFrm->can_dlc = end;
      for (uint8_t i = 1; i < end; ++i) {
        sendFrm->data[i] = *addr;
        if (memPageGuard++ >= memory.pageSize) {
          // break at 2048 bits for a stm32
          // should never arrive here!
          return C_bootloaderErrMemoryPageViolation;
        }
        if (++addr >= eAddr) {
          sendFrm->can_dlc = i;
          break; // frameId should now be frameId == frames
        }
      }
      if (canbridge_send(sendFrm, 1000) < 1) {
          printCanError();
          return C_bootloaderErrSendFailed;
      }
    }

    // wait for remote to Ack this page
    do {
        // progressbar
        uint32_t total = (uint32_t)(endAddr - fileCache),
                 diff = total - (uint32_t)(endAddr - addr);
        double progress = (double)diff / total;
        printProgress(progress);

        // get response from node
        if (canbridge_recv(recvFrm, 1000) < 1) {
            printCanError();
            return C_bootloaderErrReceiveTimeout;
        }
        if (!abortVar)
            return C_bootloaderErrUnknown;
    } while(recvFrm->can_dlc != 2 &&
            recvFrm->data[0] != C_bootloaderReadFlash);

    // check if we should resend
    if (recvFrm->data[1] == C_bootloaderErrOK)
      ++canPageNr;
    else if (recvFrm->data[1] == C_bootloaderErrResend) {
        addr = addrAtCanPageStart;
        memPageGuard -= (frameNr * 7) + (8 - sendFrm->can_dlc);
        goto writeCanPageLoop;
    }
    else
        nodeErrExit("Error returned from node %s\n", recvFrm->data[1]);

    if (addr >= endAddr)
        return C_bootloaderErrOK; // finished!
    else if (addr - addrAtMemPageStart >= memory.pageSize)
        goto writeMemPageLoop;
    goto writeCanPageLoop;
}


// handles reception of binfile, communicates with node, fills file in filecache
static can_bootloaderErrs_e recvFileFromNode(uint8_t *fileCache, canframe_t *sendFrm,
                                            canframe_t *recvFrm, uint32_t startAddr,
                                            uint32_t endAddr)
{
    uint16_t canPageNr;
    uint8_t frames, frameNr;
    uint32_t crc = 0;

    uint32_t lastStoredIdx = 0,
             previousStoredIndex = 0;

    canPageNr = 0;

readCanPageLoop:
    frameNr = frames = 0;
    lastStoredIdx = previousStoredIndex;

    // loop to receive a complete canPage
    do {
      if (canbridge_recv(recvFrm, 1000) < 1) {
          printCanError();
          return C_bootloaderErrNoResponse;
      }

      if ((recvFrm->data[0] & 0x80) && frames == 0) {
        // get header
        crc = (uint32_t)(recvFrm->data[4] << 24 | recvFrm->data[3] << 16 |
                         recvFrm->data[2] << 8  | recvFrm->data[1]);
        frames = recvFrm->data[5];
        if (canPageNr != (recvFrm->data[7] << 8 | recvFrm->data[6])) {
          return C_bootloaderErrCanPageOutOfOrder;
        }
      } else {
        // it's a payload frame, frames might arrive at random order
        uint32_t cPage = (canPageNr * (BOOTLOADER_PAGE_SIZE +1)) * 7,
                 fOffset = ((recvFrm->data[0] & 0x7F) * 7);
        if (lastStoredIdx < cPage + fOffset - 1 + recvFrm->can_dlc)
            lastStoredIdx = cPage + fOffset - 1 + recvFrm->can_dlc;
        for (uint8_t i = 1; i < recvFrm->can_dlc; ++i) {
          fileCache[cPage + fOffset + i -1] = recvFrm->data[i];
        }
        ++frameNr;
        //printf("at idx:%u cPage:%u fOffset:%u\n", cPage + fOffset + recvFrm->can_dlc -1, cPage, fOffset);
      }

    } while (frameNr < frames);

    // progressbar
    uint32_t total = (uint32_t)(endAddr - startAddr -1);
    double progress = (double)lastStoredIdx / total;
    printProgress(progress);

    // check canPage
    // pointer buf[0] advance to start of canPage.
    // last frame might be less than 7 bytes
    uint32_t recvCrc = crc32(0, fileCache + (canPageNr * (BOOTLOADER_PAGE_SIZE +1) * 7),
                             lastStoredIdx - previousStoredIndex);
    if (crc != recvCrc) {
      // notify node that we need this canPage retransmitted
      sendFrm->can_dlc = 2;
      sendFrm->data[0] = C_bootloaderReadFlash;
      sendFrm->data[1] = C_bootloaderErrResend;
      if (canbridge_send(sendFrm, 1000) < 1) {
          printCanError();
          return C_bootloaderErrSendFailed;
      }

      goto readCanPageLoop;
    }


    // if we haven't received a complete pageSize yet
    // and we are not at end of bin
    // (last memPage sent from invoker might be less than pageSize)
    if (lastStoredIdx < (endAddr - startAddr -1)) {
      ++canPageNr;
      sendFrm->can_dlc = 2;
      sendFrm->data[0] = C_bootloaderReadFlash;
      sendFrm->data[1] = C_bootloaderErrOK;
      if (canbridge_send(sendFrm, 1000) < 1) {
          printCanError();
          return C_bootloaderErrSendFailed;
      }
      previousStoredIndex = lastStoredIdx;
      goto readCanPageLoop;
    }

    return C_bootloaderErrOK;
}

// --------------------------------------------------------------------
// public functions and variables

void cleanup(void)
{
    if (canbridge_status() != CANBRIDGE_CLOSED)
        canbridge_close();
    if (binFile)
        fclose(binFile);
    binFile = 0;
}

void cleanExit(void)
{
    cleanup();
    exit(EXIT_SUCCESS);
}


void errExit(char *errStr)
{
    if (errStr != 0) {
        fprintf(stderr, "%s%s", "**", errStr);
        fflush(stderr);
    }
    cleanup();
    exit(EXIT_FAILURE);
}

void printCanError(void)
{
    if (strlen(canbridge_errmsg) > 0) {
        fprintf(stderr, "**%s", canbridge_errmsg);
        fflush(stderr);
    }
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
    //debug create a file sequenced (easier view in memory browser)
    //for (int i = 0; i < 256; ++i) {
    //    for (int j = 0; j < 256; j++)
    //        fputc(j, binFile);
    //}
    //goto readCleanup;

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

    if (canbridge_send(&sendFrm, 100) < 1) {
        printCanError();
        errExit("Failed to send flash command");
    }

    if (!filteredRecv(&recvFrm, 100, 0, C_bootloaderReadFlash))
        errExit("No ack for CAN read\n");

    if (recvFrm.data[1] != C_bootloaderErrOK)
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

    // release from blocking loop
    sendFrm.can_dlc = 1;
    sendFrm.data[0] = C_bootloaderUnblock;
    if (canbridge_send(&sendFrm, 1000) < 1) {
        printCanError();
        errExit("Couldn't send unblock");
    }

    if (err == C_bootloaderErrOK) {
        // successfull
        if (fwrite(fileCache, sizeof(uint8_t), len, binFile) != len) {
            fprintf(stderr, "**Error when saving file to disk\n");
            errOccured = true;
            goto readCleanup;
        }

        fprintf(stdout, "\n--Successfully written to '%s'\n\n", fileName);
    } else {
        // some error during reception?
        fprintf(stderr, "**Error when recieving bin from node: %s\n", bootloadErrToStr(err));
        errOccured = true;
    }

    // fallthrough to cleanup

readCleanup:
    if (fileName !=  NULL && fileName != storeName)
        free(fileName);
    if (fileCache)
        free(fileCache);
    fclose(binFile);
    binFile = 0;
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

    fprintf(stdout, "\nStartAddress:  0x%x\n"
                      "Endaddress:    0x%x\n"
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

    fprintf(stdout, "\n--Flash in node @ %x to %x\n--Memory crc32 = %u\n",
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

    fprintf(stdout, "\n--Reset done succesfully!\n");
}

// this function prints crc of a local file
void doChecksumLocalCmd(const char *binName)
{
    long sz;
    uint8_t *cache = readLocalFile(binName, &sz);
    binCrc = crc32(0, cache, sizeof(uint8_t) * (size_t)sz);
    free(cache);

    fprintf(stdout, "\n--Binfile crc32 = %u\n\n", binCrc);
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

    fprintf(stdout, "\n--Binfile crc32 = %u\n\n", binCrc);

    // get remote crc up to size of our localfile
    memoptions_t mopt = {
         memory.bootRomStart,
         memory.bootRomStart + (uint32_t)sz
    };

    // request crc from node
    doChecksumCmd(&mopt);

    if (binCrc == nodeCrc)
        fprintf(stdout, "\n--Files match!!!\n\n");
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
    sendFrm.data[sendFrm.can_dlc++] =  (startPage & 0x000000FF);
    sendFrm.data[sendFrm.can_dlc++] =  (startPage & 0x0000FF00) >> 8;

    // nr of pages
    uint32_t nrPages = (mopt->upperbound - mopt->lowerbound) / memory.pageSize;
    sendFrm.data[sendFrm.can_dlc++] =  (nrPages & 0x000000FF);
    sendFrm.data[sendFrm.can_dlc++] =  (nrPages & 0x0000FF00) >> 8;

    if (canbridge_send(&sendFrm, 1000) < 1) {
        printCanError();
        errExit("Couldnt send on can network\n");
    }

    const uint32_t timeout = 30000;
    if (canbridge_recv(&recvFrm, timeout) < 1) {
        printCanError();
        fprintf(stderr, "**No repsonse from node, timeout after:%dms", timeout);
        errExit(0);
    }

    if (recvFrm.can_dlc < 1)
        errExit("Malformed response fron Node\n");

    if (recvFrm.data[0] != C_bootloaderEraseFlash)
        errExit("Node reponded with wrong command\n");

    if (recvFrm.data[1] != C_bootloaderErrOK)
        nodeErrExit("An error occured in node %s\n", recvFrm.data[1]);

    fprintf(stdout, "--Successfully erased memory!\n");
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
    } else if (mopt->lowerbound == memory.bootRomStart &&
               mopt->upperbound == memory.bootRomEnd)
    {
        // send only our binfile (is probably smaller than max memory)
        mopt->upperbound = memory.bootRomStart + (uint32_t)sz;
    }

    fprintf(stdout, "\n--Writing binfile '%s' with crc32 = %u\n\n", binName, binCrc);

    // now that we have successfully opened the file we retrive memory from node
    sendFrm.can_dlc = 0;
    sendFrm.data[sendFrm.can_dlc++] = C_bootloaderWriteFlash;
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

    if (canbridge_send(&sendFrm, 100) < 1) {
        errExit("Failed to send flash command");
    }

    if (!filteredRecv(&recvFrm, 100, 0, C_bootloaderWriteFlash))
        errExit("No ack for CAN write\n");

    if (recvFrm.data[1] != C_bootloaderErrOK)
        nodeErrExit("Node gives error when init write mode:", recvFrm.data[1]);

    can_bootloaderErrs_e err = writeFileToNode(mopt, fileCache,
                                               &sendFrm, &recvFrm);

    // release from blocking loop
    sendFrm.can_dlc = 1;
    sendFrm.data[0] = C_bootloaderUnblock;
    if (canbridge_send(&sendFrm, 1000) < 1) {
        printCanError();
        errExit("Couldn't send unblock");
    }

    if (err == C_bootloaderErrOK) {
        fprintf(stdout, "\n--Write process completed successfully!\nChecking checksum...\n");

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

        fprintf(stdout, "\n--CRC match!! bincrc:%u  nodecrc:%u\n\n", binCrc, nodeCrc);
        fprintf(stdout, "\n--You can now reset node to make it run normally on the new firmware\n  Do you want to reset? [Y] or [N]");
        int ch = getc(stdin);
        if (ch == 'Y' || ch == 'y' || ch == 'j' || ch == 'J') {
            err = resetNode(&sendFrm, &recvFrm);
            if (err != C_bootloaderErrOK) {
                fprintf(stderr, "**Error during reset: %s\n\n", bootloadErrToStr(err));
                errorOccured = true;
                goto writeCleanup;
            }

            fprintf(stdout, "\n--Successfully reset node!\n\n");
        }

        // fallthrough to writeCleanup

    } else {
        fprintf(stderr, "\n**Write process error %s\n", bootloadErrToStr(err));
        errorOccured = true;
        goto writeCleanup;
    }

writeCleanup:
    free(fileCache);
    if (errorOccured)
        errExit(0);
}


void doBootloaderModeCmd(void)
{
    canframe_t sendFrm, recvFrm;
    initFrame(&sendFrm);
    initFrame(&recvFrm);
    bool printResetFail = true;
    while (true) {
        sendFrm.can_dlc = 1;
        sendFrm.data[0] = C_bootloaderReset;
        if (canbridge_send(&sendFrm, 100) < 1) {
            printCanError();
            if (printResetFail) {
                printResetFail =false;
                fprintf(stderr, "**Unable to reset node, please powercycle node (ie. yank fuse)\n");
            }
        }
        if (canbridge_recv(&recvFrm, 1000) == 1) {
            if ((recvFrm.can_dlc > 0) &&
                (recvFrm.data[0] == C_bootloaderWait ||
                 recvFrm.data[0] == C_bootloaderReset))
            {
                for (int i = 0; i < 10; ++i) {
                    // shower our node with commands in the hope one will catch
                    sendFrm.can_dlc = 1;
                    sendFrm.data[0] = C_bootloaderWait;
                    canbridge_send(&sendFrm, 3);

                }
                goto bootModeOut;
            }
        } else {
            printCanError();
            errExit("Can't send to CAN\n");
        }
    }

bootModeOut:
    // trigger a cmd so we set node in cammand mode
    fprintf(stdout, "\n--Successfully set in bootloadermode\n--Printing memory\n\n");
    doPrintMemorySetupCmd(); // using memory print for this
}
