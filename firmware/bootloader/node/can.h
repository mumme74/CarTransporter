/*
 * can.h
 *
 *  Created on: 27 jul 2018
 *      Author: jof
 */

#ifndef CAN_H_
#define CAN_H_

#include <libopencm3/stm32/can.h>

#define BUFFER_SIZE 128

typedef struct {
  uint8_t DLC;
  union {
    uint16_t IDE;
    uint32_t EID;
  };
  bool ext;
  bool rtr;
  union {
    uint8_t data8[8];
    uint16_t data16[4];
    uint32_t data32[2];
    uint64_t data64;
  };
} canframe_t;


bool canGet(canframe_t *msg);
void canInit(uint32_t filterId);
void canInitFrame(canframe_t *msg, uint32_t id);
int8_t canPost(canframe_t *msg);

void canShutdown(void);

#endif /* CAN_H_ */
