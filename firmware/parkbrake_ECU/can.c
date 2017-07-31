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
#include "eeprom_setup.h"
#include "sensors.h"

// -----------------------------------------------------------------------------------------
// begin private variables and configurations for this module

static thread_t *canRxThdp,
                *canPIDPeriodicSendp;

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
static bool isRemoteFrame(CANRxFrame *rxf);
static void processRx(CANRxFrame *rxf);
static void initTxFrame(CANTxFrame *txf, uint16_t msgType, uint16_t msgId);
static void broadcastOnCan(void);
static void sendCurrents(void);

// ----------------------------------------------------------------------------------------
// begin callbacks
static void processRx(CANRxFrame *rxf)
{
    uint16_t sid; // save stackspace

    sid = rxf->SID & CAN_MSG_TYPE_MASK;
    if (sid == CAN_MSG_TYPE_ERROR) {
        // not interested... TODO filter out
    } else if (sid == CAN_MSG_TYPE_UPDATE) {
        // not sure if we're really interested in anything here
        // our own updates send by a dedicated thread
    } else if (sid == CAN_MSG_TYPE_DIAG) {
        // DTCs data list etc
    } else if (sid == CAN_MSG_TYPE_COMMAND) {
        // set in service and so forth
        sid = rxf->SID & CAN_MSG_ID_MASK;
        if (sid < C_parkbrakeCmd_first || sid > C_parkbrakeCmd_last)
            return; // not meant for this node

        switch (sid) {
        case C_parkbrakeSetConfig: {
            if (rxf->DLC != 3) {
                DEBUG_OUT_PRINTF("malformed CAN config cmd: %x wrong DLC", rxf->SID);
                return;
            }
            uint8_t idx = rxf->data8[0];
            if (idx >= S_EOF) {
                DEBUG_OUT_PRINTF("malformed CAN config cmd: %d index out of range", idx);
                return;
            }
            uint16_t vlu = rxf->data8[1] & (rxf->data8[2] << 8); // little endian
            ee_changeSetting(idx, vlu);
            ee_saveSetting(idx);
        } return;
        case C_parkbrakeGetConfig: {
            if (rxf->DLC != 1) {
                DEBUG_OUT_PRINTF("malformed CAN config cmd: %x wrong DLC", rxf->SID);
                return;
            }
            uint8_t idx = rxf->data8[0];
            if (idx >= S_EOF) {
                DEBUG_OUT_PRINTF("malformed CAN config cmd: %d index out of range", idx);
                return;
            }

            //is valid, send response
            CANTxFrame txf;
            initTxFrame(&txf, CAN_MSG_TYPE_COMMAND, sid);
            txf.DLC = 3;
            txf.data8[0] = idx;
            txf.data8[1] = settings[idx] & 0x00FF; // little byte first
            txf.data8[2] = (settings[idx] & 0xFF00) >> 8;
            canTransmitTimeout(&CAND1, CAN_ANY_MAILBOX, &txf, MS2ST(10));

        } return;
        case C_parkbrakeGetState: {
            if (!isRemoteFrame(rxf))
                return;
            CANTxFrame txf;
            initTxFrame(&txf, CAN_MSG_TYPE_COMMAND, sid);
            txf.DLC = 4;
            txf.data8[0] = ctrl_getState(LeftFront);
            txf.data8[1] = ctrl_getState(RightFront);
            txf.data8[2] = ctrl_getState(LeftRear);
            txf.data8[3] = ctrl_getState(RightRear);
            canTransmitTimeout(&CAND1, CAN_ANY_MAILBOX, &txf, MS2ST(10));
        }   return;
        case C_parkbrakeServiceSet:
            if (!isRemoteFrame(rxf))
                return;
            ctrl_setStateAll(SetServiceState);
            return;
        case C_parkbrakeServiceUnset:
            if (!isRemoteFrame(rxf))
                return;
            // do the state change
            ctrl_setStateAll(Tightening);
            return;
        default:
            DEBUG_OUT_PRINTF("Non allowed CAN cmd: %x", rxf->SID);
            return;
        }

    // end Command
    } // else should not be possible due protocol definition
}
// ----------------------------------------------------------------------------------------
// begin private helper functions

static bool isRemoteFrame(CANRxFrame *rxf)
{
    if (rxf->RTR != CAN_RTR_REMOTE) {
        DEBUG_OUT_PRINTF("CAN RTR not set on %x", rxf->SID);
        return false;
    }
    return true;
}

static void initTxFrame(CANTxFrame *txf, uint16_t msgType, uint16_t msgId)
{
    txf->SID = msgType | msgId | C_parkbrakeNode;
    txf->IDE = CAN_IDE_STD;
}

