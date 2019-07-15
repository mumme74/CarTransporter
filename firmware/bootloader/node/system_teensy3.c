/*
 * system_teensy3.c
 *
 *  Created on: 2 jul 2019
 *      Author: jof
 */

#include <Arduino.h>
#include <kinetis.h>
#include "system.h"
#include "can.h"
#include "commands.h"

#define FLTL_FCCOB_PGMSEC 0x0B
#define FLTL_FCCOB_ERSSCR 0x09
#define FTFL_FCCOB_SETRAM 0x81
#define FMC_PFB0CR_CINV_WAY (0x0Fu << 20);


// from https://github.com/alex-Arc/FirmwareFlasher/blob/master/FirmwareFlasher.h
#define RAMFUNC  __attribute__ ((section(".fastrun"), noinline, noclone, optimize("Os") ))


#ifdef ARDUINO
# include <Arduino.h>
# ifdef DEBUG_PRINT
#  include <usb_serial.h>
  void print_str(const char *str) {
    uint8_t i =0;
    for(; i < 30 && str[i] != 0; ++i) ;
    usb_serial_write(str, i);
    //usb_serial_flush_output();
  }
  void endl() {
    print_str("\r\n");
    yield();
    //usb_serial_flush_output();
  }

  void print_uint(uint32_t vlu) {
    int begin = 1;
    for (int i = 7; i >= 0; --i) {
	uint8_t c = (vlu & (0xF << (i * 4))) >> (i * 4);
	if (c > 9) c += 7; // to get to alphabet
	else if(c == 0 && i > 0 && begin) continue;
	begin = 0;
	usb_serial_putchar(c + 48);
    }
    //usb_serial_flush_output();
  }
  void print_flush() {
    usb_serial_flush_output();
  }

  int print_available() {
    return usb_serial_available();
  }
# endif
#endif



// from linkscript
extern const size_t _appRomStart, _appRomEnd, _bootRom, _stack, _heapstart;

// from 4.3.4
// http://infocenter.arm.com/help/topic/com.arm.doc.dui0553b/DUI0553.pdf
#define SCB_VTOR_TBLOFF_LSB     7
#define SCB_VTOR_TBLOFF         (0x1FFFFFF << SCB_VTOR_TBLOFF_LSB)



// --------------- begin private functions ------------------------
// from 27.4.2 Flash Bank 0 Control Register
/*
RAMFUNC bool flash_is_idle() {
  return FTFL_FSTAT & FTFL_FSTAT_CCIF;
}


RAMFUNC void flash_invalidate_cache() {
  __disable_irq();
  FMC_PFB0CR |= FMC_PFB0CR_CINV_WAY;
  __enable_irq();
}


RAMFUNC void flash_wait_for_last_operation() {
  while((FTFL_FSTAT & FTFL_FSTAT_CCIF) == 0)
    ;
}

RAMFUNC void flash_err_clr() {
  FTFL_FSTAT &= FTFL_FSTAT_RDCOLERR |
                FTFL_FSTAT_ACCERR | FTFL_FSTAT_FPVIOL;
  flash_wait_for_last_operation();
}

RAMFUNC bool flash_has_error() {
  if (flash_is_idle())
    return (FTFL_FSTAT & FTFL_FSTAT_MGSTAT0);
  return false;
}

RAMFUNC bool flash_run_cmd() {
  FTFL_FSTAT |= FTFL_FSTAT_CCIF;
  flash_wait_for_last_operation();
  return 0 != (FTFL_FSTAT & (FTFL_FSTAT_RDCOLERR | FTFL_FSTAT_ACCERR |
                             FTFL_FSTAT_FPVIOL   | FTFL_FSTAT_MGSTAT0));
}
*/

// -------------- Begin exported symbols ---------------------------

#if NODE_ID==CAN_PARKBRAKE_NODE
const uint16_t canId = CAN_MSG_TYPE_COMMAND | C_parkbrakeCmdBootloader | C_parkbrakeNode;
#elif NODE_ID==CAN_SUSPENSION_NODE
const uint16_t canId = CAN_MSG_TYPE_COMMAND | C_suspensionCmdBootloader | C_suspensionNode;
#endif


const uint16_t pageSize = 2048; // 2kb

void hard_fault_isr(void) {
  digitalWrite(13, !digitalRead(13));
}

// ------------- Begin public functions ------------------------------

void systemInit(void) {
  //Serial.begin(115200); // actually does nothing in teensy
  delay(50); // allow usb to settle

  /*
  // FIXME ONLY for debuging
  pinMode(6, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  // hard fault
  pinMode(13, OUTPUT);
  */
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

    /*
    // for debug make sure it transmits in correct byte order
    canInitFrame(&msg, canId);
    for(int i = 0; i < 8; ++i)
      msg.data8[msg.DLC++] = i;
    canPost(&msg);
    // end debug
    */

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
  for(;;);
}


