/*
 * system_teensy3.c
 *
 *  Created on: 2 jul 2019
 *      Author: jof
 */

#include "system.h"
#include "can.h"
#include "commands.h"
#include <Arduino.h>
#include <kinetis.h>

#ifdef ARDUINO
# include <Arduino.h>
# ifdef DEBUG_PRINT
#  include <usb_serial.h>
  void print_str(const char *str) {
    uint8_t i =0;
    for(; i < 30 && str[i] != 0; ++i) ;
    usb_serial_write(str, i);
    usb_serial_flush_output();
  }
  void endl() {
    print_str("\r\n");
    yield();
    usb_serial_flush_output();
  }

  void print_uint(uint32_t vlu) {
    for (int i = 7; i >= 0; --i) {
	uint8_t c = (vlu & (0xF << (i * 4))) >> (i * 4);
	if (c > 9) c += 7; // to get to alphabet
	else if(c == 0 && i > 0) continue;
	usb_serial_putchar(c + 48);
    }
    usb_serial_flush_output();
  }
# endif
#endif


// from https://github.com/alex-Arc/FirmwareFlasher/blob/master/FirmwareFlasher.h
#define RAMFUNC  __attribute__ ((section(".fastrun"), noinline, noclone, optimize("Os") ))

// from linkscript
extern const size_t _appRomStart, _appRomEnd, _bootRom, _stack, _heapstart;

// from 4.3.4
// http://infocenter.arm.com/help/topic/com.arm.doc.dui0553b/DUI0553.pdf
#define SCB_VTOR_TBLOFF_LSB     7
#define SCB_VTOR_TBLOFF         (0x1FFFFFF << SCB_VTOR_TBLOFF_LSB)



// --------------- begin private functions ------------------------

bool flash_is_idle() {
  return FTFL_FSTAT & FTFL_FSTAT_CCIF;
}

// from 27.4.2 Flash Bank 0 Control Register
static const uint32_t FMC_PFB0CR_CINV_WAY = 0x0Fu << 20;

RAMFUNC void flash_invalidate_cache() {
  __disable_irq();
  FMC_PFB0CR |= FMC_PFB0CR_CINV_WAY;
  __enable_irq();
}


void flash_wait_for_last_operation() {
  while(!flash_is_idle())
    ;
}

void flash_err_clr() {
  FTFL_FSTAT &= FTFL_FSTAT_RDCOLERR |
                FTFL_FSTAT_ACCERR | FTFL_FSTAT_FPVIOL;
  flash_wait_for_last_operation();
}

bool flash_has_error() {
  if (flash_is_idle())
    return (FTFL_FSTAT & FTFL_FSTAT_MGSTAT0);
  return false;
}

bool flash_run_cmd() {
  FTFL_FSTAT |= FTFL_FSTAT_CCIF;
  flash_wait_for_last_operation();
  return 0 != (FTFL_FSTAT & (FTFL_FSTAT_RDCOLERR | FTFL_FSTAT_ACCERR |
                             FTFL_FSTAT_FPVIOL   | FTFL_FSTAT_MGSTAT0));
}

// -------------- Begin exported symbols ---------------------------

#if NODE_ID==CAN_PARKBRAKE_NODE
const uint16_t canId = CAN_MSG_TYPE_COMMAND | C_parkbrakeCmdBootloader | C_parkbrakeNode;
#elif NODE_ID==CAN_SUSPENSION_NODE
const uint16_t canId = CAN_MSG_TYPE_COMMAND | C_suspensionCmdBootloader | C_suspensionNode;
#endif


const uint16_t pageSize = 2048; // 2kb

volatile uint32_t globalMsSinceStartup;

// ------------- Begin public functions ------------------------------

void systemInit(void) {
  //Serial.begin(115200); // actually does nothing in teensy
  globalMsSinceStartup = -1; // invalidate

  // FIXME ONLY for debuging
  pinMode(6, OUTPUT);
}

void systemDeinit(void) {
  //Serial.end(); // actually does nothing in teensy
}

// abstraction to make code portable
uint32_t systemMillis(void) {
  return millis();
}



void systickInit(void) {} /* stub */
void systickShutdown(void) {} /* stub */

