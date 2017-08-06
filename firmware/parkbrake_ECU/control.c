/*
 * control.c
 *
 *  Created on: 26 jul 2017
 *      Author: fredrikjohansson
 */

#include "ch.h"
#include "hal.h"
#include "control.h"
#include "hal_eeprom.h"
#include "eeprom_setup.h"
#include "sensors.h"
#include "debug.h"
#include "can_protocol.h"
#include "diag.h"


/**
 * This module handles the state of parkingbrakes
 *
 * Its basically a state machine
 */

#define STATE_MASK(wheel) ((stateFlags & (0x0F << wheel)) >> wheel)
#define RR_STATE (stateFlags & 0x0000F000U >> 3)
#define LR_STATE (stateFlags & 0x00000F00U >> 2)
#define RF_STATE (stateFlags & 0x000000F0U >> 1)
#define LF_STATE (stateFlags & 0x0000000FU)

#define WHEEL_THD_STACK_SIZE 128


#define BRIDGE_ENABLE \
    chVTReset(&bridgeDisableTimer); \
    palClearPad(GPIOB, GPIOB_Bridge_Disable); \
    palSetPad(GPIOC, GPIOC_uC_SET_POWER);


#define MAILBOX_SIZE 2

// --------------------------------------------------------------------------------------
// global variables exported from this module

/**
 * stateFile documentation
 * 0xXXXX0000 -> X = reserved, not used at the moment
 * 0x0000x000 -> x = current stored state for LeftFront
 * 0x00000x00 -> x = current stored state for RightFront
 * 0x000000x0 -> x = current stored state for LeftRear
 * 0x0000000x -> x = current stored state for RightRear
 */


static uint32_t stateFlags = 0;


// ------------------------------------------------------------------------------------
// function prototypes and local variable

typedef struct {
    const char *threadName;
    mailbox_t *mb;
    msg_t **mb_queue;
    uint8_t evtReceiveFlag;
    sen_measure adcMeasure;
    volatile const uint16_t *motorcurrent;
    volatile const uint8_t *wheelSpeed;
    const char *abbreviation;
    ctrl_wheels wheel;
    uint8_t releasePin;
    uint8_t tightenPin;

} wheelthreadinfo_t;


static void saveState(uint32_t state, ctrl_wheels wheel);
static msg_t releaseWheel(wheelthreadinfo_t *info);
static msg_t tightenWheel(wheelthreadinfo_t *info);
static msg_t serviceWheel(wheelthreadinfo_t *info);

static mailbox_t mbLF, mbLR, mbRF, mbRR;

static msg_t mbLF_queue[MAILBOX_SIZE] = { 0, 0 },
             mbLR_queue[MAILBOX_SIZE] = { 0, 0 },
             mbRF_queue[MAILBOX_SIZE] = { 0, 0 },
             mbRR_queue[MAILBOX_SIZE] = { 0, 0 };

static virtual_timer_t bridgeDisableTimer,
                       saveStateTimer;

static thread_t *lfThdp,
                *lrThdp,
                *rfThdp,
                *rrThdp;


// ------------------------------------------------------------------------------------
// begin callbacks

// this callback is so we don't inadvertently turn off the bridge until all wheels are finished
void bridgeDisableTimerCallback(void *arg)
{
    (void)arg;

    palSetPad(GPIOB, GPIOB_Bridge_Disable);
    palClearPad(GPIOC, GPIOC_uC_SET_POWER);

    // stop current measurement, resume background checks
    chEvtBroadcastFlags(&sen_MsgHandlerThd, (eventflags_t)StopAll);
}

// this callback is only so we don't save 4 times in eeprom when 1 is enough
// it takes 10ms each time and that would block thread
void saveStateTimerCallback(void *arg)
{
    (void)arg;
    if (fileStreamSeek(stateFile, 0)) {
        EepromWriteWord(stateFile, stateFlags);
    }
}


// -------------------------------------------------------------------------------------
// begin threads