RAMFUNC can_bootloaderErrs_e systemFlashErase(uint8_t *startAddr, uint8_t *endAddr)
{
  if (startAddr < (uint8_t*)&_appRomStart)
    return C_bootloaderErrStartAddressOutOfRange;
  if (endAddr > (uint8_t*)&_appRomEnd)
    return C_bootloaderErrEndAddressOutOfRange;

  can_bootloaderErrs_e err = C_bootloaderErrOK;

  __disable_irq();
//
//  // figure out how to unprotect flash memory for these sectors
//  uint32_t totalRomSize = _appRomEnd - _bootRom; // 262144 ie 256kb
//  uint32_t protectRegionSize = totalRomSize / 32;
//  uint32_t protectByteRegions = protectRegionSize / 8;

  // no old commands working on memory
  while((FTFL_FSTAT & FTFL_FSTAT_CCIF) == 0);

  do {
    // erase sector cmd
    FTFL_FCCOB0 = FLTL_FCCOB_ERSSCR; // (ERSSCR) 28.4.11.7 Erase Flash Sector Command
    FTFL_FCCOB1 = ((uint32_t)startAddr & 0x00FF0000u) >> 16; // address of sector to erase
    FTFL_FCCOB2 = ((uint32_t)startAddr & 0x0000FF00u) >> 8;
    FTFL_FCCOB3 = ((uint32_t)startAddr & 0x000000FFu) >> 0;

    // run command
    FTFL_FSTAT = FTFL_FSTAT_CCIF;
    while((FTFL_FSTAT & FTFL_FSTAT_CCIF) == 0);

    // Test for errors
    if (FTFL_FSTAT & (FTFL_FSTAT_RDCOLERR | FTFL_FSTAT_ACCERR |
		       FTFL_FSTAT_FPVIOL   | FTFL_FSTAT_MGSTAT0))
    {
      err = C_bootloaderErrPageEraseFailed;
      break;
    }

    startAddr += pageSize; // set next page
  } while (startAddr < endAddr);

  //flash_invalidate_cache();
  FMC_PFB0CR |= FMC_PFB0CR_CINV_WAY
  while((FTFL_FSTAT & FTFL_FSTAT_CCIF) == 0);

  __enable_irq();

  return err;
}

