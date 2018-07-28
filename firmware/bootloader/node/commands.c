/*
 * commands.c
 *
 *  Created on: 27 jul 2018
 *      Author: jof
 */

#include "commands.h"
#include "system.h"
#include <can_protocol.h>

#define CAN_NEXT_MSG  while ((msg = canGet()) == NULL)  // blocks until rcv
#define CAN_POST_MSG  while ((canPost(msg) < 0))        // --""--       txv
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define FLASH_WAIT while (FLASH_SR & FLASH_SR_BSY)


// _appRomXXX from linkscript
extern const uint8_t *_appRomStart, *_appRomEnd, *_bootRom;

extern uint32t *sys_tick_handler;

static void sendErr(canframe_t *msg, can_bootloaderErrs_e err)
{
  msg->DLC = 2;
  msg->data8[1] = err;
  CAN_POST_MSG;
}


// this function is grabbed from https://github.com/cvra/CRC/blob/56cc8e841085bc3cca5b0f608c85841a2b135677/crc32.c
static uint32_t crc32(uint32_t init, const void *data, size_t length)
{
    // static to save some stackspace,
    // not sure if its needed due to non recursive, but bss (better save space)...
    static uint8_t *p;
    static uint32_t i, crc, bit;

    p = (uint8_t *)data;
    crc = ~init;
    for (i = 0; i < length; i++) {
        for (bit = 0; bit < 8; bit++) {
            if ((crc & 1) != ((p[i] >> bit) & 1)) {
                crc = (crc >> 1) ^ BOOTLOADER_CRC32_POLYNOMIAL;
            } else {
                crc = crc >> 1;
            }
        }
    }
    return ~crc;
}

// used by both readflash and writeflash
static bool getAndCheckAddress(canframe_t *msg, uint8_t *adr, uint8_t *endAdr)
{
  adr  = (uint8_t*)((msg->data8[3] << 24) | (msg->data8[2] << 16) |
                    (msg->data8[1] << 8)  | (msg->data8[0])) ;
  endAdr = adr + ((msg->data8[2] << 16) | (msg->data8[1] << 8) |
                  (msg->data8[0]));
  if (adr < _appRomStart) {
    sendErr(msg, C_bootloaderErrStartAdressOutOfRange);
    return false; // to commands loop
  } else if (endAdr > _appRomEnd) {
    sendErr(msg, C_bootloaderErrEndAdressOutOfRange);
    return false; // to commands loop
  }
  return true;
}

static bool runCommand(canframe_t *msg)
{
  static uint32_t crc;
  static uint16_t canPageNr; // as in CAN page
  static uint8_t  frames, frameNr, *adr, *endAdr;

  switch(msg->data8[0]){
  case C_bootloaderReadFlash: {
    if (!getAndCheckAddress(msg, adr, endAdr))
      break;
    canPageNr = 0;

readPageLoop:
    // begin header
    frameNr = 0;
    frames = MIN(endAdr - adr, BOOTLOADER_PAGE_SIZE) / 7 +
             MIN(endAdr - adr, BOOTLOADER_PAGE_SIZE) % 7;
    crc = crc32(0, adr, MIN(endAdr - adr, BOOTLOADER_PAGE_SIZE));
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
        msg->data8[i] = *adr;
        if (adr++ >= endAdr)
          break; // frameId should now be frameId == frames
      }
      CAN_POST_MSG;
    }

    // wait for remote to Ack this page
    do {
      CAN_NEXT_MSG;
    } while(msg->DLC != 2 && msg->data8[0] != C_bootloaderReadFlash);

    // check if we should resend
    if (msg->data8[1] == 0xAA)
      ++canPageNr;
    if (adr >= endAdr)
      return true; // finished!, to commands loop
    goto readPageLoop;
  }  break;

  case C_bootloaderChecksum: {
    if (!getAndCheckAddress(msg, adr, endAdr))
      break;
    crc = crc32(0, adr, endAdr - adr);
    msg->DLC = 5;
    msg->data8[1] = (crc & 0x000000FF);
    msg->data8[2] = (crc & 0x0000FF00) >> 8;
    msg->data8[3] = (crc & 0x00FF0000) >> 16;
    msg->data8[4] = (crc & 0xFF000000) >> 24;
    CAN_POST_MSG; // if buffer full we bust out to main
  }  break;

  case C_bootloaderWriteFlash: {
    if (!getAndCheckAddress(msg, adr, endAdr))
      break;
    uint16_t memPages = ((endAdr - adr) / pageSize) +
                        ((endAdr - adr) % pageSize);

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
    ++canPageNr;

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
          buf[canPageNr + (msg->data8[0] & 0x7F) + i] = msg->data8[i];
        }
      }

      ++frameNr;
    } while (frameNr < frames);

    // check canPage
    // pointer buf[0] advance to start of canPage.
    // last frame might be less than 7 bytes
    if (!crc32(0, buf + canPageNr, (frames * 7) - (7 + msg->DLC))) {
      sendErr(msg, C_bootloaderErrResend);
      --canPageNr;
      goto writeCanPageLoop;
    }


    // if we haven't received a complete pageSize yet
    // and we are not at end of bin
    // (last memPage sent from invoker might be less than pageSize)
    if ((canPageNr + frames) < pageSize)
      goto writeCanPageLoop;

    // here we are at the last canPage of this memPage
    // if we're also at the last memPage we should also reset bytes
    // up to end of memPage (fill with 0xFF)

    // setup a pointer to our buffer with offset to end of just received +1
    //  (msg->DLC is +1 due to data8[0] is frameid)
    uint8_t *pbuf = buf + canPageNr + (msg->data8[0] & 0x7F) + msg->DLC;
    while (pbuf++ < (buf + pageSize))
      *pbuf = 0xFF;

    // write flashpage to rom
    err = systemFlashWritePage((uint16_t*)buf, (uint16_t*)adr);
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
    adr = (uint8_t*)_appRomStart + (canPageNr * pageSize);
    if (adr > _appRomEnd) {
      sendErr(msg, C_bootloaderErrStartPageOutOfRange);
      break;
    }
    endAdr = adr + ((msg->data8[4] << 8 | msg->data8[3]) * pageSize);
    if (endAdr > _appRomEnd) {
      sendErr(msg, C_bootloaderErrPageLenOutOfRange);
      break;
    }

    // is within appRom boundaries

    // do architecture dependent stuff in system module
    sendErr(msg, systemFlashErase(adr, endAdr));
  }  break;

  case C_bootloaderStartAddress: {
    msg->DLC = 2;
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
    if (msg->DLC < 1 || (msg->data8[0] & 0x80) == 0)
      return false;
    // TODO assure HSI is enabled?
    res = runCommand(msg);
    if (!res)
      break;
    CAN_NEXT_MSG;
  }

  return res;
}


