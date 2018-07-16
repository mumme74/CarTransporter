/*
 * storage.h
 *
 *  Created on: 4 apr 2015
 *      Author: jof
 */

#ifndef STORAGE_H_
#define STORAGE_H_


#include <stdint.h>

namespace store {
static const uint8_t
  DTC_BYTESIZE = 4,
  DTC_ADRESS_HI_OFFSET  = 0,
  DTC_ADRESS_LO_OFFSET  = 1,
  DTC_ERRTYPE_OFFSET    = 2,
  DTC_OCCURENCES_OFFSET = 3;

static const uint16_t
  // reserved 0-9
  // 10 - 128 config section for each node

  // DTC 128- 255
  // this is implemented as a ringstore
  DTC_COUNT_ADR       = 0x0080,
  DTC_STARTPTR_ADR_LO = 0x0081, // first DTC ptr in ringstore
  DTC_STARTPTR_ADR_HI = 0x0082,
  DTC_ENDPTR_ADR_LO   = 0x0083, // last DTC ptr in ringstore
  DTC_ENDPTR_ADR_HI   = 0x0084,
  DTC_STORE_START_ADR = 0x0085, // first byte in first DTC
  DTC_STORE_END_ADR   = 0x00FF // last byte in last DTC
  // DTC store 0xFF - 0x85 = 0x7A = 120
  // 4bytes in each DTC = 120/4 = 30 DTCs

  ;

static const uint8_t
  DTC_MAXCOUNT = (DTC_STORE_END_ADR - DTC_STORE_START_ADR) / 4;

namespace Suspension {
static const uint16_t
  // height state machine 10 - 100
  HEIGHT_WANTED_STATE_ADR  = 0x000A,
  HEIGHT_WANTED_STATE_SIZE = 2,
  HEIGHT_KP_ADR            = 0x000E,
  HEIGHT_KP_SIZE           = 4,
  HEIGHT_KI_ADR            = 0x0012,
  HEIGHT_KI_SIZE           = 4,
  HEIGHT_KD_ADR            = 0x0016,
  HEIGHT_KD_SIZE           = 4,
  HEIGHT_LEFT_LOW_ADR      = 0x001A, // setpoint for left low position
  HEIGHT_LEFT_LOW_SIZE     = 2,
  HEIGHT_RIGHT_LOW_ADR     = 0x001C, // setpoint for right low position
  HEIGHT_RIGHT_LOW_SIZE    = 2,
  HEIGHT_LEFT_NORMAL_ADR   = 0x001E, // setpoint for left normal position
  HEIGHT_LEFT_NORMAL_SIZE  = 2,
  HEIGHT_RIGHT_NORMAL_ADR  = 0x0020, // setpoint for right normal position
  HEIGHT_RIGHT_NORMAL_SIZE = 2,
  HEIGHT_LEFT_HIGH_ADR     = 0x0022, // setpoint for left high position
  HEIGHT_LEFT_HIGH_SIZE    = 2,
  HEIGHT_RIGHT_HIGH_ADR    = 0x0024,
  HEIGHT_RIGHT_HIGH_SIZE   = 2,
  HEIGHT_CYL_DIA_MM_ADR    = 0x0026, // air bellows diameter
  HEIGHT_CYL_DIA_MM_SIZE   = 2,
  HEIGHT_DEAD_WEIGHT_ADR   = 0x0028, // the chassi weight without anything on top flatbed
  HEIGHT_DEAD_WEIGHT_SIZE  = 2,
  HEIGHT_DEAD_BAND_ADR     = 0x002A,
  HEIGHT_DEAD_BAND_SIZE    = 2,
  HEIGHT_SENSORS_REVERSED_ADR  = 0x002C, // bit0 = left, bit1 = right
  HEIGHT_SENSORS_REVERSED_SIZE = 1

  ;
}; // namespace Suspension

union byte4 {
  uint8_t  buf[4];
  float    decimal;
  uint32_t uint32;
  int32_t  int32;
};

union byte2 {
  uint8_t  buf[2];
  uint16_t uint16;
  int16_t  int16;
};

union byte1 {
	uint8_t uint8;
	int8_t  int8;
};

byte4 read4_from_eeprom(uint16_t startAdr);
byte2 read2_from_eeprom(uint16_t startAdr);
byte1 read1_from_eeprom(uint16_t startAdr);
void  write4_to_eeprom(uint16_t startAdr, byte4 &vlu);
void  write2_to_eeprom(uint16_t startAdr, byte2 &vlu);
void  write1_to_eeprom(uint16_t startAdr, byte1 &vlu);

}; // namespace store



#endif /* STORAGE_H_ */
