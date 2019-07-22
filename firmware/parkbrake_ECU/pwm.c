/*
 * pwm.c
 *
 *  Created on: 21 jul 2019
 *      Author: jof
 */

#include "pwm.h"
#include <ch.h>
#include <hal.h>
#include "board.h"


typedef struct {
  stm32_gpio_t *port;
  uint8_t pin;
} PortPins_t;

typedef struct {
  PortPins_t diag, tighten, loosen;
  uint16_t errCounter;
} WheelPins_t;

//------------------------------------------------------------------
// begin private functions and variables

static thread_t *pwmHandlerThdp;

static WheelPins_t portPins[4] = {
    { // LF
      {GPIOB, GPIOB_LeftFront_DIAG},
      {GPIOC, GPIOC_LeftFront_Tighten},
      {GPIOC, GPIOC_LeftFront_Loosen},
      0
    },
    { // RF
      {GPIOB, GPIOB_RightFront_DIAG},
      {GPIOC, GPIOC_RightFront_Tighten},
      {GPIOC, GPIOC_RightFront_Loosen},
      0
    },
    { // LR
      {GPIOA, GPIOA_LeftRear_DIAG},
      {GPIOC, GPIOC_LeftRear_Tighten},
      {GPIOC, GPIOC_LeftRear_Loosen},
      0
    },
    { // RR
      {GPIOA, GPIOA_RightRear_DIAG},
      {GPIOC, GPIOC_RightRear_Tighten},
      {GPIOC, GPIOC_RightRear_Loosen},
      0
    },
};

static THD_WORKING_AREA(waPWMhandlerThd, 128);

static THD_FUNCTION(pwmHandlerThd, arg)
{
    (void)arg;

    chRegSetThreadName("pwm_PWMhandler");
    static uint8_t pwmCnt = 0;
    static const uint16_t ENABLE_CYCLES = 2000; // 4seconds inactivity, then bridge off
    static const uint16_t maxDiagErrors = 300;
    static uint16_t enableCounter = 0; // turns of bridge if no action for a
                                   // predetermined number of cycles
    static bool resetBridge = true, // first time we want to make sure bridge is reset
                bridgeOff = true;

    while(!chThdShouldTerminateX()) {
      chThdSleep(TIME_US2I(200));
      // TODO how to handle ADC events?

      osalSysLock();

      for (uint8_t i = 0; i < 4; ++i) {
        switch(pwm_values.dir[i]) {
        case PWM_tighten:
          if (pwmCnt == 0 && pwm_values.duty[i] > 0) {
            if (portPins[i].errCounter < maxDiagErrors)
              palSetPad(portPins[i].tighten.port,
                        portPins[i].tighten.pin);
          } else if (pwmCnt == pwm_values.duty[i]) {
            palClearPad(portPins[i].tighten.port,
                        portPins[i].tighten.pin);
          }
          enableCounter = ENABLE_CYCLES;
          break;
        case PWM_loosen:
          if (pwmCnt == 0 && pwm_values.duty[i] > 0) {
            if (portPins[i].errCounter < maxDiagErrors)
              palSetPad(portPins[i].loosen.port,
                        portPins[i].loosen.pin);
          } else if (pwmCnt == pwm_values.duty[i]) {
            palClearPad(portPins[i].loosen.port,
                        portPins[i].loosen.pin);
          }
          enableCounter = ENABLE_CYCLES;
          break;
        case PWM_off: default:
          palClearPad(portPins[i].tighten.port,
                      portPins[i].tighten.pin);
          palClearPad(portPins[i].loosen.port,
                      portPins[i].loosen.pin);
          break;
        }

        if (enableCounter == ENABLE_CYCLES &&
            palReadPad(portPins[i].tighten.port,
                       portPins[i].diag.pin))
        {
          ++portPins[i].errCounter;
          resetBridge = true;
        }
      }

      // auto enable/disable after some cycles
      if (enableCounter == ENABLE_CYCLES) {
        if (bridgeOff) {
          resetBridge = true;
          bridgeOff = false;
        }
      } else if (enableCounter && --enableCounter == 0) {
        palClearPad(GPIOB, GPIOB_Bridge_Reset_INV);
        bridgeOff = true;
      }

      // if a error has occured
      if (resetBridge) {
        palClearPad(GPIOB, GPIOB_Bridge_Reset_INV);
        palSetPad(GPIOB, GPIOB_Bridge_Reset_INV);
        resetBridge = false;
      }

      osalSysUnlock();

      if (pwmCnt++ > 9) pwmCnt = 0;
    }
}




