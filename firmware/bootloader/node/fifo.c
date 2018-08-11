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


void fifo_init(fifo_t *queue, canframe_t *buf[], uint8_t size)
{
  queue->buf = buf;
  queue->size = size;
  queue->head = 0;
  queue->tail = 0;
}

bool fifo_push(fifo_t *queue, canframe_t *frm)
{
  if ((queue->head +1 == queue->tail) ||
      ((queue->head +1 == queue->size) && (queue->tail == 0)))
  {
    // no more room
    return false;
  }

  memcpy(queue->buf[queue->head], frm, sizeof(canframe_t));
  ++queue->head;
  if (queue->head == queue->size)
    queue->head = 0; // wrap around
  return true;
}

bool fifo_pop(fifo_t *queue, canframe_t *frm)
{
  if (queue->tail != queue->head) {
    memcpy(frm, queue->buf[queue->tail++], sizeof(canframe_t));
    if (queue->tail == queue->size)
      queue->tail = 0;
    return true;
  }
  return false; // nothing stored, empty
}

int fifo_spaceleft(fifo_t *queue)
{
  if (queue->head < queue->tail)
    return queue->size - (queue->tail - queue->head);
  return queue->size - (queue->head - queue->tail);
}
