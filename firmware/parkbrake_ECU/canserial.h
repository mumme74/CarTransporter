/*
 * canserial.h
 *
 *  Created on: 24 jul 2018
 *      Author: jof
 */

#ifndef CANSERIAL_H_
#define CANSERIAL_H_
#include <ch.h>


// post pointers to \0 terminated strings
extern mailbox_t canserialMBSend;

void canserial_init(void);

#endif /* CANSERIAL_H_ */
