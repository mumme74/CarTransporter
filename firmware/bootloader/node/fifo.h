/*
 * fifo.h
 *
 *  Created on: 11 aug 2018
 *      Author: jof
 */

#ifndef FIFO_H_
#define FIFO_H_

#include <stdint.h>
#include <stddef.h>
#include "can.h"

typedef struct {
  volatile uint8_t tail,
                    head;
  uint8_t size;
  canframe_t **buf;
} fifo_t;

void fifo_init(fifo_t *queue, canframe_t *buf[], uint8_t size);
bool fifo_push(fifo_t *queue, canframe_t *frm);
bool fifo_pop(fifo_t *queue, canframe_t *frm);
//int fifo_spaceleft(fifo_t *queue);


#endif /* FIFO_H_ */
