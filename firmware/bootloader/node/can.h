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


bool canGet(canframe_t *msg);
void canInit(void);
void canInitFrame(canframe_t *msg, uint32_t id);
int8_t canPost(canframe_t *msg);
void canShutdown(void);

#ifdef __cplusplus
}
#endif

#endif /* CAN_H_ */
