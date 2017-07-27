/*
 * CarTransporter
 * Copyright (C) 2017  Fredrik Johansson mumme74@github.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef SENSORS_H
#define SENSORS_H

#include "ch.h"

/*
 * The parkbrake ECU has:
 *
 *  4 analog current sensor for each parkbrake motor
 *  2 analog voltage sensors for 12V bat and 12V ign
 *
 *  3 external digital inputs, brakelights, button, button_inv
 *
 *
 */

#define PWR_ENABLED_SIG palReadPad(GPIOA, GPIOA_PWR_ENABLED_SIG)
#define IGN_ON_SIG      palReadPad(GPIOD, GPIOD_IGN_ON_SIG)
#define LIGHTS_ON_SIG   palReadPad(GPIOD, GPIOD_LIGHTS_ON_SIG)
#define BUTTON_SIG      palReadPad(GPIOE, GPIOE_BUTTON_SIG)
#define BUTTON_INV_SIG  palReadPad(GPIOE, GPIOE_BUTTON_INV_SIG)
#define TIGHTEN_CMD_SIG palReadPad(GPIOF, GPIOF_TIGHTEN_CMD_SIG)
#define RELEASE_CMD_SIG palReadPad(GPIOF, GPIOF_RELEASE_CMD_SIG)

#define EVENT_FLAG_ADC_BACKGROUND 1U
#define EVENT_FLAG_ADC_REARAXLE 2U
#define EVENT_FLAG_ADC_FRONTAXLE 4U

// this module has prefix sen_


typedef enum {
    StopAll = 0,
    StartAll = 1,
    StopFront = 2,
    StartFront = 4,
    StopRear = 5,
    StartRear = 6,
} sen_measure;

typedef struct {
    // motorcurrents i milliamps
    uint16_t leftRear,
             leftFront,
             rightRear,
             rightFront;

    uint16_t maxLeftRear,
             maxLeftFront,
             maxRightRear,
             maxRightFront;

} sen_motor_currents_t;

typedef struct {
    uint16_t ignVolt;
    uint16_t batVolt;
} sen_voltages_t;

typedef struct {

} sen_button_t;

extern event_source_t sen_AdcMeasured;
extern event_source_t sen_MsgHandlerThd;

// shared sensor info, should only be readable everywhere except sensors.c
extern const sen_motor_currents_t sen_motorCurrents;
extern const sen_voltages_t       sen_voltages;
extern const int8_t               sen_chipTemperature;


void sen_initSensors(void);
// clears previous measurment (should be called when starting a Tighten or Loosen sequence)

// diagnose wheelsensor circuits
int sen_diagWheelSensors(void);


#endif // SENSORS_H