// some thread local info
wheelthreadinfo_t lfInfo = {
    "leftFrontThread", &mbLF, (msg_t**)&mbLF_queue, EVENT_FLAG_ADC_FRONTAXLE,
    StartFront, &sen_motorCurrents.leftFront, &sen_wheelSpeeds.leftFront_rps,
    "LF", LeftFront, GPIOC_LeftFront_Loosen, GPIOC_LeftFront_Tighten
};
wheelthreadinfo_t rfInfo = {
    "rightFrontThread", &mbRF, (msg_t**)&mbRF_queue, EVENT_FLAG_ADC_FRONTAXLE,
    StartFront, &sen_motorCurrents.rightFront, &sen_wheelSpeeds.rightFront_rps,
    "RF", RightFront, GPIOC_RightFront_Loosen, GPIOC_RightFront_Tighten
};
wheelthreadinfo_t lrInfo = {
    "leftRearThread", &mbLR, (msg_t**)&mbLR_queue, EVENT_FLAG_ADC_REARAXLE,
    StartRear, &sen_motorCurrents.leftRear, &sen_wheelSpeeds.leftRear_rps,
    "LR", LeftRear, GPIOC_LeftRear_Loosen, GPIOC_LeftRear_Tighten
};
wheelthreadinfo_t rrInfo = {
    "rightRearThread", &mbRR, (msg_t**)&mbRR_queue, EVENT_FLAG_ADC_REARAXLE,
    StartRear, &sen_motorCurrents.rightRear, &sen_wheelSpeeds.rightRear_rps,
    "RR", RightRear, GPIOC_RightRear_Loosen, GPIOC_RightRear_Tighten
};

// we create 4 copies of this thread, one for each wheel
static THD_WORKING_AREA(waRightFrontHandler, WHEEL_THD_STACK_SIZE);
static THD_WORKING_AREA(waLeftFrontHandler, WHEEL_THD_STACK_SIZE);
static THD_WORKING_AREA(waLeftRearHandler, WHEEL_THD_STACK_SIZE);
static THD_WORKING_AREA(waRightRearHandler, WHEEL_THD_STACK_SIZE);
static THD_FUNCTION(wheelHandler, args)
{
    wheelthreadinfo_t *info  = (wheelthreadinfo_t*)args;

    chRegSetThreadName(info->threadName);

    static event_listener_t adcEvt;

    while(TRUE) {
        ctrl_states action = (ctrl_states)chMBFetch(info->mb, *info->mb_queue, TIME_INFINITE);

        // listen to currents to perform this action
        chEvtRegisterMaskWithFlags(&sen_measuredEvts, &adcEvt,
                                   EVENT_MASK(0), info->evtReceiveFlag);

        msg_t res;
        switch (action) {
        case Releasing:
            // tell sensor module to start measure currents
            chEvtBroadcastFlags(&sen_MsgHandlerThd, info->adcMeasure);
            // do the action
            res = releaseWheel(info);
            if (res != MSG_OK)
            {
                DEBUG_OUT_PRINTF("error %s motor release", info->abbreviation);
                saveState(ErrorState, info->wheel);
                // setting a DTC and broadcast to CAN
                chMBPost(&dtc_MB, res, TIME_IMMEDIATE);
            } else {
                saveState(Released, info->wheel);
            }
            break;
        case Tightening:
            chEvtBroadcastFlags(&sen_MsgHandlerThd, (eventflags_t)StartFront);
            res = tightenWheel(info);
            if (res != MSG_OK)
            {
                DEBUG_OUT_PRINTF("error %s motor tighten", info->abbreviation);
                saveState(ErrorState, info->wheel);
                // setting a DTC and broadcast to CAN
                chMBPost(&dtc_MB, res, TIME_IMMEDIATE);
            } else {
                saveState(Tightened, info->wheel);
            }

            break;
        case SetServiceState:
            chEvtBroadcastFlags(&sen_MsgHandlerThd, (eventflags_t)StartFront);
            res = serviceWheel(info);
            if (res != MSG_OK)
            {
                DEBUG_OUT_PRINTF("error %s motor service", info->abbreviation);
                saveState(ErrorState, info->wheel);
                // setting a DTC and broadcast to CAN
                chMBPost(&dtc_MB, res, TIME_IMMEDIATE);
            } else {
                saveState(InServiceState, info->wheel);
            }

            break;
        default:
            continue; // next while loop
        // else nothing
        }

        // unregister so we don't fill up queue by other old measurements
        chEvtUnregister(&sen_measuredEvts, &adcEvt);

        // when done we start a timer which eventually disables the bridge
        // we don't want to do that in this instant as there maight be other wheels that are still
        // doing some actions on there part
        chVTSet(&bridgeDisableTimer, S2ST(5), bridgeDisableTimerCallback, NULL);
    }
}

