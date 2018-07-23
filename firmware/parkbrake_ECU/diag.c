/*
 * dtc.c
 *
 *  Created on: 4 aug 2017
 *      Author: fredrikjohansson
 */

#include "diag.h"
#include "can.h"
#include "ch.h"
#include "hal.h"
#include "hal_eeprom.h"
#include "eeprom_setup.h"
#include "can_protocol.h"
#include "sensors.h"
#include "control.h"
#include "debug.h"

#define MAILBOX_SIZE 10

#define DIAG_CAN_TIMEOUT MS2ST(25)

#define MAX_DTCS 34
#define DTC_SIZE 27  /* each DTC occupies 26byte including freeze frame*/


// ---------------------------------------------------------------------------------------
// begin public exported variables

mailbox_t dtc_MB,
          diag_CanMB;

// ---------------------------------------------------------------------------------------
// begin private variables and function prototypes
static thread_t *setDtcp,
                *diagCanThdp;

static msg_t dtcMBqueue[MAILBOX_SIZE],
             diagCanMBqueue[MAILBOX_SIZE];

static systime_t startupTime = 0;

static uint16_t storedDtcs[MAX_DTCS +1];
static uint8_t storedDtcLength;

// ---------------------------------------------------------------------------------------
// begin helper functions for this module

/*
 * Eeprom memory for DTC layout
 * #adr 0 [0:7] number of stored dtc
 *
 * #adr 1..100:
 *   2by    1by          2by           8by      8by        5by = 26bytes / dtc with freeze frame
 * [b0-b1] [b2]        [b3-b4]       [b5-b12] [b13-b20] [b21-b25] bytes
 * u[0-15] [16-23]     [24-39]       [39-95]  [96-159]  [b160-207] bits
 * dtc-nr  occurences  time since st  pid_1     pid_2     pid_3
 */
void initDtcListFromEeprom(void)
{
    // fill from eeprom
    if (fileStreamSeek(dtcFile, 0) == 0) {
        storedDtcLength = EepromReadHalfword(dtcFile);
        if (storedDtcLength > MAX_DTCS)
          storedDtcLength = MAX_DTCS; // a DTC is uint16_t
    }

    msg_t i;
    for (i = 0; i < storedDtcLength; ++i) {
        msg_t pos = (i * DTC_SIZE) + 1; // +1 because #0 holds dtc length
        if (fileStreamSeek(dtcFile, pos) == pos)
            storedDtcs[i] = EepromReadHalfword(dtcFile);
    }
}

