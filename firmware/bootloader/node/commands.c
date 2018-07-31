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

#define CAN_NEXT_MSG  while ((msg = canGet()) == NULL)  // blocks until rcv
#define CAN_POST_MSG  while ((canPost(msg) < 0))        // --""--       txv
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define FLASH_WAIT while (FLASH_SR & FLASH_SR_BSY)


// _appRomXXX from linkscript
extern const uint8_t *_appRomStart, *_appRomEnd, *_bootRom;

static void sendErr(canframe_t *msg, can_bootloaderErrs_e err)
{
  msg->DLC = 2;
  msg->data8[1] = err;
  CAN_POST_MSG;
}

// used by both readflash and writeflash
static bool getAndCheckAddress(canframe_t *msg, uint8_t *addr, uint8_t *endAddr)
{
  addr = (uint8_t*)((msg->data8[3] << 24) | (msg->data8[2] << 16) |
                    (msg->data8[1] << 8)  | (msg->data8[0])) ;
  endAddr = addr + ((msg->data8[2] << 16) | (msg->data8[1] << 8) |
                  (msg->data8[0]));
  if (addr < _appRomStart) {
    sendErr(msg, C_bootloaderErrStartAddressOutOfRange);
    return false; // to commands loop
  } else if (endAddr > _appRomEnd) {
    sendErr(msg, C_bootloaderErrEndAddressOutOfRange);
    return false; // to commands loop
  }
  return true;
}

