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
 *  4 external digital inputs, brakelights, button, button_inv, and ign_on
 *
 *
 */


// this module has prefix sen_


// quick defines to read digital inputs

// internal to ECU signals
#define SEN_LEFT_FRONT_DIAG  palReadPad(GPIOB, GPIOB_LeftFront_DIAG) /* High when LF hardware current limit triggered */
#define SEN_RIGHT_FRONT_DIAG palReadPad(GPIOB, GPIOB_RightFront_DIAG) /* High when RF hardware current limit triggered */
#define SEN_LEFT_REAR_DIAG   palReadPad(GPIOA, GPIOA_LeftRear_DIAG) /* High when LR hardware current limit triggered */
#define SEN_RIGHT_REAR_DIAG  palReadPad(GPIOA, GPIOA_RightRear_DIAG) /* High when RR hardware current limit triggered */
#define SEN_TIGHTEN_CMD_SIG  palReadPad(GPIOF, GPIOF_TIGHTEN_CMD_SIG) /* Hardware says it's ok to tighten */
#define SEN_RELEASE_CMD_SIG  palReadPad(GPIOF, GPIOF_RELEASE_MD_SIG) /* Hardware says it's ok to release */
#define SEN_PWR_ENABLED_SIG  palReadPad(GPIOA, GPIOA_PWR_ENABLED_SIG) /*when power to bridge is let through (hardware limited for safety reasons) */

// external to ECU signals
#define SEN_IGN_ON_SIG       !palReadPad(GPIOD, GPIOD_IGN_ON_SIG) /* ignition is on */
#define SEN_LIGHTS_ON_SIG    !palReadPad(GPIOD, GPIOD_LIGHTS_ON_SIG) /* brake lights */
#define SEN_BUTTON_SIG       palReadPad(GPIOE, GPIOE_BUTTON_SIG)  /* activate button input */
#define SEN_BUTTON_INV_SIG   palReadPad(GPIOE, GPIOE_BUTTON_INV_SIG) /* activate button inverted input */

/*
// Event flags for sensor events
#define EVENT_FLAG_ADC_BACKGROUND 1U
#define EVENT_FLAG_ADC_REARAXLE   2U
#define EVENT_FLAG_ADC_FRONTAXLE  4U
// digital inputs
#define EVENT_FLAG_PWR_ENABLED_SIG 0x08U
#define EVENT_FLAG_TIGHTEN_CMD_SIG 0x10U
#define EVENT_FLAG_RELEASE_CMD_SIG 0x20U
#define EVENT_FLAG_BUTTON_SIG      0x40U
#define EVENT_FLAG_BUTTON_INV_SIG  0x80U
#define EVENT_FLAG_IGN_ON_SIG      0x100U
#define EVENT_FLAG_LIGHTS_ON_SIG   0x200U
// bridge internals
#define EVENT_FLAG_BRIDGE_LF_DIAG  0x400U
#define EVENT_FLAG_BRIDGE_RF_DIAG  0x800U
#define EVENT_FLAG_BRIDGE_LR_DIAG  0x1000U
#define EVENT_FLAG_BRIDGE_RR_DIAG  0x2000U
#define EVENT_FLAG_BRIDGE_DIAG_PINS 0x3C00U
*/

// helper macros, might be useful
#define MAX(a,b) ((a) > (b) ? a : b)
#define MIN(a,b) ((a) < (b) ? a : b)


typedef enum {
    // msg events
    MsgStopAll      = 0x00000001U,
    MsgStartAll     = 0x00000002U,
    MsgStopFront    = 0x00000004U,
    MsgStartFront   = 0x00000008U,
    MsgStopRear     = 0x00000010U,
    MsgStartRear    = 0x00000020U,
    MsgEventEnd     = 0x00000080U,
    MsgAllEvents    = 0x000000FFU,

    // ADC events
    AdcBackground   = 0x00000100U,
    AdcRearAxle     = 0x00000200U,
    AdcFrontAxle    = 0x00000400U,
    AdcEventEnd     = 0x00008000U,
    AdcAllEvents    = 0x0000FF00U,

    // Signals
    SigPwrEnabled   = 0x00010000U,
    SigTightenCmd   = 0x00020000U,
    SigReleaseCmd   = 0x00040000U,
    SigButton       = 0x00080000U,
    SigButtonInv    = 0x00100000U,
    SigIgnOn        = 0x00200000U,
    SigLightsOn     = 0x00400000U,
    SigEventEnd     = 0x00800000U,
    SigAllEvents    = 0x00FF0000U,

    // Bridge
    BrgLFDiag       = 0x01000000U,
    BrgRFDiag       = 0x02000000U,
    BrgLRDiag       = 0x04000000U,
    BrgRRDiag       = 0x08000000U,
    BrgAllDiags     = 0x0F000000U,
    BrgEventEnd     = 0x80000000U,
    BrgAllEvents    = 0xFF000000U,

} sen_measure_evt;

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
    uint8_t leftFront_rps, // in revolution per second
            rightFront_rps,
            leftRear_rps,
            rightRear_rps;

} sen_wheelspeeds_t;

// when broadcasting to msgHandlerThd mask with these values
static const uint8_t ADC_EVT = 0,
                     MSG_EVT = 4;

extern event_source_t sen_measuredEvts,
                      sen_msgHandlerThdEvt;

// shared sensor info, should only be readable everywhere except sensors.c
extern sen_motor_currents_t sen_motorCurrents;
extern sen_voltages_t       sen_voltages;
extern int8_t               sen_chipTemperature;
extern sen_wheelspeeds_t    sen_wheelSpeeds;

void sen_initSensors(void);
// clears previous measurement (should be called when starting a Tighten or Loosen sequence)

// diagnose wheelsensor circuits
int sen_diagWheelSensors(void);

// gets the avarage rps of wheels with less than 20% slip
uint8_t sen_wheelAverage(void);

// calculates vehicle speed from fastest spinning wheel, returns in kph
uint8_t sen_vehicleSpeed(void);


#endif // SENSORS_H
