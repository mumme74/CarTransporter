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
#include "diag.h"

#define MAILBOX_SIZE 10

// -----------------------------------------------------------------------------------------
// begin private variables and configurations for this module

static thread_t *canRxThdp,
                *canPIDPeriodicSendThdp,
                *canCmdSetThdp;

static mailbox_t cmdSet_MB;

static msg_t cmdSetMBqueue[MAILBOX_SIZE];


static CANConfig canCfg = {
    // MCR register (Master control register)
    CAN_MCR_AWUM | CAN_MCR_ABOM, // auto wakeup, auto bus off
    // BTR register (Bit timing register)
    CAN_BTR_SJW(0) |
    CAN_BTR_TS2(2U) | // Synchronization width=0, time quanta TS2 (sample point+transmit point)
    CAN_BTR_TS1(13U) | // time quanta on bit quanta ts1 (sample point)
    CAN_BTR_BRP(7U)    // Baud rate prescaler
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
//static bool isRemoteFrame(CANRxFrame *rxf);
static void processRx(CANRxFrame *rxf);
static void broadcastOnCan(can_msgIdsUpdate_e PID);
static void sendCurrents(void);

// ----------------------------------------------------------------------------------------
// begin callbacks
static void processRx(CANRxFrame *rxf)
{
    uint16_t sid; // save stackspace

    sid = rxf->SID & CAN_MSG_TYPE_MASK;
    if (sid == CAN_MSG_TYPE_EXCEPTION) {
        // not interested in incoming exceptions... TODO filter out
    } else if (sid == CAN_MSG_TYPE_UPDATE) {
        // our own updates send by a dedicated thread
        // remote Node requests update frame
      sid = rxf->SID & CAN_MSG_ID_MASK;
      if (sid >= C_parkbrakeUpdPID_FIRST && sid <= C_parkbrakeUpdPID_LAST)
        broadcastOnCan((can_msgIdsUpdate_e)sid);

    } else if (sid == CAN_MSG_TYPE_DIAG) {
        // DTCs data list etc
        sid = rxf->SID & CAN_MSG_ID_MASK;
        if (sid < C_parkbrakeDiag_FIRST || sid > C_parkbrakeDiag_LAST)
            return; // not meant for this node

        switch (sid) {
        case C_parkbrakeDiagDTCLength: {
//            if (!isRemoteFrame(rxf))
//                return;
            msg_t msg = C_parkbrakeDiagDTCLength << 16;
            chMBPost(&diag_CanMB, msg, TIME_IMMEDIATE);
        }   return;
        case C_parkbrakeDiagDTC: // fallthrough
        case C_parkbrakeDiagDTCFreezeFrame: {
            if (rxf->DLC != 1) {
                DEBUG_OUT_PRINTF("malformed CAN diag: %x wrong DLC", rxf->SID);
                return;
            }
            if (rxf->data8[0] >= dtc_length()) {
                DEBUG_OUT_PRINTF("malformed CAN diag: %x wrong DTC idx", rxf->data8[0]);
                return;
            }
            msg_t msg = (sid << 16) | rxf->data8[0];
            chMBPost(&diag_CanMB, msg, TIME_IMMEDIATE);
        }    return;

        case C_parkbrakeDiagClearDTCs: {
            if (rxf->DLC != 1) {
                DEBUG_OUT_PRINTF("malformed CAN diag erase dtc: %x wrong DLC", rxf->SID);
                return;
            }
            if (rxf->data8[0] > dtc_length()) {
                DEBUG_OUT_PRINTF("malformed CAN diag erase dtc: %x wrong DTC len", rxf->data8[0]);
                return;
            }
            msg_t msg = (sid << 16) | rxf->data8[0];
            chMBPost(&diag_CanMB, msg, TIME_IMMEDIATE);
        } return;
        case C_parkbrakeDiagActuatorTest:
            if (rxf->DLC != 1) {
                DEBUG_OUT_PRINTF("malformed CAN diag erase dtc: %x wrong DLC", rxf->SID);
                return;
            }
            msg_t msg = (sid << 16) | rxf->data8[0];
            chMBPost(&diag_CanMB, msg, TIME_IMMEDIATE);

        } return;
    } else if (sid == CAN_MSG_TYPE_COMMAND) {
        // set in service and so forth
        sid = rxf->SID & CAN_MSG_ID_MASK;
        if (sid < C_parkbrakeCmd_first || sid > C_parkbrakeCmd_last)
            return; // not meant for this node

        switch (sid) {
        case C_parkbrakeCmdSetConfig: {
            if (rxf->DLC != 3) {
                DEBUG_OUT_PRINTF("malformed CAN config cmd: %x wrong DLC", rxf->SID);
                return;
            }
            if (rxf->data8[0] >= S_EOF) {
                DEBUG_OUT_PRINTF("malformed CAN config cmd: %d index out of range", rxf->data8[0]);
                return;
            }
            // bitstuff our msg to fit in 32bits
            msg_t msg = rxf->data8[1] | (rxf->data8[2] << 8); // 2 lowest bytes are settings value
            msg |= (rxf->data8[0] & 0x7F) << 16;// these bits is idx
            msg |= C_parkbrakeCmdSetConfig << 20;// 2 highest bytes is action

            chMBPost(&cmdSet_MB, msg, TIME_IMMEDIATE);

        } return;
        case C_parkbrakeCmdGetConfig: {
            if (rxf->DLC != 1) {
                DEBUG_OUT_PRINTF("malformed CAN config cmd: %x wrong DLC", rxf->SID);
                return;
            }
            if (rxf->data8[0] >= S_EOF) {
                DEBUG_OUT_PRINTF("malformed CAN config cmd: %d index out of range", rxf->data8[0]);
                return;
            }

            //is valid, send response
            CANTxFrame txf;
            can_initTxFrame(&txf, CAN_MSG_TYPE_COMMAND, C_parkbrakeCmdGetConfig);
            txf.DLC = 3;
            txf.data8[0] = rxf->data8[0];
            txf.data8[1] = settings[rxf->data8[0]] & 0x00FF; // little byte first
            txf.data8[2] = (settings[rxf->data8[0]] & 0xFF00) >> 8;
            canTransmitTimeout(&CAND1, CAN_ANY_MAILBOX, &txf, MS2ST(10));

        } return;
        case C_parkbrakeCmdGetState: {
            CANTxFrame txf;
            can_initTxFrame(&txf, CAN_MSG_TYPE_COMMAND, C_parkbrakeCmdGetState);
            txf.DLC = 4;
            txf.data8[0] = ctrl_getStateWheel(LeftFront);
            txf.data8[1] = ctrl_getStateWheel(RightFront);
            txf.data8[2] = ctrl_getStateWheel(LeftRear);
            txf.data8[3] = ctrl_getStateWheel(RightRear);
            canTransmitTimeout(&CAND1, CAN_ANY_MAILBOX, &txf, MS2ST(10));
        }   return;
        case C_parkbrakeCmdServiceSet: {
            chMBPost(&cmdSet_MB, (msg_t)C_parkbrakeCmdServiceSet, TIME_IMMEDIATE);
        }   return;
        case C_parkbrakeCmdServiceUnset:
            // do the state change
            chMBPost(&cmdSet_MB, (msg_t)C_parkbrakeCmdServiceUnset, TIME_IMMEDIATE);
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
/*
static bool isRemoteFrame(CANRxFrame *rxf)
{
    if (rxf->RTR != CAN_RTR_REMOTE) {
        DEBUG_OUT_PRINTF("CAN RTR not set on %x", rxf->SID);
        return false;
    }
    return true;
}
*/

// send update pids of volts wheel brakes status etc
// C_parkbrakePID_1 and C_parkbrakePID_3
static void broadcastOnCan(can_msgIdsUpdate_e PID)
{
    static uint64_t oldPid_1 = 0,
                    oldPid_2 = 0,
                    oldPid_3 = 0;

    // build data for PID_1
    CANTxFrame txf;
    can_buildPid1Data(txf.data8);

    if (txf.data64[0] != oldPid_1 || PID == C_parkbrakeUpdPID_1) {
        // something changed, broadcast
        oldPid_1 = txf.data64[0];
        can_initTxFrame(&txf, CAN_MSG_TYPE_UPDATE, C_parkbrakeUpdPID_1);
        txf.DLC = 8;
        canTransmitTimeout(&CAND1, CAN_ANY_MAILBOX, &txf, TIME_IMMEDIATE);
    }

      // PID_2
      can_buildPid2Data(txf.data8);
    if (txf.data64[0] != oldPid_2 || PID == C_parkbrakeUpdPID_2) {
        // something changed, broadcast
        oldPid_2 = txf.data64[0];
        can_initTxFrame(&txf, CAN_MSG_TYPE_UPDATE, C_parkbrakeUpdPID_2);
        txf.DLC = 8;
        canTransmitTimeout(&CAND1, CAN_ANY_MAILBOX, &txf, TIME_IMMEDIATE);
    }

    // build data for PID_3
    can_buildPid3Data(txf.data8);

    if (txf.data64[0] != oldPid_3 || PID == C_parkbrakeUpdPID_3) {
        // something changed, broadcast
        oldPid_3 = txf.data64[0];
        can_initTxFrame(&txf, CAN_MSG_TYPE_UPDATE, C_parkbrakeUpdPID_3);
        txf.DLC = 6;
        canTransmitTimeout(&CAND1, CAN_ANY_MAILBOX, &txf, TIME_IMMEDIATE);
    }
}

static void sendCurrents(void)
{
    CANTxFrame txf;
    can_initTxFrame(&txf, CAN_MSG_TYPE_UPDATE, C_parkbrakeUpdPID_2);

    can_buildPid2Data(txf.data8);

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
//        if (chEvtWaitAnyTimeout(ALL_EVENTS, MS2ST(100)) == 0)
//          continue;
        while (canReceive(&CAND1, CAN_ANY_MAILBOX, &rxmsg, TIME_INFINITE) == MSG_OK) {
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

    event_listener_t elFront, elRear;
    chEvtRegisterMask(&sen_measuredEvts, &elFront, AdcFrontAxle);
    chEvtRegisterMask(&sen_measuredEvts, &elRear, AdcRearAxle);
    static const uint16_t broadcastTime = 500; // ms
    systime_t pid2_timeout = 0;

    while (TRUE) {
        eventflags_t flg = chEvtWaitAnyTimeout(AdcRearAxle | AdcFrontAxle,  //EVENT_FLAG_ADC_REARAXLE | EVENT_FLAG_ADC_FRONTAXLE,
                                            MS2ST(broadcastTime));
        if (flg == 0) {
            // timeout, try periodic send
            broadcastOnCan(C_NoUpdateFrame);
        } else if (pid2_timeout < chVTGetSystemTime()) {
            // send currents each 100ms
            pid2_timeout = MS2ST(100);
            sendCurrents();
        }
    }
}


// operations in this thread blocks, so we don't process them in rxHandlerthd
static THD_WORKING_AREA(waCanCmdSet, 350);
static THD_FUNCTION(canCmdSet, arg)
{
    (void)arg;
    chRegSetThreadName("canCmdSet");

    static CANTxFrame txf;

    while(TRUE) {
      static msg_t msg;
      if (chMBFetch(&cmdSet_MB, &msg, TIME_INFINITE) != MSG_OK)
          continue;

      static uint16_t settingsVlu;
      static uint8_t     settingsIdx;
      static can_msgIdsCommand_e action;

      settingsVlu  =  msg & 0x0000FFFF; // 2 lowest bytes are settings value
      settingsIdx  = (msg & 0x007F0000) >> 16; // lowest bytes is idx
      action       = (msg & 0xFF800000) >> 20; // 2 highest bytes is action, parkbrakeCmds begin at 0x10 << 3

      switch (action) {
      case C_parkbrakeCmdSetConfig: {
        ee_changeSetting(settingsIdx, settingsVlu);
        can_initTxFrame(&txf, CAN_MSG_TYPE_COMMAND, C_parkbrakeCmdSetConfig);
        txf.DLC = 2;
        txf.data8[0] = settingsIdx;
        txf.data8[1] = ee_saveSetting(settingsIdx) > 0 ? 0xAA : 0;
      } break;
      case C_parkbrakeCmdServiceSet: {
        can_initTxFrame(&txf, CAN_MSG_TYPE_COMMAND, C_parkbrakeCmdServiceSet);
        txf.DLC = 1;
        if (sen_vehicleSpeed() ==0 )
          txf.data8[0] = ctrl_setStateAll(SetServiceState) > 0 ? 0xAA : 0;
        else
          txf.data8[0] = 0;
      } break;
      case C_parkbrakeCmdServiceUnset: {
        can_initTxFrame(&txf, CAN_MSG_TYPE_COMMAND, C_parkbrakeCmdServiceUnset);
        txf.DLC = 1;
        if (sen_vehicleSpeed() ==0 )
          txf.data8[0] = ctrl_setStateAll(Tightening) > 0 ? 0xAA : 0;
        else
          txf.data8[0] = 0;
      } break;
      default:
        continue; // do nothing, bust out to next msg in mailbox
      }

      // send response
      canTransmitTimeout(&CAND1, CAN_ANY_MAILBOX, &txf, MS2ST(10));
    }
}

// ----------------------------------------------------------------------------------------
// begin public API for this module

void can_init(void)
{
    canStart(&CAND1, &canCfg);

    // start rx thread listener
    canRxThdp = chThdCreateStatic(&waCanRxThd, sizeof(waCanRxThd), NORMALPRIO -1, canRxThd, NULL);

    // start periodic PID thread (broadcast sensor values on CAN)
    canPIDPeriodicSendThdp = chThdCreateStatic(&waCanPIDPeriodicSend, sizeof(waCanPIDPeriodicSend),
                                               NORMALPRIO -5, canPIDPeriodicSend, NULL);

    // blocking cmds thd
    chMBObjectInit(&cmdSet_MB, cmdSetMBqueue, MAILBOX_SIZE);
    canCmdSetThdp = chThdCreateStatic(&waCanCmdSet, sizeof(waCanCmdSet),
                                      NORMALPRIO -2, canCmdSet, NULL);
}

// requires a uint8_t[8] buffer
void can_buildPid1Data(uint8_t data8[])
{

    data8[0] = ctrl_getStateWheel(LeftFront);
    data8[1] = ctrl_getStateWheel(RightFront);
    data8[2] = ctrl_getStateWheel(LeftRear);
    data8[3] = ctrl_getStateWheel(RightRear);
    data8[4] = sen_wheelSpeeds.leftFront_rps;
    data8[5] = sen_wheelSpeeds.rightFront_rps;
    data8[6] = sen_wheelSpeeds.leftRear_rps;
    data8[7] = sen_wheelSpeeds.rightRear_rps;
}

// requires a uint8_t[8] buffer
void can_buildPid2Data(uint8_t data8[])
{

    data8[0] = sen_motorCurrents.leftFront / 1000; // mA -> A
    data8[1] = sen_motorCurrents.rightFront / 1000; // mA -> A
    data8[2] = sen_motorCurrents.leftRear / 1000; // mA -> A
    data8[3] = sen_motorCurrents.rightRear / 1000; // mA -> A

    data8[4] = sen_motorCurrents.maxLeftFront / 1000; // mA -> A
    data8[5] = sen_motorCurrents.maxRightFront / 1000; // mA -> A
    data8[6] = sen_motorCurrents.maxLeftRear / 1000; // mA -> A
    data8[7] = sen_motorCurrents.maxRightRear / 1000; // mA -> A
}

// requires a uint8_t[5] buffer
void can_buildPid3Data(uint8_t data8[])
{

    data8[0] = (sen_voltages.batVolt & 0x00FF); // little endian
    data8[1] = (sen_voltages.batVolt & 0xFF00) >> 8;

    data8[2] = (sen_voltages.ignVolt & 0x00FF); // little endian
    data8[3] = (sen_voltages.ignVolt & 0xFF00) >> 8;

    data8[4] = sen_chipTemperature; // as signed int

    data8[5] = (SEN_IGN_ON_SIG << 7)      | (SEN_LIGHTS_ON_SIG << 6) |
               (SEN_BUTTON_SIG << 5)      | (SEN_BUTTON_INV_SIG << 4) |
               (SEN_LEFT_FRONT_DIAG << 3) | (SEN_RIGHT_FRONT_DIAG << 2) |
               (SEN_LEFT_REAR_DIAG << 1)  | (SEN_RIGHT_REAR_DIAG << 0);
}

void can_initTxFrame(CANTxFrame *txf, uint16_t msgType, uint16_t msgId)
{
  txf->IDE = CAN_IDE_STD;;
  txf->EID = 0;
  txf->RTR = 0;
  txf->SID = msgType | msgId | C_parkbrakeNode;
}
