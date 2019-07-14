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

bool commandsStart(canframe_t *msg);
bool commandIsResetFrame(canframe_t *frm);

#ifdef __cplusplus
}
#endif

#endif /* COMMANDS_H_ */
