/*
 * system_teensy3.cpp
 *
 *  Created on: 2 jul 2019
 *      Author: jof
 */


#include "system.h"
#include "can.h"
#include "commands.h"
#include <Arduino.h>


// from 4.3.4
// http://infocenter.arm.com/help/topic/com.arm.doc.dui0553b/DUI0553.pdf
#define SCB_VTOR_TBLOFF_LSB     7
#define SCB_VTOR_TBLOFF         (0x1FFFFFF << SCB_VTOR_TBLOFF_LSB)

// -------------- Begin exported symbols ---------------------------

#if NODE_ID==CAN_PARKBRAKE_NODE
const uint16_t canId = CAN_MSG_TYPE_COMMAND | C_parkbrakeCmdBootloader | C_parkbrakeNode;
#elif NODE_ID==CAN_SUSPENSION_NODE
const uint16_t canId = CAN_MSG_TYPE_COMMAND | C_suspensionCmdBootloader | C_suspensionNode;
#endif


const uint16_t pageSize = 2048; // 2kb

volatile uint32_t globalMsSinceStartup;

// ------------- End exported symbols ------------------------------

void systemInit(void) {
  Serial.begin(115200);
  globalMsSinceStartup = -1; // invalidate
}

void systemDeinit(void) {
  Serial.end();
}

void systickInit(void) {} /* stub */
void systickShutdown(void) {} /* stub */

void systemToApplication(void) {
  extern const size_t _appRomStart, _bootRom, _estack, _sdata;
  typedef void (*funcptr_t)(void);

  /* variable that will be loaded with the start address of the application */
  size_t  *jumpAddress;
  const size_t* applicationAddress = &_appRomStart;

  // safetycheck, if hang in bootloader mode if we dont have a valid start of vector table
  if (applicationAddress[0] > _estack || applicationAddress[0] < _sdata) {
    canframe_t msg;
    canInitFrame(&msg, canId);
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
  // and https://mcuoneclipse.com/2015/07/01/how-to-reset-an-arm-cortex-m-with-software/
  SCB_AIRCR |= (0x5FA << 16) | (1 << 2);

  // wait forever until reset is done in hardware
  for(;;)
    ;
}
