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

#ifndef __EEPROM_SPI_H__
#define __EEPROM_SPI_H__

#if HAL_USE_SPI || defined(__DOXYGEN__)

/**
 * @extends EepromFileConfig
 */
typedef struct {
  _eeprom_file_config_data
  /**
   * Driver connected to IC.
   */
  SPIDriver       *spip;
  /**
   * Config associated with SPI driver.
   */
  const SPIConfig *spicfg;
} SPIEepromFileConfig;

/**
 * @brief   @p SPIEepromFileStream specific data.
 */
#define _eeprom_file_stream_data_spi                                       \
  _eeprom_file_stream_data

/**
 * @extends EepromFileStream
 *
 * @brief   EEPROM file stream driver class for SPI device.
 */
typedef struct {
  const struct EepromFilelStreamVMT *vmt;
  _eeprom_file_stream_data_spi
  /* Overwritten parent data member. */
  const SPIEepromFileConfig *cfg;
} SPIEepromFileStream;

/**
 * Open I2C EEPROM IC as file and return pointer to the file stream object
 * @note      Fucntion allways successfully open file. All checking makes
 *            in read/write functions.
 */
EepromFileStream *SPIEepromFileOpen(SPIEepromFileStream *efs,
                                    const SPIEepromFileConfig *eepcfg,
                                    const EepromDevice *eepdev);

#define SPIEepromFileOpen(efs, eepcfg, eepdev) \
  EepromFileOpen((EepromFileStream *)efs, (EepromFileConfig *)eepcfg, eepdev);

#endif /* HAL_USE_SPI */

#endif /* __EEPROM_SPI_H__ */
