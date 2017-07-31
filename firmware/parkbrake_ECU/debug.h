/*
 * debug.h
 *
 *  Created on: 31 jul 2017
 *      Author: fredrikjohansson
 */

#ifndef DEBUG_H_
#define DEBUG_H_


#ifdef DEBUG_MODE
#include <string.h>
#include "chprintf.h"
#define DEBUG_OUT(msg) sdAsynchronousWrite(&SD1, (uint8_t*)msg, strlen(msg))
#define DEBUG_OUT_PRINTF(msg, str) {\
    char buf[50];                   \
    chsnprintf(buf, 49, msg, str);  \
    DEBUG_OUT(msg);                  \
}
#else
#define DEBUG_OUT(msg)
#define DEBUG_OUT_PRINTF(msg, str)
#endif


#endif /* DEBUG_H_ */
