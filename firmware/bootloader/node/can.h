/*
 * can.h
 *
 *  Created on: 27 jul 2018
 *      Author: jof
 */

#ifndef CAN_H_
#define CAN_H_

#include <stdbool.h>
#include "can_types.h"

#define BUFFER_SIZE 128

#ifdef __cplusplus
extern "C" {
#endif



void canWaitRecv(can_frame_t *msg);
void canWaitSend(can_frame_t *msg);
bool canGet(can_frame_t *msg);
void canInit(void);
void canInitFrame(can_frame_t *msg, uint32_t id);
int8_t canPost(can_frame_t *msg);
void canShutdown(void);
void canDisableIRQ(void);
void canEnableIRQ(void);


#ifdef __cplusplus
}
#endif

#endif /* CAN_H_ */
