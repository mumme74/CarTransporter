/*
 * main_teensy.cpp
 *
 *  Created on: 2 jul 2019
 *      Author: jof
 */


#include <Arduino.h>

extern "C" {
#include <system.h>
#include <commands.h>
#include <can.h>
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  static uint32_t bootloaderWindow = millis() + 100;

  while (bootloaderWindow > millis()) {

  }
}

