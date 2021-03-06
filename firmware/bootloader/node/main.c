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

    can_frame_t msg;
    canInitFrame(&msg, CAN_MY_ID);
    msg.data8[msg.DLC++] = C_bootloaderReset;
    canPost(&msg);

    while (systemMillis() < WAIT_BEFORE_APPBOOT){
      if (canGet(&msg))
        commandsStart(&msg); // resets from within
    }

    canShutdown();
    systickShutdown();
    systemDeinit();
    systemToApplication();
}