void systemToApplication(void)
{
  typedef void (*funcptr_t)(void);

  /* variable that will be loaded with the start address of the application */
  size_t  *jumpAddress;
  const size_t* applicationAddress = &_appRomStart;

  // safetycheck, if hang in bootloader mode if we dont have a valid start of vector table
  if (applicationAddress[0] > _stack +4 ||
      applicationAddress[0] < _heapstart + 100)
  {
    canframe_t msg;
    canInitFrame(&msg, canId);
    systemInit(); // set up clock
    canInit();// need to reactivate
    while (1) {
      msg.DLC = 2;
      msg.data8[0] = C_bootloaderReset;
      msg.data8[1] = C_bootloaderErrNonValidBin;
      canPost(&msg);
      msg.data8[0] = C_bootloaderWait;
      msg.DLC = 1;
      canPost(&msg);
      commandsStart(&msg);
    }
  }
  print_str("after");


  /* get jump address from application vector table */
  jumpAddress = (size_t*) applicationAddress[1];

  /* load this address into function pointer */
  funcptr_t start_func = (funcptr_t)jumpAddress;

  /* Disable all interrupts */
  for(int i = 0; i < NVIC_NUM_INTERRUPTS; ++i){
    NVIC_CLEAR_PENDING(i);
    NVIC_DISABLE_IRQ(i);
  }

  // relocate vectortable to app offset vectortable is now offset to appRom
  //
  SCB_VTOR = SCB_VTOR_TBLOFF & ((_appRomStart - _bootRom) << SCB_VTOR_TBLOFF_LSB);

  // 14.4.3.1 RUN mode
  // The processor reads the start SP (SP_main) from vector-table offset 0x000
  // The processor reads the start PC from vector-table offset 0x004
  // LR is set to 0xFFFF_FFFF.

  /* set stack pointer as in application's vector table */
  // change both Mainstackpointer and PriorityStackpointer
  asm("msr msp, %[address]" : : [address] "r" (applicationAddress));
  asm("msr psp, %[address]" : : [address] "r" (applicationAddress));
  // asm("msr lr, %[address]" : : [address] "r" (0xFFFFFFFF));

  /* call our application startup */
  start_func();
}

void systemReset() {
  // from arm documentation, page 4-17
  //http://infocenter.arm.com/help/topic/com.arm.doc.dui0553b/DUI0553.pdf
  //http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0552a/Cihehdge.html
  // and https://mcuoneclipse.com/2015/07/01/how-to-reset-an-arm-cortex-m-with-software/
  // 6.2.2.8 in kinetis K20 manual
  SCB_AIRCR = (0x5FA << 16) | (1 << 2);

  // wait forever until reset is done in hardware
  for(;;)
    ;
}


can_bootloaderErrs_e systemFlashErase(uint8_t *startAddr, uint8_t *endAddr)
{
  if (startAddr < (uint8_t*)_appRomStart)
    return C_bootloaderErrStartAddressOutOfRange;
  if (endAddr > (uint8_t*)_appRomEnd)
    return C_bootloaderErrEndAddressOutOfRange;

  can_bootloaderErrs_e err = C_bootloaderErrOK;
//
//  // figure out how to unprotect flash memory for these sectors
//  uint32_t totalRomSize = _appRomEnd - _bootRom; // 262144 ie 256kb
//  uint32_t protectRegionSize = totalRomSize / 32;
//  uint32_t protectByteRegions = protectRegionSize / 8;


  do {
//    // make sure sector is unprotected
//    flash_wait_for_last_operation(); // can't change FPROT registers while other cmds running
//    // figure out which byte that handles this region
//    uint32_t curRegion = (startAddr - _bootRom) / protectRegionSize;
//    uint32_t curRegionByte = curRegion / 8;
//    volatile uint8_t *protectionByte = nullptr;
//    switch(curRegionByte) {
//    case 0: protectionByte = FTFL_FPROT3; break;
//    case 1: protectionByte = FTFL_FPROT2; break;
//    case 2: protectionByte = FTFL_FPROT1; break;
//    case 3: protectionByte = FTFL_FPROT0; break;
//    default:
//      Serial.printf("unhandled protection curRegionByte:%l\n", curRegionByte);
//      return C_bootloaderErrPageEraseFailed;
//    }
//
//    // figure out which bit in this byte to twiddle
//    protectionByte |=

    // erase sector cmd
    FTFL_FCCOB0 = 0x09; // (ERSSCR) 28.4.11.7 Erase Flash Sector Command
    FTFL_FCCOB3 = ((uint32_t)startAddr & 0x00FF0000u) >> 16;
    FTFL_FCCOB4 = ((uint32_t)startAddr & 0x0000FF00u) >> 8;
    FTFL_FCCOB5 = ((uint32_t)startAddr & 0x000000FFu) >> 0;

    // Run cmd
    if (!flash_run_cmd()) {
      err = C_bootloaderErrPageEraseFailed;
      break;
    }

    startAddr += pageSize; // set next page
  } while (startAddr < endAddr);

  flash_invalidate_cache();

  return err;
}

