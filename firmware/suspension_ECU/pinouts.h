/*
 * pinouts.h
 *
 *  Created on: 1 apr 2015
 *      Author: jof
 */

#ifndef PINOUTS_H_
#define PINOUTS_H_

#include <stdint.h>
#include <Arduino.h>

// ITS5215L outputs
static const uint8_t
    leftFill_outPin = 9,
    leftFill_diagPin = 2,
    leftDump_outPin = 7,
    leftDump_diagPin = 8,
    leftSuck_outPin = 24,
    leftSuck_diagPin = 26,
    rightFill_outPin = 10,
    rightFill_diagPin = 11,
    rightDump_outPin = 13,
    rightDump_diagPin = 12,
    rightSuck_outPin = 25,
    rightSuck_diagPin = 27,
    airdryer_outPin = 6,
    airdryer_diagPin = 1,
	spare1_outPin = 23,
	spare1_diagPin = 14;

// compressor out BTS6133D
static const uint8_t
    compressor_outPin = 5,
    compressor_diagPin = 0,
    compressor_csPin = A8; //22;

static const uint8_t
    CAN_TX_pin = 3,
    CAN_RX_pin = 4;

static const uint8_t
    leftPressureAD_pin          = A3, //17,
    leftHeightAD_pin            = A6, //20,
    rightPressureAD_pin         = A2,//16,
    rightHeightAD_pin           = A5,//19,
    systemPressureAD_pin    	= A4,//18,
    spare1AD_pin                = A7,//21,
    compressorTempAD_pin        = A12,
    spareTemp1AD_pin            = A1; // 15


#endif /* PINOUTS_H_ */
