/*
 * commands.c
 *
 *  Created on: 27 jul 2018
 *      Author: jof
 */


#include "commands.h"
#include "system.h"
#include "crc32.h"
#include <can_protocol.h>



#define CAN_POST_MSG  while ((canPost(msg) < 0))        // --""--       txv
#define MIN(x, y) (((x) < (y)) ? (x) : (y))



// _appRomXXX from linkscript
extern const uint32_t _appRomStart, _appRomEnd;

static void sendErr(canframe_t *msg, can_bootloaderErrs_e err)
{
  msg->DLC = 2;
  msg->data8[1] = err;
  CAN_POST_MSG;
}

static byte4_t addr, endAddr;

// used by both readflash and writeflash
static bool getAndCheckAddress(canframe_t *msg)
{
  addr.b0 = msg->data8[1];
  addr.b1 = msg->data8[2];
  addr.b2 = msg->data8[3];
  addr.b3 = msg->data8[4];
  endAddr.b0 = msg->data8[5];
  endAddr.b1 = msg->data8[6];
  endAddr.b2 = msg->data8[7];
  endAddr.b3 = 0;
  endAddr.vlu += addr.vlu;
//  addr = (uint8_t*)((msg->data8[4] << 24) | (msg->data8[3] << 16) |
//                    (msg->data8[2] << 8)  | (msg->data8[1])) ;
//  endAddr = addr + ((msg->data8[7] << 16) | (msg->data8[6] << 8) |
//                    (msg->data8[5]));
  if (addr.ptr32 < &_appRomStart) {
    sendErr(msg, C_bootloaderErrStartAddressOutOfRange);
    return false; // to commands loop
  } else if (endAddr.ptr32 > &_appRomEnd) {
    sendErr(msg, C_bootloaderErrEndAddressOutOfRange);
    return false; // to commands loop
  }
  return true;
}

