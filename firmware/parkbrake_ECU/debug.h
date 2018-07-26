/*
 * debug.h
 *
 *  Created on: 31 jul 2017
 *      Author: fredrikjohansson
 */

#ifndef DEBUG_H_
#define DEBUG_H_


#ifdef DEBUG_MODE
#ifdef CANSERIAL
#include "canserial.h"
#define DEBUG_STREAM CANSerial0
#define DEBUG_OUT_PRINTF(msg, ...)  \
    chprintf((BaseSequentialStream*)&DEBUG_STREAM, msg, #__VA_ARGS__); \
    DEBUG_STREAM.vmt->flush(&DEBUG_STREAM); \

#else // not CANSERIAL

#define DEBUG_STREAM SD1
#define DEBUG_OUT_PRINTF(msg, ...)  \
    chprintf((BaseSequentialStream*)&DEBUG_STREAM, msg, #__VA_ARGS__); \

#endif // CANSERIAL
#define DEBUG_OUT(msg) DEBUG_STREAM.vmt->write(&DEBUG_STREAM, (uint8_t*)msg, strlen(msg));

#include <string.h>
#include "chprintf.h"

#else
#define DEBUG_STREAM
#define DEBUG_OUT(msg)
#define DEBUG_OUT_PRINTF(msg, str)
#endif


#endif /* DEBUG_H_ */
