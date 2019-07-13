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


int main(void)
{
    systemInit();
    canInit(); // canId from system.c
    systickInit();

    canframe_t msg;
    canInitFrame(&msg, canId);
    msg.data8[msg.DLC++] = C_bootloaderReset;
    canPost(&msg);

    while (systemMillis() < WAIT_BEFORE_APPBOOT + 7000){
      if (canGet(&msg))
        commandsStart(&msg); // resets from within
    }

    systemDeinit();
    canShutdown();
    systickShutdown();
    systemToApplication();
}
