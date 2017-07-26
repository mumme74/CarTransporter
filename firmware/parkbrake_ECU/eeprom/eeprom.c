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
#include <string.h>

#include "eeprom/eeprom.h"

/**
 * @breif  Find low level EEPROM device by id.
 */
const EepromDevice *EepromFindDevice(const char *name) {

  extern EepromDevice *__eeprom_drv_table[];
  int i;
  const EepromDevice *drv;

  chDbgAssert(name != NULL, "EepromFindDevice");

  for (i = 0; i < EEPROM_DRV_TABLE_SIZE; i++) {
    drv = __eeprom_drv_table[i];
    if (drv->name != NULL && !strcmp(drv->name, name)) {
      return drv;
    }
  }

  return NULL;
}

/**
 * Open EEPROM IC as file and return pointer to the file stream object
 * @note      Fucntion allways successfully open file. All checking makes
 *            in read/write functions.
 */
EepromFileStream *EepromFileOpen(EepromFileStream *efs,
                                 const EepromFileConfig *eepcfg,
                                 const EepromDevice *eepdev) {

  chDbgAssert((efs != NULL) && (eepcfg != NULL) && (eepdev != NULL) &&
             (eepdev->efsvmt != NULL), "EepromFileOpen");
  chDbgAssert(efs->vmt != eepdev->efsvmt, "file allready opened");
  chDbgAssert(eepcfg->barrier_hi > eepcfg->barrier_low, "wrong barriers");
  chDbgAssert(eepcfg->pagesize < eepcfg->size, "pagesize can not be lager than EEPROM size");
  chDbgAssert(eepcfg->barrier_hi <= eepcfg->size, "barrier out of bounds");

  efs->vmt      = eepdev->efsvmt;
  efs->cfg      = eepcfg;
  efs->errors   = FILE_OK;
  efs->position = 0;
  return (EepromFileStream *)efs;
}

uint8_t EepromReadByte(EepromFileStream *efs) {

  uint8_t buf;
  fileStreamRead(efs, &buf, sizeof(buf));
  return buf;
}

uint16_t EepromReadHalfword(EepromFileStream *efs) {

  uint16_t buf;
  fileStreamRead(efs, (uint8_t *)&buf, sizeof(buf));
  return buf;
}

uint32_t EepromReadWord(EepromFileStream *efs) {

  uint32_t buf;
  fileStreamRead(efs, (uint8_t *)&buf, sizeof(buf));
  return buf;
}

size_t EepromWriteByte(EepromFileStream *efs, uint8_t data) {

  return fileStreamWrite(efs, &data, sizeof(data));
}

size_t EepromWriteHalfword(EepromFileStream *efs, uint16_t data) {

  return fileStreamWrite(efs, (uint8_t *)&data, sizeof(data));
}

size_t EepromWriteWord(EepromFileStream *efs, uint32_t data) {

  return fileStreamWrite(efs, (uint8_t *)&data, sizeof(data));
}