// clears all memory of eeprom
uint8_t eraseAll(void)
{
    size_t sz = fileStreamGetSize(dtcFile);
    size_t i, written = 0;
    if (fileStreamSeek(dtcFile, 0) != 0)
        return 0;

    for (i = 0;  i + 3 < sz; i += 4) {
        written += EepromWriteWord(dtcFile, 0);
    }

    storedDtcLength = 0;

    return written;
}
// ---------------------------------------------------------------------------------------
// begin threads
static THD_WORKING_AREA(waSetDtc, 350);
static THD_FUNCTION(setDtc, arg)
{
    (void)arg;
    chRegSetThreadName("setDtc");

    startupTime = chVTGetSystemTime();


    while (TRUE) {
        can_DTCs_e dtc;
        if (chMBFetch(&dtc_MB, (msg_t*)&dtc, TIME_INFINITE) != MSG_OK)
            continue;

        msg_t i;
        uint8_t occurences = 0,
                storedIdx = 0;
        uint16_t dtcTime = 0;

        for (i = 0; i < storedDtcLength; ++i) {
            if (storedDtcs[i] == dtc) {
                // already stored, increment occurence counter
                msg_t pos = 1 + (i * DTC_SIZE) + sizeof(uint16_t);
                if (fileStreamSeek(dtcFile, pos) == pos) {
                    occurences = EepromReadByte(dtcFile);
                    if (occurences < 0xFF)
                        ++occurences;
                    dtcTime = EepromReadHalfword(dtcFile);
                    fileStreamSeek(dtcFile, pos);
                    EepromWriteByte(dtcFile, occurences);
                    storedIdx = i;
                    break;
                }
            }
        }

        if (occurences == 0) {
            // dtc wasn't stored, its a new DTC
            msg_t pos = 1 + (storedDtcLength * DTC_SIZE);
            if (storedDtcLength > MAX_DTCS) {
                // restart DTC from bottom
                pos = 1;
                storedIdx = 0;
            } else {
                storedIdx = storedDtcLength;
            }
            if (fileStreamSeek(dtcFile, pos) == pos) {
                static uint8_t data8[8]; // static to save stackspace
                storedDtcs[storedDtcLength] = (uint16_t)dtc;

                // save dtc code
                EepromWriteHalfword(dtcFile, (uint16_t)dtc);

                // occurences
                EepromWriteByte(dtcFile, ++occurences);

                // time since start
                systime_t t = chVTTimeElapsedSinceX(startupTime);
                dtcTime = ST2S(t);
                EepromWriteHalfword(dtcFile, dtcTime);

                // freeze frame
                // pid1 8bytes
                can_buildPid1Data(data8);
                EepromWriteWord(dtcFile, (data8[3] << 24 | data8[2] << 16 |
                                          data8[1] << 8 | data8[0]));
                EepromWriteWord(dtcFile, (data8[7] << 24 | data8[6] << 16 |
                                          data8[5] << 8 | data8[4]));

                // pid2 8bytes
                can_buildPid2Data(data8);
                EepromWriteWord(dtcFile, (data8[3] << 24 | data8[2] << 16 |
                                          data8[1] << 8 | data8[0]));
                EepromWriteWord(dtcFile, (data8[7] << 24 | data8[6] << 16 |
                                          data8[5] << 8 | data8[4]));

                // pid3 5 bytes
                can_buildPid3Data(data8);
                EepromWriteWord(dtcFile, (data8[3] << 24 | data8[2] << 16 |
                                          data8[1] << 8 | data8[0]));
                EepromWriteByte(dtcFile, data8[4]);

                // save the new dtc length
                if (storedDtcLength <= MAX_DTCS) {
                    if (fileStreamSeek(dtcFile, 0) == 0) {
                        storedDtcLength += 1;
                        EepromWriteByte(dtcFile, storedDtcLength);
                    }
                }

            }
        }

        // broadcast this dtc as a new error
        static CANTxFrame txf;
        can_initTxFrame(&txf, CAN_MSG_TYPE_EXCEPTION, C_parkbrakeExcNewDTC);
        txf.DLC = 6;
        txf.data8[0] = storedIdx;
        txf.data8[1] = dtc & 0x00FF;
        txf.data8[2] = (dtc & 0xFF00) >> 8;
        txf.data8[3] = occurences;
        txf.data8[4] = (dtcTime & 0x00FF);
        txf.data8[5] = (dtcTime & 0xFF00) >> 8;
        canTransmitTimeout(&CAND1, CAN_ANY_MAILBOX, &txf, DIAG_CAN_TIMEOUT);
    }
}


