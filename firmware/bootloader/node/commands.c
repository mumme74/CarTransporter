/*
 * commands.c
 *
 *  Created on: 27 jul 2018
 *      Author: jof
 */


#include "commands.h"
#include "system.h"
#include "crc32.h"
#include "can.h"
#include <can_protocol.h>



#define CAN_POST_MSG  while ((canPost(msg) < 0))        // --""--       txv
#define MIN(x, y) (((x) < (y)) ? (x) : (y))



// _appRomXXX from linkscript
extern const uint32_t _appRomStart, _appRomEnd, _ramStart, _stack;

static void sendErr(can_frame_t *msg, can_bootloaderErrs_e err)
{
  msg->DLC = 2;
  msg->data8[1] = err;
  canWaitSend(msg);
}

static byte4_t addr, endAddr;

// used by both readflash and writeflash
static bool getAndCheckAddress(can_frame_t *msg)
{
  read4bytes(msg, 1, &addr);
  //addr.b0 = msg->data8[4];
  //addr.b1 = msg->data8[3];
  //addr.b2 = msg->data8[2];
  //addr.b3 = msg->data8[1];
  read3bytes(msg, 5, &endAddr);
  //endAddr.b0 = msg->data8[7];
  //endAddr.b1 = msg->data8[6];
  //endAddr.b2 = msg->data8[5];
  endAddr.b3 = 0;
  endAddr.vlu += addr.vlu;
  if (addr.ptr32 < &_appRomStart) {
    sendErr(msg, C_bootloaderErrStartAddressOutOfRange);
    return false; // to commands loop
  } else if (endAddr.ptr32 > &_appRomEnd) {
    sendErr(msg, C_bootloaderErrEndAddressOutOfRange);
    return false; // to commands loop
  }
  return true;
}

