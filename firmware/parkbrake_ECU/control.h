/*
 * control.h
 *
 *  Created on: 26 jul 2017
 *      Author: fredrikjohansson
 */

#ifndef CONTROL_H_
#define CONTROL_H_

//#include "ch.h"

/**
 * This module handles the state of parkingbrakes
 *
 * Its basically a state machine
 */

/**
 * @brief Must be high to arm Currentlimit
 */
#define BRIDGE_RESET \
  BRIDGE_CL_OFF \
  BRIDGE_CL_ON \
  palClearPad(GPIOC, GPIOC_uC_SET_POWER);

#define BRIDGE_CL_ON { palSetPad(GPIOB, GPIOB_Bridge_Reset_INV); }

#define BRIDGE_CL_OFF { \
  /* potentially dangerus if CL is triggered */ \
  if (palReadPad(GPIOB, GPIOB_LeftFront_DIAG)) { \
      palClearPad(GPIOC, GPIOC_LeftFront_Tighten); \
      palClearPad(GPIOC, GPIOC_LeftFront_Loosen); \
  } \
  if (palReadPad(GPIOB, GPIOB_RightFront_DIAG)) { \
      palClearPad(GPIOC, GPIOC_RightFront_Tighten); \
      palClearPad(GPIOC, GPIOC_RightFront_Loosen); \
  } \
  if (palReadPad(GPIOA, GPIOA_LeftRear_DIAG)) { \
      palClearPad(GPIOC, GPIOC_LeftRear_Tighten); \
      palClearPad(GPIOC, GPIOC_LeftRear_Loosen); \
  } \
  if (palReadPad(GPIOB, GPIOA_RightRear_DIAG)) { \
      palClearPad(GPIOC, GPIOC_RightRear_Tighten); \
      palClearPad(GPIOC, GPIOC_RightRear_Loosen); \
  } \
  palClearPad(GPIOB, GPIOB_Bridge_Reset_INV); \
}

#define BRIDGE_ALL_OUTPUTS_OFF { \
  palClearPad(GPIOC, GPIOC_LeftFront_Tighten); \
  palClearPad(GPIOC, GPIOC_LeftFront_Loosen); \
  palClearPad(GPIOC, GPIOC_RightFront_Tighten); \
  palClearPad(GPIOC, GPIOC_RightFront_Loosen); \
  palClearPad(GPIOC, GPIOC_LeftRear_Tighten); \
  palClearPad(GPIOC, GPIOC_LeftRear_Loosen); \
  palClearPad(GPIOC, GPIOC_RightRear_Tighten); \
  palClearPad(GPIOC, GPIOC_RightRear_Loosen); \
}

typedef enum {
    Tightened = 0,
    Released = 1,
    Tightening = 2,
    Releasing = 3,
    InServiceState = 4, // when we need to change brake pads etc
    SetServiceState = 5,
    ErrorState = 8
} ctrl_states;

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
void ctrl_thdsTerminate(void);
void ctrl_doShutdown(void);

// on all axles
int ctrl_setStateAll(ctrl_states state);

// Axle wise
int ctrl_setStateAxle(ctrl_states state, ctrl_axles axle);

// on single wheel
int ctrl_setStateWheel(ctrl_states state, ctrl_wheels wheel);

// on a single wheel
ctrl_states ctrl_getStateWheel(ctrl_wheels wheel);
// on a single wheel
bool ctrl_checkForError(ctrl_wheels wheel);
// check on all wheels
int ctrl_checkForErrors(void);




#endif /* CONTROL_H_ */
