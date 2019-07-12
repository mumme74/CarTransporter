/*
 * main_teensy.cpp
 *
 *  Created on: 2 jul 2019
 *      Author: jof
 */


#include <Arduino.h>
#include <system.h>
#include <commands.h>
#include <can.h>

extern "C" {
extern void _canLoop(void);
} // extern "C"

void setup() {
  systemInit();
  canInit();
  pinMode(6, OUTPUT);
}

void loop() {
  static uint32_t bootloaderWindow = millis() + WAIT_BEFORE_APPBOOT +10000;
  static uint32_t loopCnt = 0;
  static bool toApplicationRunned = false;

  canframe_t msg;

  // notify master node that we can take a new firmware
  if (loopCnt++ == 0) {
    canInitFrame(&msg, canId);
    msg.data8[msg.DLC++] = C_bootloaderReset;
    canPost(&msg);
  }

  if (bootloaderWindow > millis()) {
    _canLoop(); // send possible unsent msgs
    if (canGet(&msg))
      commandsStart(&msg); // resets from within

  } else if (!toApplicationRunned) {
    // timeout, we didn't have any bootloder requests from CAN
    toApplicationRunned = true;
    systemDeinit();
    systemToApplication();
    systemReset();
  }
  _canLoop();
}