static bool runCommand(canframe_t *msg)
{
  static byte4_t crc;
  static byte2_t canPageNr; // as in CAN page
  static uint8_t  frames, frameNr; //, *addr, *endAddr;
rescan:
  print_str("rC\r\n");
  switch(msg->data8[0]){
  case C_bootloaderReadFlash: {
    if (!getAndCheckAddress(msg))
      break;
    // report to invoker that we are in read command mode
    msg->data8[1] = C_bootloaderErrOK;
    msg->DLC = 2;
    CAN_POST_MSG;

    canPageNr.vlu = 0;
    uint8_t *addrAtCanPageStart;

readPageLoop:
    // begin header
    frameNr = 0;
    addrAtCanPageStart = addr.ptr8;
    // concept here is that addr advances in send loop with nr bytes written
    // we will eventually reach endAddr
    // also when reading memory we don't need that 2K memory page restriction that we need when writing to memory
    frames = MIN(((endAddr.vlu - addr.vlu) / 7 + (endAddr.vlu - addr.vlu) % 7),
                 BOOTLOADER_PAGE_SIZE+1);
print_str("read frames:");print_uint(frames);endl();

    crc.vlu = crc32(0, addr.ptr8 + (frameNr * 7),
                    MIN(endAddr.vlu - addr.vlu, ((BOOTLOADER_PAGE_SIZE+1) * 7)));
    msg->DLC = 8;
    msg->data8[0] = C_bootloaderReadFlash;
    msg->data8[1] = crc.b0; // (crc & 0x000000FF);
    msg->data8[2] = crc.b1; // (crc & 0x0000FF00) >> 8;
    msg->data8[3] = crc.b2; // (crc & 0x00FF0000) >> 16;
    msg->data8[4] = crc.b3; // (crc & 0xFF000000) >> 24;
    // len of page
    msg->data8[5] = frames;
    msg->data8[6] = canPageNr.b0; //(canPageNr & 0x00FF);
    msg->data8[7] = canPageNr.b1; //(canPageNr & 0xFF00) >> 8;
    CAN_POST_MSG;
    // inc canPageNr when we know if invoker has crc accepted this frame
    // done below

    // begin payload frames
    while (frameNr < frames) {
      msg->data8[0] = frameNr++;
      for (uint8_t i = 1; i < 8; ++i) {
        msg->data8[i] = *addr.ptr8;
        if (addr.vlu++ >= endAddr.vlu) {
          msg->DLC = i;
          break; // frameId should now be frameId == frames
        }
      }
      CAN_POST_MSG;
    }

    // wait for remote to Ack this page

    print_str("wait ack adr:");print_uint(addr.vlu);
    do {
      canWaitRecv(msg);
      print_str("got can");endl();
      if (msg->DLC != 2 || msg->data8[0] != C_bootloaderReadFlash)
        goto rescan;
      if (msg->IDE != (canId & (CAN_MSG_ID_MASK | CAN_MSG_TYPE_MASK )))
	continue;
    } while(msg->data8[0] != C_bootloaderReadFlash);

    print_str("after ack adr:");print_uint(addr.vlu);
    print_str(" eAdr:");print_uint(endAddr.vlu);endl();

    // check if we should resend
    if (msg->data8[1] == C_bootloaderErrOK)
      ++canPageNr.vlu;
    else {
      // resend
      addr.ptr8 = addrAtCanPageStart;
    }
    if (addr.vlu >= endAddr.vlu)
      return true; // finished!, to commands loop
    goto readPageLoop;
  }  break;

  case C_bootloaderChecksum: {
    if (!getAndCheckAddress(msg))
      break;
    crc.vlu = crc32(0, addr.ptr8, endAddr.vlu - addr.vlu);
    msg->DLC = 5;
    msg->data8[1] = crc.b0; //(crc & 0x000000FF);
    msg->data8[2] = crc.b1; //(crc & 0x0000FF00) >> 8;
    msg->data8[3] = crc.b2; //(crc & 0x00FF0000) >> 16;
    msg->data8[4] = crc.b3; //(crc & 0xFF000000) >> 24;
    CAN_POST_MSG; // if buffer full we bust out to main
  }  break;

  case C_bootloaderWriteFlash: {
    if (!getAndCheckAddress(msg))
      break;
    uint16_t memPages = ((endAddr.vlu - addr.vlu) / pageSize);

    uint16_t memPagesWritten = 0 -1;

    // initial state is tell invoker we are ready to recieve
    msg->data8[1] = C_bootloaderErrOK;
    msg->DLC = 2;
    CAN_POST_MSG;

    uint8_t buf[pageSize];
    can_bootloaderErrs_e err;
    uint32_t lastStoredIdx;

writeMemPageLoop:
    ++memPagesWritten;
    canPageNr.vlu = 0;

writeCanPageLoop:
    frameNr = frames = 0;
    lastStoredIdx = 0;

    // loop to receive a complete canPage
    do {
      canWaitRecv(msg);
      if ((msg->data8[0] & 0x80)) {
        if (msg->data8[0] != C_bootloaderWriteFlash)
          return true; // stuck in loop, we should probably abort

        // get header
        crc.b0 = msg->data8[1];
        crc.b1 = msg->data8[2];
        crc.b2 = msg->data8[3];
        crc.b3 = msg->data8[4];
        //crc = (msg->data8[4] << 24 | msg->data8[3] << 16 |
        //       msg->data8[2] << 8  | msg->data8[1]);

        frames = msg->data8[5];

        byte2_t pgNr;
        pgNr.b0 = msg->data8[6];
        pgNr.b1 = msg->data8[6];

        //if (canPageNr != (msg->data8[7] << 8 | msg->data8[6])) {
        if (canPageNr.vlu != pgNr.vlu) {
          sendErr(msg, C_bootloaderErrCanPageOutOfOrder);
          return true; // to commands loop
        }
      } else {
        // it's a payload frame
        uint32_t cPage = (canPageNr.vlu * (BOOTLOADER_PAGE_SIZE+1) * 7),
                 fOffset = ((msg->data8[0] & 0x7F) * 7);
        if (lastStoredIdx < cPage + fOffset + msg->DLC -1)
            lastStoredIdx = cPage + fOffset + msg->DLC -1;
        for (uint8_t i = 1; i < 8; ++i) {
          if (msg->DLC == i)
            break; // frameNr == frames should be true now
          buf[cPage + fOffset + i -1] = msg->data8[i];
        }
        ++frameNr;
      }

    } while (frameNr < frames);

    // check canPage
    // pointer buf[0] advance to start of canPage.
    // last frame might be less than 7 bytes

    uint32_t recvCrc = crc32(0, buf + (canPageNr.vlu * (BOOTLOADER_PAGE_SIZE+1) * 7),
                             lastStoredIdx - (canPageNr.vlu * (BOOTLOADER_PAGE_SIZE+1) * 7));
    if (crc.vlu != recvCrc) {
      sendErr(msg, C_bootloaderErrResend);
      goto writeCanPageLoop;// resend this canPage
    } else {
      ++canPageNr.vlu;
    }


    // if we haven't received a complete pageSize yet
    // and we are not at end of bin
    // (last memPage sent from invoker might be less than pageSize)
    if (lastStoredIdx < pageSize &&
        memPagesWritten < memPages)
    {
      // notify invoker that we are ready for next frame
      sendErr(msg, C_bootloaderErrOK);
      goto writeCanPageLoop;
    }

    // here we are at the last canPage of this memPage
    // if we're also at the last memPage we should also reset bytes
    // up to end of memPage (fill with 0xFF)

    // setup a pointer to our buffer with offset to end of just received +1
    //  (msg->DLC is +1 due to data8[0] is frameid)
    //uint8_t *pbuf = buf + (((canPageNr -1) * 0x7F) * 7) + (frames * 7) -  (7 - msg->DLC) +1;
    uint8_t *pbuf = &buf[MIN(lastStoredIdx, pageSize) -1];
    while (++pbuf < (buf + pageSize)) {
      *pbuf = 0xFF;
    }

    // write flashpage to rom
    err = systemFlashWritePage((uint16_t*)buf, addr.ptr16);
    if (err != C_bootloaderErrOK) {
      sendErr(msg, err);
      break;
    } else {
      // advance addr to save in next memorypage next time
      addr.vlu += pageSize;
    }

    // a *.bin might span over many memPages
    if (memPagesWritten < memPages) {
      // notify invoker that we are ready for next frame
      sendErr(msg, C_bootloaderErrOK);
      goto writeMemPageLoop;
    }

    sendErr(msg, err);

  }  break; // to commands loop

  case C_bootloaderEraseFlash: {
    canPageNr.b0 = msg->data8[1];
    canPageNr.b1 = msg->data8[2];
    //canPageNr = (msg->data8[2] >> 8 | msg->data8[1]);
    addr.ptr8 = ((uint8_t*)&_appRomStart) + (canPageNr.vlu * pageSize);
    if (addr.ptr32 > &_appRomEnd) {
      sendErr(msg, C_bootloaderErrStartPageOutOfRange);
      break;
    }
    byte2_t pgCnt;
    pgCnt.b0 = msg->data8[3];
    pgCnt.b1 = msg->data8[4];
    endAddr.vlu = addr.vlu + (pgCnt.vlu * pageSize);
    //endAddr = addr + ((msg->data8[4] << 8 | msg->data8[3]) * pageSize);
    if (endAddr.ptr32 > &_appRomEnd) {
      sendErr(msg, C_bootloaderErrPageLenOutOfRange);
      break;
    }

    // is within appRom boundaries

    // do architecture dependent stuff in system module
    sendErr(msg, systemFlashErase(addr.ptr8, endAddr.ptr8));
  }  break;

  case C_bootloaderStartAddress: {
    msg->DLC = 5;
    //const uint32_t romStart = (uint32_t)&_appRomStart;
    const byte4_t romStart = { (uint32_t)&_appRomStart };
    msg->data8[1] = romStart.b0; //(romStart & 0x000000FF);
    msg->data8[2] = romStart.b1; //(romStart & 0x0000FF00) >> 8;
    msg->data8[3] = romStart.b2; //(romStart & 0x00FF0000) >> 16;
    msg->data8[4] = romStart.b3; //(romStart & 0xFF000000) >> 24;
    CAN_POST_MSG;
  }  break;

  case C_bootloaderMemPageInfo: {
    msg->DLC = 5;
    canPageNr.vlu = ((uint32_t)&_appRomEnd - (uint32_t)&_appRomStart) / pageSize;
    const byte2_t pgSz = { pageSize };
    msg->data8[1] = pgSz.b0; //(pageSize & 0x00FF);
    msg->data8[2] = pgSz.b1; //(pageSize & 0xFF00) >> 8;
    msg->data8[3] = canPageNr.b0; //(canPageNr & 0x00FF);
    msg->data8[4] = canPageNr.b1; //(canPageNr & 0x00FF) >> 8;
    CAN_POST_MSG;
  }  break;

  case C_bootloaderReset:
    systemReset();
    break;
  default:; // bust out
  }
  return true; // return to cmds loop
}

// ------------------------------------------------------------------------
// public functions and variables
bool commandsStart(canframe_t *msg)
{
  bool res =true;
  while(res){
    if (msg == NULL || msg->DLC < 1 ||
        (msg->data8[0] & 0x80) == 0)
      return false;
    res = runCommand(msg);
    print_str("good cmd:");
    print_uint((uint8_t)res + 48);
    print_str("\r\n");
    if (!res)
      break;
    print_str("wait for can\r\n");
    canWaitRecv(msg);
    print_str("got from can\r\n");
  }

  return res;
}


bool commandIsResetFrame(canframe_t *frm)
{
  return (frm->DLC == 1 && frm->data8[0] == C_bootloaderReset);
}