can_bootloaderErrs_e systemFlashWritePage(uint16_t *memPageBuf,
                                          volatile uint16_t *addr)
{

  if (addr < (uint16_t*)_appRomStart)
    return C_bootloaderErrStartAddressOutOfRange;

  can_bootloaderErrs_e err = C_bootloaderErrOK;



  // check that FlexRAM is in RAM mode not EEPROM
  flash_wait_for_last_operation();
  bool isEEPROM = FTFL_FCNFG & FTFL_FCNFG_EEERDY;
  if (isEEPROM) {
    // set to RAM
    FTFL_FCCOB0 = 0x81; // (SETRAM) 28.4.11.15 Set FlexRAM Function Command
    FTFL_FCCOB1 = 0xFF; // Make FlexRAM available as RAM:
    flash_wait_for_last_operation();
  }

  // erase this memorypage before we write new stuff
  // we must do this as flash write requires 0xffff in address to write
  // erase sector cmd
  FTFL_FCCOB0 = 0x09; // (ERSSCR) 28.4.11.7 Erase Flash Sector Command
  FTFL_FCCOB1 = ((uint32_t)addr & 0x00FF0000u) >> 16; // address of sector to erase
  FTFL_FCCOB2 = ((uint32_t)addr & 0x0000FF00u) >> 8;
  FTFL_FCCOB3 = ((uint32_t)addr & 0x000000FFu) >> 0;
  FTFL_FSTAT |= FTFL_FSTAT_CCIF; // do cmd, continue while it does its thing

  // fill FlexRAM with our payload

  // write in phrases (64bits), cast to phrase
  uint64_t *memBuf = (uint64_t*)memPageBuf;
  uint32_t *sectorAddr = (uint32_t*)addr;


  // first some boundaries
  const uint64_t *flexRamStart = (uint64_t*)0x14000000u, // from 3.5.1.2
                 //*flexRamEnd   = (uint64_t*)0x140007FFu,
                 *flexRam2half = (uint64_t*)0x14000400u;

  // we can't store more than 1kb at a time in flexRam, sequential writes
  do {

    // pointer to flexram
    volatile uint64_t *flexRamBuf = (uint64_t*)flexRamStart;

    // FIXME this assumes a 2kb FlexRAM conf
    // and we can only write to lower 1/2 of flexRAM

    uint16_t phrasesWritten = 0;
    // do 1kb each and sequential writes
    do {

      *flexRamBuf = *memBuf; // write to rom

      // checks for write errors (addr is volatile)
      if (*flexRamBuf != *memBuf) {
        err = C_bootloaderErrPageWriteFailed;
        // quit writing sequence and restore
        break;
      }

      ++phrasesWritten;
      ++flexRamBuf;
    } while (++memBuf < flexRam2half);

    // erase cmd might not be finished yet
    flash_wait_for_last_operation();

    if (flash_has_error()) {
      err = C_bootloaderErrPageWriteFailed;
      //break;
    } else {
      FTFL_FCCOB0 = 0x0B; // (PGMSEC) 28.4.11.8 Program Section Command
      FTFL_FCCOB1 = ((uint32_t)sectorAddr & 0x00FF0000u) >> 16; // address of sector to program
      FTFL_FCCOB2 = ((uint32_t)sectorAddr & 0x0000FF00u) >> 8;
      FTFL_FCCOB3 = ((uint32_t)sectorAddr & 0x000000FFu) >> 0;
      // number of phrases to store
      // a phrase is 64bits aligned [2:0]
      FTFL_FCCOB4 = (phrasesWritten & 0xFF00u) >> 8; // length stored in FlexRAM
      FTFL_FCCOB5 = (phrasesWritten & 0x00FFu) >> 0;
      FTFL_FSTAT |= FTFL_FSTAT_CCIF; // run cmd
    }

    // advance sector storeAddress
    sectorAddr += flexRam2half - flexRamStart;
  } while(err == C_bootloaderErrOK && memBuf < (uint64_t*)(addr + pageSize));

  // cleanup and sanity checks

  flash_wait_for_last_operation();
  if (flash_has_error())
    err = C_bootloaderErrPageWriteFailed;

  if (isEEPROM) {
    // restore to EEPROM
    flash_wait_for_last_operation();
    FTFL_FCCOB0 = 0x81; // (SETRAM) 28.4.11.15 Set FlexRAM Function Command
    FTFL_FCCOB1 = 0x00; // Make FlexRAM available as EEPROM:
    flash_run_cmd();
  }

  // program cache must be invalidated
  flash_invalidate_cache();

  return err;
}