//RAMFUNC can_bootloaderErrs_e _systemFlashWritePage(uint16_t *memPageBuf,
//                                          volatile uint16_t *addr);
RAMFUNC can_bootloaderErrs_e systemFlashWritePage(uint16_t *memPageBuf,
                                          volatile uint16_t *addr)
{
  //print_str("romstart");print_uint((uint32_t)_appRomStart);endl();print_flush();
  if (addr < (uint16_t*)&_appRomStart)
    return C_bootloaderErrStartAddressOutOfRange;

  can_bootloaderErrs_e err = C_bootloaderErrOK;

  __disable_irq();

  // check that FlexRAM is in RAM mode not EEPROM
  //flash_wait_for_last_operation();
  while((FTFL_FSTAT & FTFL_FSTAT_CCIF) == 0);

      // We can't use print_* here in this function
     // print_str("wr flsh 1kb\n");print_flush();

  bool isEEPROM = FTFL_FCNFG & FTFL_FCNFG_EEERDY;
  if (isEEPROM) {
    // set to RAM
    FTFL_FCCOB0 = 0x81; // (SETRAM) 28.4.11.15 Set FlexRAM Function Command
    FTFL_FCCOB1 = 0xFF; // Make FlexRAM available as RAM:
    //flash_wait_for_last_operation();
    FTFL_FSTAT |= FTFL_FSTAT_CCIF;
    while((FTFL_FSTAT & FTFL_FSTAT_CCIF) == 0);
  }


  // erase this memorypage before we write new stuff
  // we must do this as flash write requires 0xffff in address to write
  // erase sector cmd
  FTFL_FCCOB0 = FLTL_FCCOB_ERSSCR; // (ERSSCR) 28.4.11.7 Erase Flash Sector Command
  FTFL_FCCOB1 = ((uint32_t)addr & 0x00FF0000u) >> 16; // address of sector to erase
  FTFL_FCCOB2 = ((uint32_t)addr & 0x0000FF00u) >> 8;
  FTFL_FCCOB3 = ((uint32_t)addr & 0x000000FFu) >> 0;
  FTFL_FSTAT |= FTFL_FSTAT_CCIF;  // do cmd, continue while it does its thing
  //while((FTFL_FSTAT & FTFL_FSTAT_CCIF) == 0);
  // fill FlexRAM with our payload

  // write in phrases (64bits), cast to phrase
  uint64_t *memBuf = (uint64_t*)memPageBuf;
  uint32_t *sectorAddr = (uint32_t*)addr;


  // first some boundaries
  const uint64_t *flexRamStart = (uint64_t*)0x14000000u, // from 3.5.1.2
                 *flexRam2half = (uint64_t*)0x14000400u;
                 //*flexRamEnd   = (uint64_t*)0x140007FFu;
  uint8_t sectors = 2;

  // pointer to flexram
  volatile uint64_t *flexRamBuf = (uint64_t*)flexRamStart;

  // we can't store more than 1kb at a time in flexRam, sequential writes
  do {
    // FIXME this assumes a 2kb FlexRAM conf, are there any others possible in a teensy?


      // we dont need to wait for erase yet

    // and we can only write to lower 1/2 of flexRAM first, then the
    // other half when 1/2 completes in that sequence

    uint16_t phrasesWritten = 0;

    // do 1kb each and sequential writes
    while (flexRamBuf < flexRam2half) {
	// system resets at flexRam2half + 0x1A
     // if (flexRamBuf == flexRamStart + 0x400 && sectorInc) { while(1);}
      *flexRamBuf = *memBuf; // write to rom

      // checks for write errors (addr is volatile)
      if (*flexRamBuf != *memBuf) {
        err = C_bootloaderErrPageWriteFailed;
        // quit writing sequence and restore
        break;
      }

      ++phrasesWritten;
      ++flexRamBuf;
      ++memBuf;
    }

    // erase cmd might not be finished yet
    //flash_wait_for_last_operation();
    while((FTFL_FSTAT & FTFL_FSTAT_CCIF) == 0);
    //FTFL_FSTAT &= ~FTFL_FCNFG_ERSSUSP; // clear erase supend

    if (err != C_bootloaderErrOK)
      break;
    else if (FTFL_FSTAT & FTFL_FSTAT_MGSTAT0) {
	// flash has error
	//digitalWrite(9, !digitalRead(9));
      err = C_bootloaderErrPageWriteFailed;
      break;
    } else {
      FTFL_FCCOB0 = FLTL_FCCOB_PGMSEC; // (PGMSEC) 28.4.11.8 Program Section Command
      FTFL_FCCOB1 = ((uint32_t)sectorAddr & 0x00FF0000u) >> 16; // address of sector to program
      FTFL_FCCOB2 = ((uint32_t)sectorAddr & 0x0000FF00u) >> 8;
      FTFL_FCCOB3 = ((uint32_t)sectorAddr & 0x000000FFu) >> 0;
      // number of phrases to store
      // a phrase is 64bits aligned [2:0]
      FTFL_FCCOB4 = (phrasesWritten & 0xFF00u) >> 8; // length stored in FlexRAM
      FTFL_FCCOB5 = (phrasesWritten & 0x00FFu) >> 0;
      FTFL_FSTAT |= FTFL_FSTAT_CCIF; // run cmd
      while((FTFL_FSTAT & FTFL_FSTAT_CCIF) == 0);
      //flash_run_cmd();

      --sectors;

      //flash_wait_for_last_operation();
      while((FTFL_FSTAT & FTFL_FSTAT_CCIF) == 0);
    }

      // must start over from top otherwise we would write into the 2/2 half
      // if I understand cossrectly it is't allowed
      flexRamBuf = (uint64_t*)flexRamStart;

    // advance sector storeAddress
    sectorAddr += (flexRam2half - flexRamStart) *2; // as they are 64bit to 32

  } while(err == C_bootloaderErrOK && sectors);

  // cleanup and sanity checks

  //flash_wait_for_last_operation();
  while((FTFL_FSTAT & FTFL_FSTAT_CCIF) == 0);

  // flash has error
  if (FTFL_FSTAT & FTFL_FSTAT_MGSTAT0)
    err = C_bootloaderErrPageWriteFailed;

  if (isEEPROM) {
    // restore to EEPROM
    FTFL_FCCOB0 = 0x81; // (SETRAM) 28.4.11.15 Set FlexRAM Function Command
    FTFL_FCCOB1 = 0x00; // Make FlexRAM available as EEPROM:
    FTFL_FSTAT |= FTFL_FSTAT_CCIF; // run cmd
    while((FTFL_FSTAT & FTFL_FSTAT_CCIF) == 0);
  }

  // program cache must be invalidated
  //flash_invalidate_cache();
  FMC_PFB0CR |= FMC_PFB0CR_CINV_WAY;
  while((FTFL_FSTAT & FTFL_FSTAT_CCIF) == 0);

  __enable_irq();

  //print_str("exit fls:");print_uint(err);endl();
  //print_str("romstart");print_uint((uint32_t)_appRomStart);endl();print_flush();

  return err;
}

