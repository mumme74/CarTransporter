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

/*
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
    Serial.print("gotFrame\r\n");
    // check if we are full
    if (fifo_spaceleft(&can_rxqueue) < 2)// need 2 as 1 + 1 would reset (start over algorithm assumes empty)
       return;

    for (int i = 0; i < frm.len; ++i) {
      Serial.print(" b:");Serial.print(frm.buf[i], 16);
    }
    Serial.print("\r\n");

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
*/


// -----------------------------------------------------------
// public functions and variables

void canShutdown(void)
{
 // listener.detachGeneralHandler();
 //Can0.detachObj(&listener);
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
  //Can0.attachObj(&listener);
  //listener.attachGeneralHandler();
}

int8_t _canPost(canframe_t *msg)
{
  if (fifo_peek(&can_txqueue, msg)) {
    CAN_message_t frm;
    frm.id = msg->EID;
    frm.ext = msg->ext;
    frm.rtr = msg->rtr;
    frm.len = msg->DLC;
    for(uint8_t i = 0; i < msg->DLC; ++i)
      frm.buf[i] = msg->data8[i];
    if (Can0.write(frm)) {
      fifo_pop(&can_txqueue, msg);
      Sprt("post");Sprt(frm.len);Sprt("\r\n");
      return 1;
    }
  }
  return 0;
}


// should be called from teensy_main
void _canLoop(void)
{
  // debug send
  /*static uint16_t lGuard = 0;
  if (lGuard++ == 0) {
    Serial.print("_canLoop");Serial.print(lGuard);Serial.print("\r\n");
    canframe_t m;
    memset(&m, 0, sizeof(m));
    m.IDE = canId;
    m.DLC = 0;
    m.data8[m.DLC++] = 1;
    m.data8[m.DLC++] = 2;
    m.data8[m.DLC++] = 3;
    m.data8[m.DLC++] = 4;
    m.data8[m.DLC++] = 5;
    m.data8[m.DLC++] = 6;
    m.data8[m.DLC++] = 7;
    m.data8[m.DLC++] = 8;
    fifo_push(&can_txqueue, &m);
    Serial.print("Send can\r\n");
  }*/

  if (!fifo_empty(&can_txqueue)) {
static uint8_t i =0;
if ((++i % 8) == 0)
  endl();
  print_uint(fifo_empty(&can_txqueue));
    canframe_t msg;
    _canPost(&msg);
  }

  CAN_message_t frm;

  if (Can0.read(frm)) {
    // check if we are full
    if (fifo_spaceleft(&can_rxqueue) < 2)// need 2 as 1 + 1 would reset (start over algorithm assumes empty)
       return;

    canframe_t msg;
    msg.DLC = frm.len;
    msg.EID = frm.id;
    msg.ext = frm.ext != 0;
    msg.rtr = frm.rtr != 0;
    for (uint8_t i = 0; i< msg.DLC; ++i)
      msg.data8[i] = frm.buf[i];

    fifo_push(&can_rxqueue, &msg);
  }


  digitalWrite(6, !digitalRead(6));
  //delay(20);
}

} // extern "C"