static THD_WORKING_AREA(waDiagCanThd, 512);
static THD_FUNCTION(diagCanThd, arg)
{
    (void)arg;
    chRegSetThreadName("diagCanThd");

    while (TRUE) {
        static msg_t msg;
        if (chMBFetch(&diag_CanMB, &msg, TIME_INFINITE) != MSG_OK)
            continue;

        static uint8_t idx;
        static msg_t pos;
        static can_msgIdsDiag_e action;

        action = (msg & 0xFFFF0000) >> 16; // 2 highest bytes is action
        idx  = msg & 0xFF; // lowest bytes is idx

        switch (action) {
        case C_parkbrakeDiagClearDTCs: {
            CANTxFrame txf;
            can_initTxFrame(&txf, CAN_MSG_TYPE_DIAG, C_parkbrakeDiagClearDTCs);
            txf.DLC = 1;
            txf.data8[0] = storedDtcLength;
            eraseAll();
            canTransmitTimeout(&CAND1, CAN_ANY_MAILBOX, &txf, DIAG_CAN_TIMEOUT);
        }   break;
        case C_parkbrakeDiagDTC:
            pos = 1 + (idx * DTC_SIZE);
            if (fileStreamSeek(dtcFile, pos) == pos) {
                CANTxFrame txf;
                can_initTxFrame(&txf, CAN_MSG_TYPE_DIAG, C_parkbrakeDiagDTC);
                txf.data8[0] = idx;

                // read DTC code
                uint16_t hwd = EepromReadHalfword(dtcFile);
                txf.data8[1] = (hwd & 0x00FF);
                txf.data8[2] = (hwd & 0xFF00) >> 8;

                // read occurrences
                txf.data8[3] = EepromReadByte(dtcFile);

                // read time when set
                hwd = EepromReadHalfword(dtcFile);
                txf.data8[4] = (hwd & 0x00FF);
                txf.data8[5] = (hwd & 0xFF00) >> 8;

                // 6 bytes to send
                txf.DLC = 6;
                canTransmitTimeout(&CAND1, CAN_ANY_MAILBOX, &txf, DIAG_CAN_TIMEOUT);
            }

            break;
        case C_parkbrakeDiagDTCFreezeFrame: {
            pos = (idx * DTC_SIZE) + 6; // dtcLen:1by + code:2by + occu:1by + time:2by;
            if (fileStreamSeek(dtcFile, pos) == pos) {
                static CANTxFrame txf;
                can_initTxFrame(&txf, CAN_MSG_TYPE_DIAG, C_parkbrakeDiagDTCFreezeFrame);

                txf.data8[0] = idx;
                // build frame 0
                txf.data8[1] = 0; // frame 0 ,states of wheels
                uint32_t wd = EepromReadWord(dtcFile);
                txf.data8[2] = wd & 0x000000FF;         // LF state
                txf.data8[3] = (wd & 0x0000FF00) >> 8;  // RF state
                txf.data8[4] = (wd & 0x00FF0000) >> 16; // RL state
                txf.data8[5] = (wd & 0xFF000000) >> 24; // RR state
                txf.DLC = 6;
                canTransmitTimeout(&CAND1, CAN_ANY_MAILBOX, &txf, DIAG_CAN_TIMEOUT);

                // build frame 1
                txf.data8[1] = 1; // frame 1, revs of wheels
                wd = EepromReadWord(dtcFile);
                txf.data8[2] = wd & 0x000000FF;         // LF r/s
                txf.data8[3] = (wd & 0x0000FF00) >> 8;  // RF r/s
                txf.data8[4] = (wd & 0x00FF0000) >> 16; // LR r/s
                txf.data8[5] = (wd & 0xFF000000) >> 24; // RR r/s
                txf.DLC = 6;
                canTransmitTimeout(&CAND1, CAN_ANY_MAILBOX, &txf, DIAG_CAN_TIMEOUT);
                // build frame 2
                txf.data8[1] = 2; // frame 2, amps momentarily
                wd = EepromReadWord(dtcFile);
                txf.data8[2] = wd & 0x000000FF;         // LF A now
                txf.data8[3] = (wd & 0x0000FF00) >> 8;  // RF A now
                txf.data8[4] = (wd & 0x00FF0000) >> 16; // LR A now
                txf.data8[5] = (wd & 0xFF000000) >> 24; // RR A now
                txf.DLC = 6;
                canTransmitTimeout(&CAND1, CAN_ANY_MAILBOX, &txf, DIAG_CAN_TIMEOUT);
                // build frame 3
                txf.data8[1] = 3; // frame 3, amps maximum
                wd = EepromReadWord(dtcFile);
                txf.data8[2] = wd & 0x000000FF;         // LF A max
                txf.data8[3] = (wd & 0x0000FF00) >> 8;  // RF A max
                txf.data8[4] = (wd & 0x00FF0000) >> 16; // LR A max
                txf.data8[5] = (wd & 0xFF000000) >> 24; // RR A max
                txf.DLC = 6;
                canTransmitTimeout(&CAND1, CAN_ANY_MAILBOX, &txf, DIAG_CAN_TIMEOUT);
                // build frame 4
                txf.data8[1] = 4; // frame 4, volts, chip temp and digital inputs
                wd = EepromReadWord(dtcFile);
                txf.data8[2] = (wd & 0x000000FF);        // bat mV [0:15]
                txf.data8[3] = (wd & 0x0000FF00) >> 8;
                txf.data8[4] = (wd & 0x00FF0000) >> 16; // ign mV [0:15]
                txf.data8[5] = (wd & 0xFF000000) >> 24;
                // chip temp and inputs
                wd = EepromReadHalfword(dtcFile);
                txf.data8[6] = (wd & 0x00FF);      // chiptemp
                txf.data8[7] = (wd & 0xFF00) >> 8; // digital inputs
                txf.DLC = 8;
                canTransmitTimeout(&CAND1, CAN_ANY_MAILBOX, &txf, DIAG_CAN_TIMEOUT);
            }

        }   break;
        case C_parkbrakeDiagDTCLength: {
            CANTxFrame txf;
            can_initTxFrame(&txf, CAN_MSG_TYPE_DIAG, C_parkbrakeDiagDTCLength);
            txf.DLC = 1;
            txf.data8[0] = storedDtcLength;
            canTransmitTimeout(&CAND1, CAN_ANY_MAILBOX, &txf, DIAG_CAN_TIMEOUT);
        }   break;
        case C_parkbrakeDiagActuatorTest: {
            CANTxFrame txf;
            can_userError_e e = C_userErrorNone;
            if (!SEN_LIGHTS_ON_SIG)
                e = C_userErrorBrakeOff;
            else if (!SEN_IGN_ON_SIG)
                e = C_userErrorIgnOff;
            else if (!SEN_BUTTON_INV_SIG)
                e = C_userErrorBtnInvOff; // must hold button in release state

            // user errors
            if (e != C_userErrorNone) {
                can_initTxFrame(&txf, CAN_MSG_TYPE_EXCEPTION, C_parkbrakeExcUserError);
                txf.DLC = 2;
                txf.data8[0] =  e & 0x00FF;
                txf.data8[1] = (e & 0xFF00) >> 8;
                canTransmitTimeout(&CAND1, CAN_ANY_MAILBOX, &txf, DIAG_CAN_TIMEOUT);
                break;
            }

            // every condition to activate is meet, we should be able to activate test
            if (idx & 0x80) {
                ctrl_setStateWheel(Tightening, RightRear);
            } else if (idx & 0x40) {
                ctrl_setStateWheel(Tightening, LeftRear);
            } else if (idx & 0x20) {
                ctrl_setStateWheel(Tightening, RightFront);
            } else if (idx & 0x10) {
                ctrl_setStateWheel(Tightening, LeftFront);
            } else if (idx & 0x08) {
                ctrl_setStateWheel(Releasing, RightRear);
            } else if (idx & 0x04) {
                ctrl_setStateWheel(Releasing, LeftRear);
            } else if (idx & 0x02) {
                ctrl_setStateWheel(Releasing, RightFront);
            } else if (idx & 0x01) {
                ctrl_setStateWheel(Releasing, LeftFront);
            }

        }   break;
        default: break;
        }

        // check sensorcircuits each diagRequest
        sen_diagWheelSensors();
    }
}

// ---------------------------------------------------------------------------------------
// begin public API functions

void diag_init(void)
{
    initDtcListFromEeprom();

    chMBObjectInit(&dtc_MB, dtcMBqueue, MAILBOX_SIZE);
    chMBObjectInit(&diag_CanMB, diagCanMBqueue, MAILBOX_SIZE);

    setDtcp = chThdCreateStatic(&waSetDtc, sizeof(waSetDtc), LOWPRIO, setDtc, NULL);

    diagCanThdp = chThdCreateStatic(&waDiagCanThd, sizeof(waDiagCanThd), LOWPRIO, diagCanThd, NULL);


    // check sensorcircuits during startup
    sen_diagWheelSensors();
}

uint8_t dtc_length(void)
{
    return storedDtcLength;
}