static bool runCommand(canframe_t *msg)
{
  static uint32_t crc;
  static uint16_t canPageNr; // as in CAN page
  static uint8_t  frames, frameNr, *addr, *endAddr;

  switch(msg->data8[0]){
  case C_bootloaderReadFlash: {
    if (!getAndCheckAddress(msg, addr, endAddr))
      break;
    canPageNr = 0;

readPageLoop:
    // begin header
    frameNr = 0;
    // concept here is that addr advances in send loop with nr bytes written
    // we will eventually reach endAddr
    // also when reading memory we don't need that 2K memory page restriction that we need when writing to memory
    frames = MIN(endAddr - addr, BOOTLOADER_PAGE_SIZE) / 7 +
             MIN(endAddr - addr, BOOTLOADER_PAGE_SIZE) % 7;

    crc = crc32(0, addr, MIN(endAddr - addr, BOOTLOADER_PAGE_SIZE));
    msg->DLC = 8;
    msg->data8[0] = C_bootloaderReadFlash;
    msg->data8[1] = (crc & 0x000000FF);
    msg->data8[2] = (crc & 0x0000FF00) >> 8;
    msg->data8[3] = (crc & 0x00FF0000) >> 16;
    msg->data8[4] = (crc & 0xFF000000) >> 24;
    // len of page
    msg->data8[5] = frames;
    msg->data8[6] = (canPageNr & 0x0000FF00) >> 8;
    msg->data8[7] = (canPageNr & 0x00FF0000) >> 16;
    CAN_POST_MSG;
    ++canPageNr;

    // begin payload frames
    while (frameNr < frames) {
      msg->data8[0] = frameNr++;
      for (uint8_t i = 1; i < 8; ++i) {
        msg->data8[i] = *addr;
        if (++addr >= endAddr)
          break; // frameId should now be frameId == frames
      }
      CAN_POST_MSG;
    }

    // wait for remote to Ack this page
    do {
      CAN_NEXT_MSG;
    } while(msg->DLC != 2 && msg->data8[0] != C_bootloaderReadFlash);

    // check if we should resend
    if (msg->data8[1] == C_bootloaderErrOK)
      ++canPageNr;
    if (addr >= endAddr)
      return true; // finished!, to commands loop
    goto readPageLoop;
  }  break;

  case C_bootloaderChecksum: {
    if (!getAndCheckAddress(msg, addr, endAddr))
      break;
    crc = crc32(0, addr, endAddr - addr);
    msg->DLC = 5;
    msg->data8[1] = (crc & 0x000000FF);
    msg->data8[2] = (crc & 0x0000FF00) >> 8;
    msg->data8[3] = (crc & 0x00FF0000) >> 16;
    msg->data8[4] = (crc & 0xFF000000) >> 24;
    CAN_POST_MSG; // if buffer full we bust out to main
  }  break;

  case C_bootloaderWriteFlash: {
    if (!getAndCheckAddress(msg, addr, endAddr))
      break;
    uint16_t memPages = ((endAddr - addr) / pageSize) +
                        ((endAddr - addr) % pageSize);

    uint16_t memPagesWritten = 0;

    // initial state is tell invoker we are ready to recieve
    msg->data8[1] = C_bootloaderErrOK;
    msg->DLC = 2;
    CAN_POST_MSG;

    uint8_t buf[pageSize];
    can_bootloaderErrs_e err;

writeMemPageLoop:
    canPageNr = 0;

writeCanPageLoop:
    frameNr = frames = 0;

    // loop to receive a complete canPage
    do {
      CAN_NEXT_MSG;
      if ((msg->data8[0] & 0x80) && frames == 0) {
        // get header
        crc = (msg->data8[4] >> 24 | msg->data8[3] >> 16 |
               msg->data8[2] >> 8  | msg->data8[1]);
        frames = msg->data8[5];
        if (canPageNr != (msg->data8[7] << 8 | msg->data8[6])) {
          sendErr(msg, C_bootloaderErrCanPageOutOfOrder);
          return true; // to commands loop
        }
      } else {
        // it's a payload frame
        for (uint8_t i = 1; i < 8; ++i) {
          if (msg->DLC == i)
            break; // frameNr == frames should be true now
          buf[(canPageNr * 0x7F) + (msg->data8[0] & 0x7F) + i] = msg->data8[i];
        }
      }

      ++frameNr;
    } while (frameNr < frames);

    // check canPage
    // pointer buf[0] advance to start of canPage.
    // last frame might be less than 7 bytes
    if (!crc32(0, buf + (canPageNr * 0x7F), (frames * 7) - (7 + msg->DLC))) {
      sendErr(msg, C_bootloaderErrResend);
      goto writeCanPageLoop;// resend this canPage
    } else {
      ++canPageNr;
    }

    // notify invoker that we are ready for next frame
    sendErr(msg, C_bootloaderErrOK);

    // if we haven't received a complete pageSize yet
    // and we are not at end of bin
    // (last memPage sent from invoker might be less than pageSize)
    if ((canPageNr + frames) < pageSize &&
        memPagesWritten + 1 < memPages)
    {
      goto writeCanPageLoop;
    }

    // here we are at the last canPage of this memPage
    // if we're also at the last memPage we should also reset bytes
    // up to end of memPage (fill with 0xFF)

    // setup a pointer to our buffer with offset to end of just received +1
    //  (msg->DLC is +1 due to data8[0] is frameid)
    uint8_t *pbuf = buf + (((canPageNr -1) * 0x7F) * 7) + (frames * 7) -  (7 - msg->DLC) +1;
    while (pbuf++ < (buf + pageSize))
      *pbuf = 0xFF;

    // write flashpage to rom
    err = systemFlashWritePage((uint16_t*)buf, (uint16_t*)addr);
    if (err != C_bootloaderErrOK) {
      sendErr(msg, err);
      break;
    }

    // a *.bin might span over many memPages
    if (++memPagesWritten < memPages)
      goto writeMemPageLoop;

    sendErr(msg, err);

  }  break; // to commands loop

  case C_bootloaderEraseFlash: {
    canPageNr = (msg->data8[2] >> 8 | msg->data8[1]) +
                ((_appRomStart - _bootRom) / pageSize);
    addr = (uint8_t*)_appRomStart + (canPageNr * pageSize);
    if (addr > _appRomEnd) {
      sendErr(msg, C_bootloaderErrStartPageOutOfRange);
      break;
    }
    endAddr = addr + ((msg->data8[4] << 8 | msg->data8[3]) * pageSize);
    if (endAddr > _appRomEnd) {
      sendErr(msg, C_bootloaderErrPageLenOutOfRange);
      break;
    }

    // is within appRom boundaries

    // do architecture dependent stuff in system module
    sendErr(msg, systemFlashErase(addr, endAddr));
  }  break;

  case C_bootloaderStartAddress: {
    msg->DLC = 5;
    uint32_t stAdr = (uint32_t)(_appRomStart);
    msg->data8[1] = (stAdr & 0x000000FF);
    msg->data8[2] = (stAdr & 0x0000FF00) >> 8;
    msg->data8[3] = (stAdr & 0x00FF0000) >> 16;
    msg->data8[4] = (stAdr & 0xFF000000) >> 24;
    CAN_POST_MSG;
  }  break;

  case C_bootloaderMemPageInfo: {
    msg->DLC = 5;
    canPageNr = (_appRomEnd - _appRomStart) / pageSize;
    msg->data8[1] = (pageSize & 0x00FF);
    msg->data8[2] = (pageSize & 0xFF00) >> 8;
    msg->data8[3] = (canPageNr & 0x00FF);
    msg->data8[4] = (canPageNr & 0x00FF) >> 8;
  }  break;

  case C_bootloaderReset:
    systemReset();
    while(1); // wait for reset
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
    // TODO assure HSI is enabled?
    res = runCommand(msg);
    if (!res)
      break;
    CAN_NEXT_MSG;
  }

  return res;
}


