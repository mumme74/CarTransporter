/*
 * can.h
 *
 *  Created on: 27 jul 2018
 *      Author: jof
 */

#ifndef CAN_H_
#define CAN_H_

#include <stdbool.h>

#define BUFFER_SIZE 128

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  union {
    uint8_t  data8[8];
    uint16_t data16[4];
    uint32_t data32[2];
    uint64_t data64;
  };
  union {
    uint16_t IDE;
    uint32_t EID;
  };
  uint8_t DLC;
  bool ext;
  bool rtr;
} canframe_t;

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

typedef union {
  uint32_t v32;
  uint16_t v16[2];
  uint8_t  v8[4];
} u32to8_t;

void canWaitRecv(canframe_t *msg);
void canWaitSend(canframe_t *msg);
bool canGet(canframe_t *msg);
void canInit(void);
void canInitFrame(canframe_t *msg, uint32_t id);
int8_t canPost(canframe_t *msg);
void canShutdown(void);
void canDisableIRQ(void);
void canEnableIRQ(void);


#ifdef __cplusplus
}
#endif

#endif /* CAN_H_ */
