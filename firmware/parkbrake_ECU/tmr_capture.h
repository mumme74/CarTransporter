/*
 * tmr_compare.h
 *
 *  Created on: 29 jul 2017
 *      Author: fredrikjohansson
 */

#ifndef TMR_COMPARE_H_
#define TMR_COMPARE_H_

/**
 * @breif this module sets up TIM12, TIM13 and TIM14  to input capture of wheel sesnors.
 * It works by measuring 5 consecutive timer measurements and move data with DMA before
 *  trigger a interrupt and calculate wheel speed
 */

// we can't use ICU module due to TIM12 and TIM13 not supported

#include "sensors.h"

void tmr_init(void);

#endif /* TMR_COMPARE_H_ */
