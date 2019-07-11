/*
 * commands.h
 *
 *  Created on: 27 jul 2018
 *      Author: jof
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "system.h"
#include "can.h"
#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef union {
  uint64_t vlu;
  uint8_t *ptr8;
  uint16_t *ptr16;
  uint32_t *ptr32;
  uint64_t *ptr64;
  struct {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    uint8_t b0, b1, b2, b3, b4, b5, b6, b7;
#else
    uint8_t b7, b6, b5, b4, b3, b2, b1, b0;
#endif
  };
} byte8_t;

typedef union {
  uint32_t vlu;
  uint8_t *ptr8;
  uint16_t *ptr16;
  uint32_t *ptr32;
  struct {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    uint8_t b0, b1, b2, b3;
#else
    uint8_t b3, b2, b1, b0;
#endif
  };
} byte4_t;

typedef union {
  uint16_t vlu;
  // no ptr type here as ptrs are 32bits
  struct {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    uint8_t b0, b1;
#else
    uint8_t b1, b0;
#endif
  };
} byte2_t;

bool commandsStart(canframe_t *msg);

#ifdef __cplusplus
}
#endif

#endif /* COMMANDS_H_ */
