/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ch.h"
#include "hal.h"

#include "sensors.h"
#include "eeprom_setup.h"
#include "control.h"
#include "button_logic.h"
#include "can.h"
#include "pwm.h"
#include "diag.h"
#include "debug.h"
#include "canserial.h"


/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  // enable hardware Current limit
  osalSysLock();
  pwm_bridgeCurLimOn();
  pwm_bridgeAllOutputsOff();
  osalSysUnlock();

#ifdef DEBUG_MODE
#ifndef CANSERIAL
  // set up debug channel on serial out
  sdStart(&SD1, NULL);
#else
  canserial_init();
#endif
#endif
  // eeprom should be first as other modules depend on its settings
  ee_initEeprom();
  can_init();
  diag_init();
  pwm_init(); // must be before ctrl_init due to mutex init
  ctrl_init();
  sen_initSensors();
  btn_initButtonLogic();

  while (TRUE) {
    while (!can_rebootReq) {
      chThdSleepMilliseconds(500); // do nothing loop, let the kernel handle it for us
      DEBUG_OUT_PRINTF("Beat%u\n", 12345); //ST2MS(chVTGetSystemTime()));
    }

    // terminate thds
    btn_thdsTerminate();
    canserial_thdsTerminate();
    ctrl_thdsTerminate();
    pwm_thdsTerminate();
    diag_thdsTerminate();
    sen_thdsTerminate();
    can_thdsTerminate();
    ee_thdsTerminate();

    // wait for theads to terminate and deinit drivers
    btn_doShutdown();
    canserial_doShutdown();
    ctrl_doShutdown();
    pwm_doShutdown();
    diag_doShutdown();
    // those with hardware driver *Start should be last
    sen_doShutdown();
    can_doShutdown();
    ee_doShutdown();

    // enable hardware Current limit for safety
    osalSysLock();
    pwm_bridgeCurLimOn();
    pwm_bridgeDisable();
    pwm_bridgeAllOutputsOff();
    osalSysUnlock();

    // turn off remaining stuff
    chSysDisable();

    // reset
    NVIC_SystemReset();
  }
}
