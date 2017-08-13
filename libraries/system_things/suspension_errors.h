/*
 * errors.h
 *
 *  Created on: 1 apr 2015
 *      Author: jof
 */

#ifndef ERRORS_H_
#define ERRORS_H_
#include <stdint.h>


enum class errorTypes : uint8_t {
  NoError,  // normal behaviour
  OpenLoad, // or shorted to plus
  Shorted,  // to ground
  SwitchOvertemp,
  Overload,
  HardwareFault,

  // Smart outputs
  OverCurrent = 10,
  OverHeated,

  // sensors
  NonValidValue = 30, // non sensical value from a sensor
  ToLowValue,    // value to low to be valid
  ToHighValue,   // value to high to be valid
  ToFastFalling, // when value changes to fast to lower
  ToFastRising   // when value changes to fast to higher
};




#endif /* ERRORS_H_ */