// -------------------------------------------------------------------------------------
// begin configuration for peripherals


// --------------------------------------------------------------------------------------
// begin private functions of this module

static void saveState(uint32_t state, ctrl_wheels wheel)
{
    chSysLock();
    // only set the bits that is related to this wheel
    uint32_t mask = 0xF << wheel;
    stateFlags = (stateFlags & ~mask) | ((state << wheel) & mask);

    // we use a timer here so we don't save 4 times in eeprom when 1 is enough
    // it takes 10ms each time and that would block thread
    chVTSet(&saveStateTimer, MS2ST(5), saveStateTimerCallback, NULL);
    chSysUnlock();
}


//static msg_t releaseWheel(uint8_t pin, volatile const uint16_t *currentp, eventflags_t adcAvtFlag)
static msg_t releaseWheel(wheelthreadinfo_t *info)
{
    BRIDGE_ENABLE;
    palSetPad(GPIOC, info->releasePin);
    msg_t success = MSG_OK;

    systime_t curOffTime = 0,
              maxTime = MS2ST(2000),
              revupTime = MS2ST(settings[S_TimeRevupRelease]); // start of release means higher currents is allowed

    bool isReleased = false;

    while (maxTime > chVTGetSystemTime()) {
        // measure current
        eventflags_t flg = chEvtWaitOneTimeout(info->evtReceiveFlag | EVENT_FLAG_BRIDGE_DIAG_PINS,
                                               US2ST(700));
        if (flg == 0) { // timeout check
            DEBUG_OUT("ADC error releasing");
            success = C_dtc_ADC_error_LF_release | info->wheel; // LSB:4 is the wheel, LF =0
            break;
        } else if (flg & EVENT_FLAG_BRIDGE_DIAG_PINS) {
            DEBUG_OUT("Hardware current limit tripped");
            success = C_dtc_OverCurrent_LF_release | info->wheel; // LSB:4 is the wheel, LF =0
            break;
        }

        uint16_t maxCurrent;

        // at first the current is allowed to be higher due to release force, (buildup torque)
        if (chVTGetSystemTime() < revupTime)
            maxCurrent = settings[S_CurrentRevupMaxRelease];
        else
            maxCurrent = settings[S_CurrentMaxRelease];

        // compare currents from the newly measured current (as we have been released from pad)
        if (*info->motorcurrent > maxCurrent) {
            // overCurrent protection
            palClearPad(GPIOC, info->releasePin);
            curOffTime = US2ST(500);
        } else if (curOffTime && curOffTime < chVTGetSystemTime()) {
            // restore current again from current protection
            curOffTime = 0;
            palSetPad(GPIOC, info->releasePin);
        } else if (chVTGetSystemTime() > revupTime) {
            // we are past revup time
            if (!isReleased && *info->motorcurrent < settings[S_CurrentFreeThreshold]) {
                // we are released but we continue for while
                // by changing maxTime so we can take advantage of current limit functionality
                isReleased = true;
                maxTime = MS2ST(settings[S_TimeContinueAfterFreed]);
            }
        }
    }

    // we are finished
    palClearPad(GPIOC, info->releasePin);

    return success;
}


