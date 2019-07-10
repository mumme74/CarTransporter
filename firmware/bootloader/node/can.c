/*
 * can.c
 *
 *  Created on: 2 jul 2019
 *      Author: jof
 */

#include "system.h"
#include "can.h"
#include "fifo.h"
#include <can_protocol.h>
#include <stddef.h>
#include <stdint.h>

#ifdef ARDUINO
#include <Arduino.h>
#endif


// private variables to this file
static canframe_t rxbuf[BUFFER_SIZE];
static canframe_t txbuf[BUFFER_SIZE];

// symbols exported in other translation units
// architecture specific
extern int8_t _canPost(canframe_t *msg);
extern void _canInit(void);


// global variables
fifo_t can_rxqueue, can_txqueue;


// -----------------------------------------------------
// exported functions
void canInit(void)
{
  // init queues
  fifo_init(&can_rxqueue, rxbuf, BUFFER_SIZE);
  fifo_init(&can_txqueue, txbuf, BUFFER_SIZE);

  _canInit();
}

/**
 * @brief get the first received msg from fifo
 * returns: false if empty
 *          fills msg with last received frame
 *              return true if not empty
 */
bool canGet(canframe_t *msg)
{
#ifdef ARDUINO
  yield();
  _canLoop();
  if (usb_serial_available())
    usb_serial_flush_output();
#endif
  return fifo_pop(&can_rxqueue, msg);
}

/**
 * @brief post a frame to can
 * returns: -1 if buffer full
 *          0 if queued
 *          1 if send immediately
 */
int8_t canPost(canframe_t *msg)
{
  // notify which node sends this msg
  msg->IDE &= (CAN_MSG_TYPE_MASK | CAN_MSG_ID_MASK | NODE_ID);

  // buffer full?
  if (!fifo_push(&can_txqueue, msg))
    return -1;

  return _canPost(msg);
}

void canInitFrame(canframe_t *msg, uint32_t id)
{
  msg->EID = id;
  msg->data64 = 0;
  msg->DLC = 0;
  msg->ext = id > 0x7FF ? true : false;
  msg->rtr = false;
}
