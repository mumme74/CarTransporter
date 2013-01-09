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

#include "eeprom_i2c.h"

#if HAL_USE_I2C || defined(__DOXYGEN__)

/**
 * Open I2C EEPROM IC as file and return pointer to the file stream object
 * @note      Fucntion allways successfully open file. All checking makes
 *            in read/write functions.
 */
EepromFileStream *EepromFileOpenI2C(I2CEepromFileStream *efs,
                                    const I2CEepromFileConfig *eepcfg,
                                    const I2CEepromDevice *eepdev) {

  chDbgCheck((efs != NULL) && (eepcfg != NULL), "");
  chDbgCheck(efs->vmt != eepdev->efsvmt, "file allready opened");
  chDbgCheck(eepcfg->barrier_hi > eepcfg->barrier_low, "wrong barriers")
  chDbgCheck(eepcfg->pagesize < eepcfg->size, "pagesize can not be lager than EEPROM size")
  chDbgCheck(eepcfg->barrier_hi <= eepcfg->size, "barrier out of bounds")

  efs->vmt = eepdev->efsvmt;
  efs->cfg = eepcfg;
  efs->errors = FILE_OK;
  efs->position = 0;
  return (EepromFileStream *)efs;
}

#endif /* HAL_USE_I2C */