// ----------------------------------------------------------------
// begin exported functions and variables

// NOTE this value has the associated semaphore, see below
PWMvalues_t pwm_values;
mutex_t pwm_valuesMtx;

/**
 * @brief starts our pwm thread
 */
void pwm_init(void) {
  // init semaphore for pwm_values
  chMtxObjectInit(&pwm_valuesMtx);
  // start pwm thread
  pwmHandlerThdp = chThdCreateStatic(&waPWMhandlerThd,sizeof(waPWMhandlerThd),
                                     NORMALPRIO +9, pwmHandlerThd, NULL);
}

/**
 * @breif tells thd to kill itself
 */
void pwm_thdsTerminate(void)
{
  chThdTerminate(pwmHandlerThdp);
}

/**
 * @brief blocks until thd has killed itself
 */
void pwm_doShutdown(void)
{
  chThdWait(pwmHandlerThdp);
}


/**
 * @brief Must be high to arm Currentlimit
 * @info   Must be run with osalSysLock
 */
void pwm_bridgeReset(void) {
  pwm_bridgeCurLimOff();
  pwm_bridgeCurLimOn();
  palClearPad(GPIOC, GPIOC_uC_SET_POWER);
}

/**
 * @brief turns on current limit to bridge
 * @info   Must be run with osalSysLock
 */
void pwm_bridgeCurLimOn(void) {
  palSetPad(GPIOB, GPIOB_Bridge_Reset_INV);
}

/**
 * @brief turns on Current limit to bridge
 * @warning ! this is potentially dangerous as
 *            hardware can't turn off itself
 * @info   Must be run with osalSysLock
 */
void pwm_bridgeCurLimOff(void) {
  /* potentially dangerus if CL is triggered */
  if (palReadPad(GPIOB, GPIOB_LeftFront_DIAG)) {
      palClearPad(GPIOC, GPIOC_LeftFront_Tighten);
      palClearPad(GPIOC, GPIOC_LeftFront_Loosen);
  }
  if (palReadPad(GPIOB, GPIOB_RightFront_DIAG)) {
      palClearPad(GPIOC, GPIOC_RightFront_Tighten);
      palClearPad(GPIOC, GPIOC_RightFront_Loosen);
  }
  if (palReadPad(GPIOA, GPIOA_LeftRear_DIAG)) {
      palClearPad(GPIOC, GPIOC_LeftRear_Tighten);
      palClearPad(GPIOC, GPIOC_LeftRear_Loosen);
  }
  if (palReadPad(GPIOB, GPIOA_RightRear_DIAG)) {
      palClearPad(GPIOC, GPIOC_RightRear_Tighten);
      palClearPad(GPIOC, GPIOC_RightRear_Loosen);
  }
  palClearPad(GPIOB, GPIOB_Bridge_Reset_INV);
}

/**
 * @breif sets all output pads to off
 * @info  Must be run with osalSysLock
 */
void pwm_bridgeAllOutputsOff(void) {
  palClearPad(GPIOC, GPIOC_LeftFront_Tighten);
  palClearPad(GPIOC, GPIOC_LeftFront_Loosen);
  palClearPad(GPIOC, GPIOC_RightFront_Tighten);
  palClearPad(GPIOC, GPIOC_RightFront_Loosen);
  palClearPad(GPIOC, GPIOC_LeftRear_Tighten);
  palClearPad(GPIOC, GPIOC_LeftRear_Loosen);
  palClearPad(GPIOC, GPIOC_RightRear_Tighten);
  palClearPad(GPIOC, GPIOC_RightRear_Loosen);
}

/**
 * @breif enable force setting power to bridge
 * @info  Must be run with osalSysLock
 */
void pwm_bridgeEnable(void) {
  pwm_bridgeReset();
  palSetPad(GPIOC, GPIOC_uC_SET_POWER);
}

/**
 * @breif ensures that we have no forced power to bridge
 * @info  Must be run with osalSysLock
 */
void pwm_bridgeDisable(void) {
  pwm_bridgeReset();
  palClearPad(GPIOC, GPIOC_uC_SET_POWER);
}
