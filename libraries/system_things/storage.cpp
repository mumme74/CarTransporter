/*
 * storage.cpp
 *
 *  Created on: 6 apr 2015
 *      Author: jof
 */

#include "storage.h"
#include <EEPROM.h>

namespace store {


byte4 read4_from_eeprom(uint16_t startAdr) {
  byte4 b4;
  b4.buf[0] = EEPROM.read(startAdr);
  b4.buf[1] = EEPROM.read(startAdr + 1);
  b4.buf[2] = EEPROM.read(startAdr + 2);
  b4.buf[3] = EEPROM.read(startAdr + 3);

  return b4;
}
byte2 read2_from_eeprom(uint16_t startAdr) {
  byte2 b2;
  b2.buf[0] = EEPROM.read(startAdr);
  b2.buf[1] = EEPROM.read(startAdr + 1);
  return b2;
}
void  write4_to_eeprom(uint16_t startAdr, byte4 &vlu) {
  EEPROM.write(startAdr, vlu.buf[0]);
  EEPROM.write(startAdr + 1, vlu.buf[1]);
  EEPROM.write(startAdr + 2, vlu.buf[2]);
  EEPROM.write(startAdr + 3, vlu.buf[3]);
}
void  write2_to_eeprom(uint16_t startAdr, byte2 &vlu) {
  EEPROM.write(startAdr, vlu.buf[0]);
  EEPROM.write(startAdr + 1, vlu.buf[1]);
}

}; // namespace store
