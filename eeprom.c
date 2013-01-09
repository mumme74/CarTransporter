/*
  Copyright 2012 Uladzimir Pylinski aka barthess.
  You may use this work without restrictions, as long as this notice is included.
  The work is provided "as is" without warranty of any kind, neither express nor implied.
*/

#include "ch.h"
#include "hal.h"

#include "eeprom/eeprom.h"

uint8_t EepromReadByte(EepromFileStream *efs) {

  uint8_t buf;
  chFileStreamRead(efs, &buf, sizeof(buf));
  return buf;
}

uint16_t EepromReadHalfword(EepromFileStream *efs) {

  uint16_t buf;
  chFileStreamRead(efs, (uint8_t *)&buf, sizeof(buf));
  return buf;
}

uint32_t EepromReadWord(EepromFileStream *efs) {

  uint32_t buf;
  chFileStreamRead(efs, (uint8_t *)&buf, sizeof(buf));
  return buf;
}

size_t EepromWriteByte(EepromFileStream *efs, uint8_t data) {

  return chFileStreamWrite(efs, &data, sizeof(data));
}

size_t EepromWriteHalfword(EepromFileStream *efs, uint16_t data) {

  return chFileStreamWrite(efs, (uint8_t *)&data, sizeof(data));
}

size_t EepromWriteWord(EepromFileStream *efs, uint32_t data) {

  return chFileStreamWrite(efs, (uint8_t *)&data, sizeof(data));
}
