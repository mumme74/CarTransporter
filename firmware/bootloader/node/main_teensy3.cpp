/*
 * main_teensy.cpp
 *
 *  Created on: 2 jul 2019
 *      Author: jof
 */


#include <Arduino.h>
#include <system.h>

extern "C" {
#include <commands.h>
#include <can.h>
}

void setup() {
  systemInit();
}

void loop() {
  static uint32_t bootloaderWindow = millis() + WAIT_BEFORE_APPBOOT;

  while (bootloaderWindow > millis()) {

  }

  systemDeinit();
  systemToApplication();
  systemReset();
}

