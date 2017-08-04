/*
 * dtc.h
 *
 *  Created on: 4 aug 2017
 *      Author: fredrikjohansson
 */

#ifndef DIAG_H_
#define DIAG_H_

#include "ch.h"


// mailbox for setting a dtc
extern mailbox_t dtc_MB,
                 diag_CanMB;

void diag_init(void);

uint8_t dtc_length(void);


#endif /* DIAG_H_ */