// send update pids of volts wheel brakes status etc
// C_parkbrakePID_1 and C_parkbrakePID_3
static void broadcastOnCan(void)
{
    static uint64_t oldPid_1 = 0,
                    oldPid_3 = 0;

    // build data for PID_1
    CANTxFrame txf;
    txf.data8[0] = ctrl_getState(LeftFront);
    txf.data8[1] = ctrl_getState(RightFront);
    txf.data8[2] = ctrl_getState(LeftRear);
    txf.data8[3] = ctrl_getState(RightRear);
    txf.data8[4] = sen_wheelSpeeds.leftFront_rps;
    txf.data8[5] = sen_wheelSpeeds.rightFront_rps;
    txf.data8[6] = sen_wheelSpeeds.leftRear_rps;
    txf.data8[7] = sen_wheelSpeeds.rightRear_rps;
    if (txf.data64[0] != oldPid_1) {
        // something changed, broadcast
        oldPid_1 = txf.data64[0];
        initTxFrame(&txf, CAN_MSG_TYPE_UPDATE, C_parkbrakePID_1);
        txf.DLC = 8;
        canTransmitTimeout(&CAND1, CAN_ANY_MAILBOX, &txf, TIME_IMMEDIATE);
    }

    // build data for PID_3
    txf.data8[0] = (sen_voltages.batVolt & 0x00FF); // little endian
    txf.data8[1] = (sen_voltages.batVolt & 0xFF00) >> 8;

    txf.data8[2] = (sen_voltages.ignVolt & 0x00FF); // little endian
    txf.data8[3] = (sen_voltages.ignVolt & 0xFF00) >> 8;

    txf.data8[4] = sen_chipTemperature; // as signed int

    txf.data8[5] = (SEN_IGN_ON_SIG << 7)      | (SEN_LIGHTS_ON_SIG << 6) |
                   (SEN_BUTTON_SIG << 5)      | (SEN_BUTTON_INV_SIG << 4) |
                   (SEN_LEFT_FRONT_DIAG << 3) | (SEN_RIGHT_FRONT_DIAG << 2) |
                   (SEN_LEFT_REAR_DIAG << 1)  | (SEN_RIGHT_REAR_DIAG << 0);

    if (txf.data64[0] != oldPid_3) {
        // something changed, broadcast
        oldPid_3 = txf.data64[0];
        initTxFrame(&txf, CAN_MSG_TYPE_UPDATE, C_parkbrakePID_3);
        txf.DLC = 6;
        canTransmitTimeout(&CAND1, CAN_ANY_MAILBOX, &txf, TIME_IMMEDIATE);
    }
}

static void sendCurrents(void)
{
    CANTxFrame txf;
    initTxFrame(&txf, CAN_MSG_TYPE_UPDATE, C_parkbrakePID_2);

    txf.data8[0] = sen_motorCurrents.leftFront / 1000; // mA -> A
    txf.data8[1] = sen_motorCurrents.rightFront / 1000; // mA -> A
    txf.data8[2] = sen_motorCurrents.leftRear / 1000; // mA -> A
    txf.data8[3] = sen_motorCurrents.rightRear / 1000; // mA -> A

    txf.data8[4] = sen_motorCurrents.maxLeftFront / 1000; // mA -> A
    txf.data8[5] = sen_motorCurrents.maxRightFront / 1000; // mA -> A
    txf.data8[6] = sen_motorCurrents.maxLeftRear / 1000; // mA -> A
    txf.data8[7] = sen_motorCurrents.maxRightRear / 1000; // mA -> A

    txf.DLC = 8;
    canTransmitTimeout(&CAND1, CAN_ANY_MAILBOX, &txf, TIME_IMMEDIATE);
}
// ----------------------------------------------------------------------------------------
// begin threads

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

static THD_WORKING_AREA(waCanPIDPeriodicSend, 256);
static THD_FUNCTION(canPIDPeriodicSend, arg)
{
    (void)arg;
    chRegSetThreadName("canPIDPeriodicSend");

    event_listener_t el;
    chEvtRegisterMaskWithFlags(&sen_measuredEvts, &el, EVENT_MASK(0),
                    EVENT_FLAG_ADC_REARAXLE | EVENT_FLAG_ADC_FRONTAXLE);
    static const uint16_t broadcastTime = 500; // ms
    systime_t pid2_timeout = 0;

    while (TRUE) {
        eventflags_t flg = chEvtWaitAnyTimeout(EVENT_FLAG_ADC_REARAXLE | EVENT_FLAG_ADC_FRONTAXLE,
                                            MS2ST(broadcastTime));
        if (flg == 0) {
            // timeout, try periodic send
            broadcastOnCan();
        } else if (pid2_timeout < chVTGetSystemTime()) {
            // send currents each 100ms
            pid2_timeout = MS2ST(100);
            sendCurrents();
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

    // start periodic PID thread (broadcast sensor values on CAN)
    canPIDPeriodicSendp = chThdCreateStatic(&waCanPIDPeriodicSend, 256, NORMALPRIO -5,
                                            canPIDPeriodicSend, NULL);
    chThdStart(canPIDPeriodicSendp);
}

