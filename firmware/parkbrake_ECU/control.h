/*
 * control.h
 *
 *  Created on: 26 jul 2017
 *      Author: fredrikjohansson
 */

#ifndef CONTROL_H_
#define CONTROL_H_

#include "ch.h"

/**
 * This module handles the state of parkingbrakes
 *
 * Its basically a state machine
 */

typedef enum {
    Tightened = 0,
    Released = 1,
    Tightening = 2,
    Releasing = 3,
    ServiceState = 4, // when we need to change brake pads etc
    ErrorState = 8
} ctrl_states ;

typedef enum {
    LeftFront = 0,
    RightFront = 1,
    LeftRear = 2,
    RightRear = 3
} ctrl_wheels;

typedef enum  {
    FrontAxle = 0,
    RearAxle = 1
} ctrl_axles;


void ctrl_init(void);

// on all axles
void ctrl_setStateAll(ctrl_states state);

// Axle wise
void ctrl_setStateAxle(ctrl_states state, ctrl_axles axle);

// on single wheel
void ctrl_setStateWheel(ctrl_states state, ctrl_wheels wheel);

// on a single wheel
ctrl_states ctrl_getState(ctrl_wheels wheel);
// on a single wheel
bool ctrl_checkForError(ctrl_wheels wheel);
// check on all wheels
int ctrl_checkForErrors(void);




#endif /* CONTROL_H_ */
