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

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  volatile uint8_t tail,
                    head;
  volatile uint8_t size;
  canframe_t *buf;
} fifo_t;

void fifo_init(fifo_t *queue, canframe_t *buf, uint8_t size);
bool fifo_push(fifo_t *queue, canframe_t *frm);
bool fifo_pop(fifo_t *queue, canframe_t *frm);
bool fifo_peek(fifo_t *queue, canframe_t *frm);
bool fifo_empty(fifo_t *queue);
int fifo_spaceleft(fifo_t *queue);

#ifdef __cplusplus
}
#endif


#endif /* FIFO_H_ */
