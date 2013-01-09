/*
  Copyright (c) 2013 Timon Wong

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

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
