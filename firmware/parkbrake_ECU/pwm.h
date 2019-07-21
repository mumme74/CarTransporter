/*
 * pwm.h
 *
 *  Created on: 21 jul 2019
 *      Author: jof
 */

#ifndef PWM_H_
#define PWM_H_

#include <stdint.h>

// This is intended to control the PWM of each wheel
// Due to inpropper harware design (WRONG pin layout)
// we must do a crude bit bang software pwm here

typedef struct  {
  union {
    uint8_t LFduty, RFduty, LRduty, RRduty; // from 0 - 10
    uint8_t duty[4];
  };
  union {
    int8_t  LFdir,  Rfdir,  LRdir,  RRdir;  // 0 = off, -1 = reversed, +1 normal
    int8_t dir[4];
  };
} PWMvalues_t;

extern PWMvalues_t pwm_values;

extern void pwm_init(void);
void pwm_thdsTerminate(void);
void pwm_doShutdown(void);

#endif /* PWM_H_ */
