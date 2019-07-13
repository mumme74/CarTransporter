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

// private variables to this file
static canframe_t rxbuf[BUFFER_SIZE];
static canframe_t txbuf[BUFFER_SIZE];

// symbols exported in other translation units
// architecture specific
extern int8_t _canPost(canframe_t *msg);
extern void _canInit(void);
extern bool _canGet(void);


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

void canWaitRecv(canframe_t *msg) {
  uint32_t resetAt = systemMillis() + WAIT_UNRESPONSE_CAN_BEFORE_RESET;
  while(!canGet(msg)) {
      if (systemMillis() > resetAt)
	systemReset();
#ifdef DEBUG_PRINT
      if (systemMillis() % 100 && usb_serial_available())
	print_flush();
#endif
  }
}

void canWaitSend(canframe_t *msg) {
  uint32_t resetAt = systemMillis() + WAIT_UNRESPONSE_CAN_BEFORE_RESET;
  while(!canPost(msg) != 1) {
      if (systemMillis() > resetAt)
	systemReset();
#ifdef DEBUG_PRINT
      if (systemMillis() % 100 && usb_serial_available())
	print_flush();
#endif
  }
}

/**
 * @brief get the first received msg from fifo
 * returns: false if empty
 *          fills msg with last received frame
 *              return true if not empty
 */
bool canGet(canframe_t *msg)
{
  if (fifo_pop(&can_rxqueue, msg))
    return true;
  return _canGet();
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

  if (!_canPost(msg)) {
      // buffer full?
      if (!fifo_push(&can_txqueue, msg))
	return -1;
      return 0;
  }

  return 1;
}

void canInitFrame(canframe_t *msg, uint32_t id)
{
  msg->EID = id;
  msg->data64 = 0;
  msg->DLC = 0;
  msg->ext = id > 0x7FF ? true : false;
  msg->rtr = false;
}