// NOTE these to functions could possibly be merged,
// but special case them for now until we now if it can be generalized enough
static msg_t tightenWheel(wheelthreadinfo_t *info)
{
    BRIDGE_ENABLE;
    palSetPad(GPIOC, info->tightenPin);
    msg_t success = MSG_OK;

    systime_t curOffTime = 0,
           maxTime = MS2ST(2000),
           revupTime = MS2ST(settings[S_TimeRevupRelease]); // revup means higher currents is allowed

    bool isTightened = false;
    uint8_t avgSpeed = 0, lockSpeed = 0;

    while (maxTime > chVTGetSystemTime()) {
        // measure current
        eventflags_t flg = chEvtWaitOneTimeout(info->evtReceiveFlag | EVENT_FLAG_BRIDGE_DIAG_PINS,
                                               US2ST(700));
        if (flg == 0) { // timeout check
            DEBUG_OUT("ADC error tighten");
            success = C_dtc_ADC_error_LF_tighten | info->wheel; // LSB:4 is the wheel, LF =0
            break;
        } else if (flg & EVENT_FLAG_BRIDGE_DIAG_PINS) {
            DEBUG_OUT("Hardware current limit tripped");
            success = C_dtc_OverCurrent_LF_tighten | info->wheel; // LSB:4 is the wheel, LF =0
            break;
        }

        uint16_t maxCurrent;

        // at first the current is allowed to higher due to release force, (buildup torque)
        if (chVTGetSystemTime() < revupTime)
            maxCurrent = settings[S_CurrentRevupMaxRelease];
        else
            maxCurrent = settings[S_CurrentMaxRelease];

        // Anti lock functionality
        if (settings[S_AntilockParkbrake] != 0) {
            avgSpeed = sen_wheelAverage();
            if (lockSpeed  == 0 && avgSpeed > 1 &&
                ((*info->wheelSpeed) * 0.8) < avgSpeed)
            {
                // wheel is slipping more than 20% event
                lockSpeed = *info->wheelSpeed;
                palClearPad(GPIOC, info->tightenPin);
                palSetPad(GPIOC, info->releasePin);
            }
            if (lockSpeed > 0 && ((avgSpeed > (*info->wheelSpeed) * 0.9) || avgSpeed < 1))
            {
                // wheel is not slipping more than 10%,or vehicle is standing still
                // restore tighten operation again
                lockSpeed = 0;
                palClearPad(GPIOC, info->releasePin);
                palSetPad(GPIOC, info->tightenPin);
            }
        } // end antilock functionality


        // compare currents from the newly measured current (as we have been released from )
        if (*info->motorcurrent > maxCurrent) {
            // overCurrent protection
            palClearPad(GPIOC, info->tightenPin);
            curOffTime = US2ST(500);
        } else if (curOffTime && curOffTime < chVTGetSystemTime()) {
            // restore current again from current protection
            curOffTime = 0;
            palSetPad(GPIOC, info->tightenPin);
        } else if (chVTGetSystemTime() > revupTime) {
            // we are past revup time

            if (!isTightened && *info->motorcurrent > settings[S_CurrentTightenedThreshold])
            {
                // we are tightened but we continue for while
                // by changing maxTime so we can take advantage of current limit functionality
                isTightened = true;
                maxTime = MS2ST(settings[S_TimeContinueAfterTightened]);
            }
        }
    }

    // we are finished
    palClearPad(GPIOC, info->tightenPin);

    return success;
}


static msg_t serviceWheel(wheelthreadinfo_t *info)
{
    BRIDGE_ENABLE;
    palSetPad(GPIOC, info->releasePin);
    msg_t success = MSG_OK;

    systime_t curOffTime = 0,
              maxTime = MS2ST(5000),
              revupTime = MS2ST(settings[S_TimeRevupRelease]); // start of release means higher currents is allowed

    bool isReleased = false;

    while (maxTime > chVTGetSystemTime()) {
        // measure current
        eventflags_t flg = chEvtWaitOneTimeout(info->evtReceiveFlag | EVENT_FLAG_BRIDGE_DIAG_PINS,
                                               US2ST(700));
        if (flg == 0) { // timeout check
            DEBUG_OUT("ADC error service");
            success = C_dtc_ADC_error_LF_service | info->wheel; // LSB:4 is the wheel, LF =0
            break;
        } else if (flg & EVENT_FLAG_BRIDGE_DIAG_PINS) {
            DEBUG_OUT("Hardware current limit tripped");
            success = C_dtc_OverCurrent_LF_service | info->wheel; // LSB:4 is the wheel, LF =0
            break;
        }

        uint16_t maxCurrent;

        // at first the current is allowed to be higher due to release force, (buildup torque)
        if (chVTGetSystemTime() < revupTime)
            maxCurrent = settings[S_CurrentRevupMaxRelease];
        else
            maxCurrent = settings[S_CurrentMaxRelease];

        // compare currents from the newly measured current (as we have been released from pad)
        if (*info->motorcurrent > maxCurrent) {
            // overCurrent protection
            palClearPad(GPIOC, info->releasePin);
            curOffTime = US2ST(500);
        } else if (curOffTime && curOffTime < chVTGetSystemTime()) {
            // restore current again from current protection
            curOffTime = 0;
            palSetPad(GPIOC, info->releasePin);
        } else if (chVTGetSystemTime() > revupTime) {
            // we are past revup time
            if (!isReleased && *info->motorcurrent < settings[S_CurrentFreeThreshold]) {
                // we are released but we continue until current raises again
                // by changing maxTime so we can take advantage of current limit functionality
                isReleased = true;
            } else if (isReleased &&
                    *info->motorcurrent > settings[S_CurrentFreeThreshold] +2000)
            {
                // break when we are release and current rise again 2amps above released threshold
                break;
            }

        }
    }

    // we are finished
    palClearPad(GPIOC, info->releasePin);

    return success;
}


