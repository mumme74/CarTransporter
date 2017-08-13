/*
 * suspension_config.h
 *
 *  Created on: 13 aug 2017
 *      Author: fredrikjohansson
 */

#ifndef SUSPENSION_CONFIG_H_
#define SUSPENSION_CONFIG_H_

#include <stdint.h>


enum Configs : uint8_t {
  NoConfig = 0,
  KP_factor_float = 1,
  KI_factor_float = 2,
  KD_factor_float = 3,
  LeftLowSetpoint_uint16 = 4,
  LeftNormalSetpoint_uint16 = 5,
  LeftHighSetpoint_uint16 = 6,
  RightLowSetpoint_uint16 = 7,
  RightNormalSetpoint_uint16 = 8,
  RightHighSetpoint_uint16 = 9,
  DeadWeight_uint16 = 10,
  DeadBand_uint16 = 11,
  CylDia_mm  = 12,
};


#endif /* SUSPENSION_CONFIG_H_ */
