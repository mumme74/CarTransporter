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
#include <ch.h>
#include "canserial.h"
#include "can_protocol.h"
#include "can.h"
#include <string.h>


#define STREAM_SIZE 0x7F
#define CANSERIAL_MB_SIZE 64
#define CANTRANSMIT_TIMEOUT TIME_MS2I(1)

// ---------------------------------------------------------------------------------
// begin public exported variables


// ---------------------------------------------------------------------------------
// begin private function prototypes for this module
void objectInit(CanSerialStream *csp, uint8_t *buffer,
                          size_t size, size_t eos);


// ----------------------------------------------------------------------------------
// begin private functions and variables to this module

event_source_t evtHasContent;
thread_t *canSerialSendp = NULL;


/* Methods implementations.*/
static size_t writes(void *ip, const uint8_t *bp, size_t n) {
  CanSerialStream *csp = ip;
  size_t nw = n, nf = 0;
  chSysLock();

  if (csp->size - csp->eos < nw)
    nw = csp->size - csp->eos;

  memcpy(csp->buffer + csp->eos, bp, nw);
  csp->eos += nw;

  // should we flip over?
  if (n > nw) {
    nf = n - nw; // flipped over
    if (nf > csp->offset)
      nf = csp->offset;
    memcpy(csp->buffer, bp + nw, nf);
    csp->eos = nf;
  }
  chSysUnlock();

  chEvtBroadcast(&evtHasContent);
  return nw + nf;
}

static size_t reads(void *ip, uint8_t *bp, size_t n) {
  CanSerialStream *csp = ip;
  chSysLock();
  size_t nr = (n <= csp->size ? n : csp->size),
         nf = 0;

  if (csp->eos >= csp->offset) {
    // we have not flipped around
    if (csp->eos - csp->offset < nr)
      nr = csp->eos - csp->offset;
    memcpy(bp, csp->buffer + csp->offset, nr);
    csp->offset += nr;
  } else {
    //  we have flipped around
    // first copy from eos to end
    if (csp->size - csp->offset < nr)
      nr = csp->size - csp->offset;
    memcpy(bp, csp->buffer + csp->offset, nr);
    csp->offset += nr;

    // should we copy from 0 to eos?
    if (nr < n) {
      nf = n - nr;
      if (nf > csp->eos)
        nf = csp->eos;
      memcpy(bp + nr, csp->buffer, nf);
      csp->offset = nf;
    }

  }
  chSysUnlock();

  return nr + nf;
}

static msg_t put(void *ip, uint8_t b) {
  CanSerialStream *csp = ip;

  if (csp->size - csp->eos <= 0) {
    // flip around
    if (csp->eos - csp->offset == csp->size)
      return MSG_RESET; // full, not yet cleared
    csp->eos = 0;
  }

  *(csp->buffer + csp->eos) = b;
  csp->eos += 1;
  return MSG_OK;
}

static msg_t get(void *ip) {
  uint8_t b;
  CanSerialStream *csp = ip;

  if (csp->size - csp->offset <= 0) {
    // flip around
    if (csp->eos == csp->offset)
      return MSG_RESET;
    csp->offset = 0;
  }

  b = *(csp->buffer + csp->offset);
  csp->offset += 1;
  return b;
}

static void flush(void *ip) {
  CanSerialStream *csp = ip;

  if (csp->offset < csp->eos || csp->offset > csp->eos)
    chEvtBroadcast(&evtHasContent);
}


static const struct CanSerialStreamVMT vmt = { 0, writes, reads, put, get, flush };

uint8_t CANSerialBuffer[STREAM_SIZE+1];

CanSerialStream CANSerial0 = {
    &vmt,
    (uint8_t*)&CANSerialBuffer,
    STREAM_SIZE,
    0,
    0
};



// ---------------------------------------------------------------------------------
// begin threads

static THD_WORKING_AREA(waCanSerialSend, 128);
static THD_FUNCTION(canSerialSend, arg)
{

  (void)arg;

  chRegSetThreadName("canSerialSend");
  static event_listener_t evtListener;
  chEvtRegister(&evtHasContent, &evtListener, EVENT_MASK(0));

  while(!chThdShouldTerminateX()) {
    // must be polled to allow clean shutdown
    if (chEvtWaitAnyTimeout(ALL_EVENTS, TIME_MS2I(1000)) != MSG_OK)
      continue;

    static uint8_t readBuf[STREAM_SIZE +1];
    static uint8_t nFrames;
    static uint8_t strlen;
    nFrames = 0;
    strlen = CANSerial0.vmt->read(&CANSerial0, (void*)&readBuf, STREAM_SIZE);

    static CANTxFrame txf;
    can_initTxFrame(&txf, CAN_MSG_TYPE_DIAG, C_parkbrakeDiagSerial);

    txf.DLC = 1;
    txf.data8[0] = 0x80 | strlen; // 1st frame

    for (int i = 0; i < strlen; ++i) {
      txf.data8[txf.DLC++] = readBuf[i];
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


/*
 * CanSerial stream object initialization.
 *
 * csp       pointer to the CanSerialStream object to be initialized
 * buffer    pointer to the memory buffer for the memory stream
 * size      total size of the memory stream buffer
 * eos       initial End Of Stream offset
 */
void objectInit(CanSerialStream *csp, uint8_t *buffer,
                          size_t size, size_t eos) {

  csp->vmt    = &vmt;
  csp->buffer = buffer;
  csp->size   = size;
  csp->eos    = eos;
  csp->offset = 0;
}


// ---------------------------------------------------------------------------------
// begin public exported api functions

void canserial_init(void)
{
  chEvtObjectInit(&evtHasContent);

  //objectInit(&CANSerial0, (uint8_t*)&CANSerialBuffer, STREAM_SIZE, 0);

  canSerialSendp = chThdCreateStatic(&waCanSerialSend, sizeof(waCanSerialSend),
                                     NORMALPRIO-10, canSerialSend, NULL);

}

void canserial_thdsTerminate(void)
{
  chThdTerminate(canSerialSendp);
}

void canserial_doShutdown(void)
{
  chThdWait(canSerialSendp);
}

size_t canserialAsynchronousWrite(uint8_t *msg, uint8_t len)
{
  return CANSerial0.vmt->write(&CANSerial0, msg, len);
}