static bool runCommand(can_frame_t *msg)
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
    uint32_t bytesDiff = endAddr.ptr8 - addr.ptr8;
    if ((bytesDiff / 7) < (BOOTLOADER_PAGE_SIZE))
      frames = (bytesDiff / 7) + ((bytesDiff % 7) ? 1 : 0);
    else
      frames = BOOTLOADER_PAGE_SIZE;
    //print_str("read frames:");print_uint(frames);print_str(" bytdif");print_uint(bytesDiff);endl();
    //print_str("addr");print_uint(addr.vlu);print_str(" endAddr");print_uint(endAddr.vlu);endl();
    //print_str("crc_len:");print_uint(MIN(endAddr.ptr8 - addr.ptr8, ((BOOTLOADER_PAGE_SIZE+1) * 7)));endl();
    crc.vlu = crc32(0, addr.ptr8, MIN(bytesDiff, ((BOOTLOADER_PAGE_SIZE) * 7)));

    msg->data8[0] = C_bootloaderReadFlash;
    msg->DLC = 1;
    fill4bytes(msg, &crc);
    //msg->data8[1] = crc.b3; // (crc & 0x000000FF);
    //msg->data8[2] = crc.b2; // (crc & 0x0000FF00) >> 8;
    //msg->data8[3] = crc.b1; // (crc & 0x00FF0000) >> 16;
    //msg->data8[4] = crc.b0; // (crc & 0xFF000000) >> 24;
    // len of page
    msg->data8[5] = frames;
    fill2bytes(msg, &canPageNr);
    //msg->data8[6] = canPageNr.b1; //(canPageNr & 0x00FF);
    //msg->data8[7] = canPageNr.b0; //(canPageNr & 0xFF00) >> 8;
    CAN_POST_MSG;
    // inc canPageNr when we know if invoker has crc accepted this frame
    // done below

    // begin payload frames
    while (frameNr < frames) {
      msg->data8[0] = frameNr++;
      for (uint8_t i = 1; i < 8; ++i) {
        msg->data8[i] = *addr.ptr8;
        if (addr.ptr8++ >= endAddr.ptr8) {
          msg->DLC = i;
          break; // frameId should now be frameId == frames
        }
      }
      // slow device interfaces might choke if send speed is to high
      uint32_t waitTo = systemMillis() + 1;
      while(systemMillis() < waitTo);

      CAN_POST_MSG;
    }

    // wait for remote to Ack this page
    //print_str("wait ack adr:");print_uint(addr.vlu);
    do {
      canWaitRecv(msg);
      print_str("got can");endl();
      if (msg->DLC == 1 && msg->data8[0] == C_bootloaderUnblock)
	break; // bust out of this command
      if (msg->DLC != 2 || msg->data8[0] != C_bootloaderReadFlash)
        goto rescan;
      if (msg->IDE != CAN_HOST_ID)
        continue;
    } while(msg->data8[0] != C_bootloaderReadFlash);

    //print_str("after ack adr:");print_uint(addr.vlu);
    //print_str(" eAdr:");print_uint(endAddr.vlu);endl();

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
    msg->DLC = 1;
    fill4bytes(msg, &crc);
    //msg->data8[1] = crc.b3; //(crc & 0x000000FF);
    //msg->data8[2] = crc.b2; //(crc & 0x0000FF00) >> 8;
    //msg->data8[3] = crc.b1; //(crc & 0x00FF0000) >> 16;
    //msg->data8[4] = crc.b0; //(crc & 0xFF000000) >> 24;
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
    canWaitSend(msg);

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
      //print_str("wr rcv\n");
      if ((msg->data8[0] & 0x80)) {
        if (msg->data8[0] != C_bootloaderWriteFlash) {
            print_str("abort wrong header cmd:");print_uint(msg->data8[0]);endl();
          return true; // stuck in loop, we should probably abort
        }

        // get header
        read4bytes(msg, 1, &crc);
        //crc.b0 = msg->data8[4];
        //crc.b1 = msg->data8[3];
        //crc.b2 = msg->data8[2];
        //crc.b3 = msg->data8[1];
        //crc = (msg->data8[4] <<24 | msg->data8[3] << 16 |
        //       msg->data8[2] << 8  | msg->data8[1]);

        frames = msg->data8[5];

        byte2_t pgNr;
        read2bytes(msg, 6, &pgNr);
        //pgNr.b0 = msg->data8[7];
        //pgNr.b1 = msg->data8[6];
        print_str("canPageNr:");print_uint(canPageNr.vlu);
        print_str(" pgNr:");print_uint(pgNr.vlu);endl();
        print_str("crc:");print_uint(crc.vlu);endl();

        //if (canPageNr != (msg->data8[7] << 8 | msg->data8[6])) {
        if (canPageNr.vlu != pgNr.vlu) {
            print_str("pg out order\n");
          sendErr(msg, C_bootloaderErrCanPageOutOfOrder);
          return true; // to commands loop
        }
      } else {
        // it's a payload frame
        uint32_t cPage = canPageNr.vlu * ((BOOTLOADER_PAGE_SIZE) * 7),
                 fOffset = ((msg->data8[0] & 0x7F) * 7);
        uint32_t idxBase = cPage + fOffset;

        /*
        print_str("frameNr:");print_uint(frameNr);
        print_str(" canPage.vlu");print_uint(canPageNr.vlu);
	print_str(" fOffset:");print_uint(fOffset);
	print_str(" msg->DLC");print_uint(msg->DLC);
	print_str(" cPage:");print_uint(cPage);endl();
	print_str(" idx:");print_uint(idxBase);endl();
	print_flush();
	*/


        if (lastStoredIdx < idxBase + msg->DLC -1)
            lastStoredIdx = idxBase + msg->DLC -1;
        for (uint8_t i = 0; i < msg->DLC; ++i) {
          buf[idxBase + i] = msg->data8[i + 1];
        }
        ++frameNr;
      }

    } while (frameNr < frames);

    print_str("got frames\n");
    print_str(" mempages ");print_uint(memPagesWritten);
    print_str(" pgSiz ");print_uint(pageSize);
    print_str(" lastIdx "); print_uint(lastStoredIdx);
    print_str("canPgnr:");print_uint(canPageNr.vlu);endl();
    print_str(" addr ");print_uint(addr.vlu);
    print_str("wr bytes ");print_uint((memPagesWritten * pageSize) + lastStoredIdx);endl();

    // check canPage
    // pointer buf[0] advance to start of canPage.
    // last frame might be less than 7 bytes
    //print_str("len:");print_uint(lastStoredIdx - (canPageNr.vlu * (BOOTLOADER_PAGE_SIZE+1) * 7));endl();
    //print_str("buf idx:");print_uint((canPageNr.vlu * (BOOTLOADER_PAGE_SIZE+1) * 7));endl();
    uint32_t recvCrc = crc32(0, buf + (canPageNr.vlu * (BOOTLOADER_PAGE_SIZE) * 7),
                             lastStoredIdx - (canPageNr.vlu * (BOOTLOADER_PAGE_SIZE) * 7));
    if (crc.vlu != recvCrc) {
	print_str("crc fail:");print_uint(recvCrc);endl();

      sendErr(msg, C_bootloaderErrResend);
      goto writeCanPageLoop;// resend this canPage
    } else {
      ++canPageNr.vlu;
    }


    // if we haven't received a complete pageSize yet
    // and we are not at end of bin
    // (last memPage sent from invoker might be less than pageSize)
    if (lastStoredIdx < pageSize &&
	&addr.ptr8[lastStoredIdx] < endAddr.ptr8)
    {
	print_str("send next\n");
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
    uint8_t *pbuf = &buf[MIN(lastStoredIdx, pageSize) - 1];
    print_str("st idx");print_uint(lastStoredIdx);endl();
    while (++pbuf < (buf + pageSize)) {
      *pbuf = 0x55; // 0xFF
    }

    print_str("bef. flsh\n");print_flush();

    // write flashpage to rom
    err = systemFlashWritePage((uint16_t*)buf, addr.ptr16);
    if (err != C_bootloaderErrOK) {
	print_str("write failed\n");
	print_str(" addr");print_uint((uint32_t)addr.ptr16);endl();
      sendErr(msg, err);
      break;
    } else {
      // advance addr to save in next memorypage next time
      addr.vlu += pageSize;
    }

    // a *.bin might span over many memPages
    if (memPagesWritten < memPages) {
      // notify invoker that we are ready for next frame
	print_str("nxt mempage\n");
      sendErr(msg, C_bootloaderErrOK);
      goto writeMemPageLoop;
    }

    print_str("out");print_uint(err);print_str(" pages:");print_uint(memPages);
    print_str("pages_written:");print_uint(memPagesWritten);endl();
    sendErr(msg, err);

  }  break; // to commands loop

  case C_bootloaderEraseFlash: {
    read2bytes(msg, 1, &canPageNr);
    //canPageNr.b0 = msg->data8[2];
    //canPageNr.b1 = msg->data8[1];
    //canPageNr = (msg->data8[2] >> 8 | msg->data8[1]);
    addr.ptr8 = ((uint8_t*)&_appRomStart) + (canPageNr.vlu * pageSize);
    if (addr.ptr32 > &_appRomEnd) {
      sendErr(msg, C_bootloaderErrStartPageOutOfRange);
      break;
    }
    byte2_t pgCnt;
    read2bytes(msg, 3, &pgCnt);
    //pgCnt.b0 = msg->data8[4];
    //pgCnt.b1 = msg->data8[3];
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
    //const uint32_t romStart = (uint32_t)&_appRomStart;
    const byte4_t romStart = { (uint32_t)&_appRomStart };
    msg->DLC = 1;
    fill4bytes(msg, &romStart);
    //msg->data8[1] = romStart.b3; //(romStart & 0x000000FF);
    //msg->data8[2] = romStart.b2; //(romStart & 0x0000FF00) >> 8;
    //msg->data8[3] = romStart.b1; //(romStart & 0x00FF0000) >> 16;
    //msg->data8[4] = romStart.b0; //(romStart & 0xFF000000) >> 24;
    CAN_POST_MSG;
  }  break;

  case C_bootloaderMemPageInfo: {
    canPageNr.vlu = ((uint32_t)&_appRomEnd - (uint32_t)&_appRomStart) / pageSize;
    const byte2_t pgSz = { pageSize };
    msg->DLC = 1;
    fill2bytes(msg, &canPageNr);
    fill2bytes(msg, &pgSz);
    //msg->data8[3] = pgSz.b1; //(pageSize & 0x00FF);
    //msg->data8[4] = pgSz.b0; //(pageSize & 0xFF00) >> 8;
    //msg->data8[1] = canPageNr.b1; //(canPageNr & 0x00FF);
    //msg->data8[2] = canPageNr.b0; //(canPageNr & 0x00FF) >> 8;
    CAN_POST_MSG;
  }  break;

  case C_bootloaderRamInfo: {
    msg->DLC = 1;
    byte4_t ramStart = { (uint32_t)&_ramStart },
            nrBytes = { (uint32_t)&_stack - (uint32_t)&_ramStart };
    fill4bytes(msg, &ramStart);
    fill3bytes(msg, &nrBytes); // only 24 bits ram space
    CAN_POST_MSG;
  } break;

  case C_bootloaderWait:
    msg->DLC = 2;
    msg->data8[1] = C_bootloaderErrOK;
    CAN_POST_MSG;
    break;
  case C_bootloaderReset:
    systemReset();
    break;
  default:; // bust out
  }
  return true; // return to cmds loop
}

// ------------------------------------------------------------------------
// public functions and variables
bool commandsStart(can_frame_t *msg)
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


bool commandIsResetFrame(can_frame_t *frm)
{
  return (frm->DLC == 1 && frm->data8[0] == C_bootloaderReset);
}

