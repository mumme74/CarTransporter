/*
 * pwm.h
 *
 *  Created on: 21 jul 2019
 *      Author: jof
 */

#ifndef PWM_H_
#define PWM_H_

#include <ch.h>
#include <stdint.h>

// This is intended to control the PWM of each wheel
// Due to inpropper harware design (WRONG pin layout)
// we must do a crude bit bang software pwm here

#define PWM_MAX_DUTY 10u

typedef enum PWMDirections {
  PWM_loosen,
  PWM_off,
  PWM_tighten
} PWMDirections_e;

typedef struct  {
  // NOTE !! these must be order in the same order as ctrl_wheels
  union {
    uint8_t LFduty, RFduty, LRduty, RRduty; // from 0 - 10
    uint8_t duty[4];
  };
  // NOTE !! these must be order in the same order as ctrl_wheels
  union {
    PWMDirections_e  LFdir,  Rfdir,  LRdir,  RRdir;  // 0 = off, -1 = reversed, +1 normal
    PWMDirections_e dir[4];
  };
} PWMvalues_t;

// NOTE this should only be consumed by pwm thread, never changed
extern PWMvalues_t pwm_values;
extern mutex_t pwm_valuesMtx;

/**
 * @brief starts our pwm thread
 */
extern void pwm_init(void);

/**
 * @breif tells thd to kill itself
 */
void pwm_thdsTerminate(void);

/**
 * @brief blocks until thd has killed itself
 */
void pwm_doShutdown(void);

/**
 * @brief Must be high to arm Currentlimit
 * @info   Must be run with osalSysLock
 */
void pwm_bridgeReset(void);

/**
 * @brief turns on current limit to bridge
 * @info   Must be run with osalSysLock
 */
void pwm_bridgeCurLimOn(void);

/**
 * @brief turns on Current limit to bridge
 * @warning ! this is potentially dangerous as
 *            hardware can't turn off itself
 * @info   Must be run with osalSysLock
 */
void pwm_bridgeCurLimOff(void);

/**
 * @breif sets all output pads to off
 * @info  Must be run with osalSysLock
 */
void pwm_bridgeAllOutputsOff(void);

/**
 * @breif enable power to bridge
 * @info  Must be run with osalSysLock
 */
void pwm_bridgeEnable(void);

/**
 * @breif ensures that we have no forced power to bridge
 * @info  Must be run with osalSysLock
 */
void pwm_bridgeDisable(void);

#endif /* PWM_H_ */
