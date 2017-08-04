/*
 * can.h
 *
 *  Created on: 30 jul 2017
 *      Author: fredrikjohansson
 */

#ifndef CAN_H_
#define CAN_H_
#include "ch.h"
#include "hal.h"

/**
 * This module handles I/O on CAN network
 */

extern mailbox_t dtcMB;


void can_init(void);

void can_buildPid1Data(uint8_t data8[]);
void can_buildPid2Data(uint8_t data8[]);
void can_buildPid3Data(uint8_t data8[]);

void can_initTxFrame(CANTxFrame *txf, uint16_t msgType, uint16_t msgId);

#endif /* CAN_H_ */
