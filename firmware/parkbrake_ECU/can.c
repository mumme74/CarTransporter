/*
 * can.c
 *
 *  Created on: 30 jul 2017
 *      Author: fredrikjohansson
 */

#include "can.h"
#include "hal.h"
#include "ch.h"
#include "can_protocol.h"
#include "control.h"
#include "debug.h"





// -----------------------------------------------------------------------------------------
// begin private variables and configurations for this module

static thread_t *canRxThdp;

static CANConfig canCfg = {
    // MCR register (Master control register)
    CAN_MCR_AWUM | CAN_MCR_ABOM, // auto wakeup, auto bus off
    // BTR register (Bit timing register)
    CAN_BTR_SJW(0) |
    CAN_BTR_TS2(2U) | // Synchronization width=0, time quanta TS2 (sample point+transmit point)
    CAN_BTR_TS1(13U) | // time quanta on bit quanta ts1 (sample point)
    CAN_BTR_BRP(18U-1)    // Baud rate prescaler
    /**
     * |<------------------------nominal bit time--------------------------->|
     * |--------------|---------------------------------|--------------------|
     * | sync segment |        bit segment 1            |   bit segment 2    |
     *    1xtq                  tq x (TS1+1)            |     tq x (TS2+1)   |
     *   (timequanta)              tBS1                 ^       tBS2         ^
     *                                             sample point         Transmit point
     *
     * BaudRate = 1/NominalBitTime
     * NominalBitTime = 1 x tq + tBS1 + tBS2
     * tBS1 = tq x (TS1 + 1)
     * tBS2 = tq x (TS2 + 1)
     * tq = (BRP +1) x tPCLK
     * tPCLK = time period of APB clock
     *
     * We want sample point to be at 87.5% (CANopen standard)
     * calcluator http://www.bittiming.can-wiki.info/
     * suggests:
     *  Div: 9,  TS1: 13,TS2: 2; total 16 tq
     *  Div: 18, TS1: 6, TS2: 1; total 8 tq
     *
     *  NOTE! BTR is Div -1
     *
     * So 36Mhz APB clock and we want 250kbits
     * 1bit should be: 4us long.
     * (1/36)*9 = 0,25us -> 0,25*16tq = 4us
     * (1/36)*18 = 0,5us -> 0,5*8tq   = 4us
     *
     */

    /* | CAN_BTR_LBKM // loopback mode */
    /* | CAN_BTR_SILM // Silent mode*/
};


// ----------------------------------------------------------------------------------------
// begin function prototypes

static void processRx(CANRxFrame *rxf)
{
    static uint16_t sid; // save stackspace

    sid = rxf->SID & CAN_MSG_TYPE_MASK;
    if (sid == CAN_MSG_TYPE_ERROR) {
        // not interested... TODO filter out
    } else if (sid == CAN_MSG_TYPE_UPDATE) {
        // not sure if we're really interested in anything here
    } else if (sid == CAN_MSG_TYPE_DIAG) {
        // DTCs data list etc
    } else if (sid == CAN_MSG_TYPE_COMMAND) {
        // set in service and so forth
        sid = rxf->SID & CAN_MSG_ID_MASK;
        if (sid < C_parkbrakeCmd_first || sid > C_parkbrakeCmd_last)
            return; // not meant for this node
        if (rxf->RTR != CAN_RTR_REMOTE) {
            DEBUG_OUT("CAN RTR not set on command");
            return;
        }

        ctrl_states state;
        switch (sid) {
        case C_parkbrakeServiceSet:
            state = ServiceState;
            break;
        case C_parkbrakeServiceUnset:
            state = Tightening;
            break;
        default:
            DEBUG_OUT_PRINTF("Non allowed CAN cmd: %x", rxf->SID & CAN_MSG_ID_MASK);
            return;
        }

        // do the state change
        ctrl_setStateAll(state);
        if (ctrl_checkForErrors() != 0) {
            // error changing state!!
            CANTxFrame txf;
            txf.DLC = 1;
            txf.RTR = CAN_RTR_DATA;
            txf.IDE = CAN_IDE_STD;
            txf.SID = rxf->SID;
            txf.data8[0] = 0;
            canTransmitTimeout(&CAND1, CAN_ANY_MAILBOX, &txf, MS2ST(10));
        }
    } // else should not be possible due protocol definition
}
// ----------------------------------------------------------------------------------------
// begin callbacks

// ----------------------------------------------------------------------------------------
// begin private helper functions and threads


static THD_WORKING_AREA(waCanRxThd, 256);
static THD_FUNCTION(canRxThd, arg)
{
    event_listener_t el;
    CANRxFrame rxmsg;
    (void)arg;
    chEvtRegister(&CAND1.rxfull_event, &el, 0);
    chRegSetThreadName("canRxThd");

    while (TRUE) {
        if (chEvtWaitAnyTimeout(ALL_EVENTS, MS2ST(100)) == 0)
          continue;
        while (canReceive(&CAND1, CAN_ANY_MAILBOX, &rxmsg, TIME_IMMEDIATE) == MSG_OK) {
            /* Process message.*/
            processRx(&rxmsg);
        }
    }
}



// ----------------------------------------------------------------------------------------
// begin public API for this module

void can_init(void)
{
    canStart(&CAND1, &canCfg);

    // start rx thread listener
    canRxThdp = chThdCreateStatic(&waCanRxThd, 256, NORMALPRIO -1, canRxThd, NULL);
    chThdStart(canRxThdp);
}

