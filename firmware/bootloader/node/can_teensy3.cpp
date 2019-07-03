/*
 * can_teensy3.cpp
 *
 *  Created on: 2 jul 2019
 *      Author: jof
 */


#include "system.h"
#include "can.h"
#include "fifo.h"
#include <stddef.h>
#include <stdint.h>
#include <cstring>
#include <Arduino.h>
#include <FlexCAN.h>

extern "C" {
extern fifo_t can_rxqueue, can_txqueue; // is visible to global from can.c


// prototypes
int8_t _canPost(canframe_t *msg);
void _canInit(void);
void _canLoop(void);

} // extern "C"


// -----------------------------------------------------------
// private to this file
CAN_filter_t canFilter1 = { 0, 0, canId },
             canFilter2 = { 0, 0, ((uint32_t)canId | C_displayNode) };

class Listener : public CANListener
{
public:
  Listener() :
    CANListener()
  {
  }
  ~Listener() {}
  void gotFrame(CAN_message_t &frm, int mailbox)
  {
    // check if we are full
    if (fifo_spaceleft(&can_rxqueue) < 2)// need 2 as 1 + 1 would reset (start over algorithm assumes empty)
       return;

    canframe_t msg;
    msg.DLC = frm.len;
    msg.EID = frm.id;
    msg.ext = frm.ext != 0;
    msg.rtr = frm.rtr != 0;
    memcpy(msg.data8, frm.buf, msg.DLC);

    fifo_push(&can_rxqueue, &msg);
  }
};

Listener listener;


// -----------------------------------------------------------
// public functions and variables

void canShutdown(void)
{
  listener.detachGeneralHandler();
  Can0.detachObj(&listener);
  Can0.end();
}



// -----------------------------------------------------------
// architecture specific from here on, treat as private
extern "C" {
void _canInit(void)
{
  const uint8_t CAN_TX_BOX_CNT = 2;
  Can0.begin(250000); // 250kHz
  Can0.setNumTXBoxes(CAN_TX_BOX_CNT);
  for (int i = 0; i < NUM_MAILBOXES - CAN_TX_BOX_CNT; ++i) {
    Can0.setFilter(canFilter1, i);
    Can0.setFilter(canFilter2, i);
  }
  Can0.attachObj(&listener);
  listener.attachGeneralHandler();
}

int8_t _canPost(canframe_t *msg)
{
  if (fifo_peek(&can_txqueue, msg)) {
    CAN_message_t frm;
    frm.id = msg->EID;
    frm.ext = msg->ext;
    frm.rtr = msg->rtr;
    frm.len = msg->DLC;
    memcpy(frm.buf, msg->data8, msg->DLC);
    if (Can0.write(frm)) {
      fifo_pop(&can_txqueue, msg);
      return 1;
    }
  }
  return 0;
}


// should be called from teensy_main
void _canLoop(void)
{
  if (!fifo_empty(&can_txqueue)) {
    canframe_t msg;
    _canPost(&msg);
  }
}

} // extern "C"