// ---------------------------------------------------------------------------------------
// begin API functions

void ctrl_init(void)
{
    // read state from EEPROM
    fileStreamSeek(stateFile, 0);
    stateFlags = EepromReadWord(stateFile);
    ctrl_checkForErrors();

    // init mailboxes to the threads
    chMBObjectInit(&mbLF, mbLF_queue, MAILBOX_SIZE);
    chMBObjectInit(&mbRF, mbRF_queue, MAILBOX_SIZE);
    chMBObjectInit(&mbLR, mbLR_queue, MAILBOX_SIZE);
    chMBObjectInit(&mbRR, mbRR_queue, MAILBOX_SIZE);

    // timer which disables bridge when all wheels are done
    chVTObjectInit(&bridgeDisableTimer);
    // timer which saves a changed state in eeprom (later so we don't write to many times)
    chVTObjectInit(&saveStateTimer);

    // init wheel control threads
    lfThdp = chThdCreateStatic(&waLeftFrontHandler, WHEEL_THD_STACK_SIZE,
                               NORMALPRIO, wheelHandler, &lfInfo);
    rfThdp = chThdCreateStatic(&waRightFrontHandler, WHEEL_THD_STACK_SIZE,
                               NORMALPRIO, wheelHandler, &rfInfo);
    lrThdp = chThdCreateStatic(&waLeftRearHandler, WHEEL_THD_STACK_SIZE,
                               NORMALPRIO, wheelHandler, &lrInfo);
    rrThdp = chThdCreateStatic(&waRightRearHandler, WHEEL_THD_STACK_SIZE,
                               NORMALPRIO, wheelHandler, &rrInfo);

}

void ctrl_setStateAll(ctrl_states state)
{
    ctrl_setStateAxle(state, FrontAxle);
    ctrl_setStateAxle(state, RearAxle);
}

void ctrl_setStateAxle(ctrl_states state, ctrl_axles axle)
{
    chDbgAssert(axle > RearAxle, "Invalid axle");

    if  (axle == RearAxle) {
        ctrl_setStateWheel(state, LeftRear);
        ctrl_setStateWheel(state, RightRear);
    } else {
        ctrl_setStateWheel(state, LeftFront);
        ctrl_setStateWheel(state, RightFront);
    }
}

// set on a single wheel
void ctrl_setStateWheel(ctrl_states state, ctrl_wheels wheel)
{
    chDbgAssert(state != Releasing &&
                state != Tightening &&
                state != SetServiceState, "Invalid state");
    chDbgAssert(wheel > RightRear, "Invalid wheel");

    // wheel is already at this state
    if (state == STATE_MASK(wheel))
        return;

    // if we have gotten here we good, save new state!
    saveState(state, wheel);

    switch (wheel) {
    case LeftFront:
        chMBPost(&mbLF, (msg_t)state, TIME_IMMEDIATE);
        break;
    case LeftRear:
        chMBPost(&mbLR, (msg_t)state, TIME_IMMEDIATE);
        break;
    case RightFront:
        chMBPost(&mbRF, (msg_t)state, TIME_IMMEDIATE);
        break;
    case RightRear:
        chMBPost(&mbRR, (msg_t)state, TIME_IMMEDIATE);
        break;
    }
}

ctrl_states ctrl_getStateWheel(ctrl_wheels wheel)
{
    return STATE_MASK(wheel);
}

int ctrl_checkForErrors(void)
{
    int errorCnt = 0;
    if (LF_STATE & ErrorState) {
        DEBUG_OUT("Left front errstate");
        ++errorCnt;
    }
    if (RF_STATE & ErrorState) {
        DEBUG_OUT("Right front errstate");
        ++errorCnt;
    }
    if (LR_STATE & ErrorState) {
        DEBUG_OUT("Left rear errstate");
        ++errorCnt;
    }
    if (RR_STATE & ErrorState) {
        DEBUG_OUT("Right rear errstate");
        ++errorCnt;
    }
    return errorCnt;
}