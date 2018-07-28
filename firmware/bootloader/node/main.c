/*
 * can.c
 *
 *  Created on: 27 jul 2018
 *      Author: jof
 */

#include "system.h"
#include "can.h"
#include "commands.h"
#include <stddef.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/cm3/nvic.h>

void hard_fault_handler(void)
{
    while(1); // debug
    //systemReboot();
}

int main(void)
{
    systemInit();
    canInit(canId); // canId from system.c
    systickInit();

    canframe_t msg, *pmsg;
    canInitFrame(&msg, canId);
    msg.data8[msg.DLC++] =  C_bootloaderWait & 0x00FF;
    msg.data8[msg.DLC++] = (C_bootloaderWait & 0xFF00) >> 8;
    canPost(&msg);

    while (globalMsSinceStartup < WAIT_BEFORE_APPBOOT + 100000){
      pmsg = canGet();
      if (pmsg != NULL)
        commandsStart(pmsg); // resets from within
    }

    systemDeinit();
    canShutdown();
    systickShutdown();
    systemToApplication();
}
