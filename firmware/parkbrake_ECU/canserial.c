/*
 * canserial.cpp
 *
 *  Created on: 24 jul 2018
 *      Author: jof
 */

/**
 * @brief This file ia an attempt to make a serial output route through CAN
 *  Used for debugging, have a remote app in raspberrypi to read debug messages
 */

#include <hal.h>
#include "canserial.h"
#include "can_protocol.h"
#include "can.h"


#define CANSERIAL_MB_SIZE 64
#define CANTRANSMIT_TIMEOUT MS2ST(1)

// ---------------------------------------------------------------------------------
// begin public exported variables
mailbox_t canserialMBSend;
char canserialMBSendBuf[CANSERIAL_MB_SIZE];


// ---------------------------------------------------------------------------------
// begin private function prototypes for this module



// ----------------------------------------------------------------------------------
// begin private functions and variables to this module



// ---------------------------------------------------------------------------------
// begin threads

static THD_WORKING_AREA(waCanSerialSend, 128);
static THD_FUNCTION(canSerialSend, arg)
{

  (void)arg;

  chRegSetThreadName("canSerialSend");

  while(TRUE) {
    static msg_t msg;
    if (chMBFetch(&canserialMBSend, &msg, TIME_INFINITE) != MSG_OK)
        continue;

    if (msg == 0)
      continue;

    static char *cp = (char*)&msg;
    static uint8_t nFrames;
    static uint8_t strlen;
    nFrames = 0;
    strlen = 0;

    // get strlen
    for (cp = (char*)msg; *cp != '\0' && strlen < 0x7F; ++cp)
      ++strlen;

    static CANTxFrame txf;
    can_initTxFrame(&txf, CAN_MSG_TYPE_DIAG, C_parkbrakeDiagSerial);

    txf.DLC = 1;
    txf.data8[0] = 0x80 | strlen; // 1st frame

    cp = (char*)msg;

    for (int i = 0; i < strlen; ++i) {
      txf.data8[txf.DLC++] = cp[i];
      if (txf.DLC > 7) {
        canTransmitTimeout(&CAND1, CAN_ANY_MAILBOX, &txf, CANTRANSMIT_TIMEOUT);
        txf.DLC = 1;
        txf.data8[0] = ++nFrames;
      }
    }

    // we might get here with a unposted frame
    if (txf.DLC > 1)
      canTransmitTimeout(&CAND1, CAN_ANY_MAILBOX, &txf, CANTRANSMIT_TIMEOUT);
  }
}




// ---------------------------------------------------------------------------------
// begin public exported api functions

void canserial_init(void)
{
  // init mailbox for printchars
  chMBObjectInit(&canserialMBSend, (msg_t*)canserialMBSendBuf,
                  CANSERIAL_MB_SIZE);

  chThdCreateStatic(&waCanSerialSend, sizeof(waCanSerialSend),
                    NORMALPRIO-10, canSerialSend, NULL);

}
