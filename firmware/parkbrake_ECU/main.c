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
#include "diag.h"
#include "debug.h"


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

#ifdef DEBUG_MODE
  // set up debug channel on serial out
  sdStart(&SD1, NULL);
#endif
  // eeprom should be first as other modules depend on its settings
  ee_initEeprom();
  can_init();
  diag_init();
  ctrl_init();
  sen_initSensors();
  btn_initButtonLogic();

  while (TRUE) {
    chThdSleepMilliseconds(500); // do nothing loop, let the kernel handle it for us
    DEBUG_OUT_PRINTF("beat\r\n", ST2MS(chVTGetSystemTime()));
  }
}
