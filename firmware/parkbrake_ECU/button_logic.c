/*
 * button_logic.c
 *
 *  Created on: 28 jul 2017
 *      Author: fredrikjohansson
 */

#include "ch.h"
#include "hal.h"
#include "button_logic.h"
#include "sensors.h"
#include "control.h"
#include "eeprom_setup.h" // for settings
#include "debug.h"


#define BTN_ACTIVATE_OK (SEN_BUTTON_SIG && !SEN_BUTTON_INV_SIG)
#define BTN_RELEASE_OK (!SEN_BUTTON_SIG && SEN_BUTTON_INV_SIG)

// -----------------------------------------------------------------------------------------
// start private variables and functions for this module

static thread_t *btnLogicp;

// -----------------------------------------------------------------------------------------
// start threads


static THD_WORKING_AREA(waButtonLogic, 128);
static THD_FUNCTION(buttonLogic, arg)
{
    (void)arg;

    // set up this thread so it only listens to these events
    event_listener_t evtListener;
    chEvtRegisterMaskWithFlags(&sen_measuredEvts, &evtListener, EVENT_MASK(0),
                               EVENT_FLAG_BUTTON_SIG | EVENT_FLAG_BUTTON_INV_SIG |
                               EVENT_FLAG_LIGHTS_ON_SIG | EVENT_FLAG_IGN_ON_SIG);

    while (TRUE) {
        // globally wait for a new event
        /*eventflags_t flg = */ chEvtWaitAny(ALL_EVENTS);



        // we must hold button in 500ms else its not a valid request
        if (chEvtWaitAnyTimeout(ALL_EVENTS, MS2ST(500)) != 0)
            continue; // was not a timeout request, user has released button or brake

        /* Hardware error prvents this from working, we can't get power to uC when IGN off
        // auto tighten
        if (flg & EVENT_FLAG_IGN_ON_SIG) {
            // we might have turned of ignition, should auto tighten
            if (!SEN_IGN_ON_SIG && settings[S_AutoTighten]) {
                // TODO should not auto tighten if we have speed
                // should delay until speed is below 7kph

                // we should not auto tighten brake if we hold release button during IGN off
                if (!BTN_RELEASE_OK)
                    ctrl_setStateAll(Tightening);
            }
        }
        */

        if (BTN_ACTIVATE_OK) {
            // should activate (tighten) parkbrake
            ctrl_setStateAll(Tightening);
        } else if (BTN_RELEASE_OK) {
            // should deactivate (release) parkbrake
            if (SEN_IGN_ON_SIG) {
                if (SEN_LIGHTS_ON_SIG) {
                    ctrl_setStateAll(Releasing);
                } else {
                    DEBUG_OUT("Must press brakepedal");
                }
            }
        }
    }
}


// -----------------------------------------------------------------------------------------
// start API functions


void btn_initButtonLogic(void)
{
    btnLogicp = chThdCreateStatic(&waButtonLogic, 128, NORMALPRIO, buttonLogic, NULL);
    chThdStart(btnLogicp);
}