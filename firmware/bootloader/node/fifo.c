/*
 * fifo.c
 *
 *  Created on: 11 aug 2018
 *      Author: jof
 */


/**
 * @brief implements a queue for can messages
 */

#include "fifo.h"
#include <string.h> // for memcpy


void fifo_init(fifo_t *queue, canframe_t *buf, uint8_t size)
{
  queue->size = size;
  queue->head = 0;
  queue->tail = 0;
  queue->buf = buf;
}

bool fifo_push(fifo_t *queue, canframe_t *frm)
{
  int sp = fifo_spaceleft(queue);
  if (sp < 2) // need 2 as 1 + 1 would reset (start over algorithm assumes empty)
  {
    // no more room
    return false;
  }
  canframe_t *f = &queue->buf[queue->head];
  memcpy(f, frm, sizeof(canframe_t));
  ++queue->head;

  if (queue->head == queue->size)
    queue->head = 0; // wrap around
  return true;
}

bool fifo_pop(fifo_t *queue, canframe_t *frm)
{
  if (fifo_peek(queue, frm)) {
    ++queue->tail;
    if (queue->tail == queue->size)
      queue->tail = 0;
    return true;
  }
  return false; // nothing stored, empty
}

bool fifo_peek(fifo_t *queue, canframe_t *frm)
{
  if (queue->tail != queue->head) {
    canframe_t *f = &queue->buf[queue->tail];
    memcpy(frm, f, sizeof(canframe_t));
    return true;
  }
  return false;
}

bool fifo_empty(fifo_t *queue)
{
  return queue->tail == queue->head;
}

int fifo_spaceleft(fifo_t *queue)
{
  if (queue->head < queue->tail)
    return queue->tail - queue->head;
  return queue->size - queue->head + queue->tail;
}
