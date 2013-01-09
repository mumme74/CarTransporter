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

#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "ch.h"
#include "hal.h"

#define _eeprom_file_config_data                                            \
  /* Lower barrier of file in EEPROM memory array. */                       \
  uint32_t        barrier_low;                                              \
  /* Higher barrier of file in EEPROM memory array. */                      \
  uint32_t        barrier_hi;                                               \
  /* Size of memory array in bytes. */                                      \
  uint32_t        size;                                                     \
  /* Size of single page in bytes. */                                       \
  uint16_t        pagesize;                                                 \
  /* Time needed by IC for single byte/page writing. */                     \
  systime_t       write_time;

typedef struct {
  _eeprom_file_config_data
} EepromFileConfig;

/**
 * @brief   @p EepromFileStream specific data.
 */
#define _eeprom_file_stream_data                                            \
  _base_file_stream_data                                                    \
  uint32_t                    errors;                                       \
  uint32_t                    position;                                     \

/**
 * @extends BaseFileStreamVMT
 *
 * @brief   @p EepromFileStream virtual methods table.
 */
struct EepromFilelStreamVMT {
  _base_file_stream_methods
};

/**
 * @extends BaseFileStream
 *
 * @brief   EEPROM file stream driver class.
 * @details This class extends @p BaseFileStream by adding some fields.
 */
typedef struct {
  /** @brief Virtual Methods Table.*/
  const struct EepromFilelStreamVMT *vmt;
  _eeprom_file_stream_data
  /** pointer to config object, must be overwritten by all derived classes.*/
  const EepromFileConfig *cfg;
} EepromFileStream;

#if !defined(chFileStreamRead)
/**
 * @brief   File Stream read.
 * @details The function reads data from a file into a buffer.
 *
 * @param[in] ip        pointer to a @p BaseSequentialStream or derived class
 * @param[out] bp       pointer to the data buffer
 * @param[in] n         the maximum amount of data to be transferred
 * @return              The number of bytes transferred. The return value can
 *                      be less than the specified number of bytes if the
 *                      stream reaches the end of the available data.
 *
 * @api
 */
#define chFileStreamRead(ip, bp, n)  (chSequentialStreamRead(ip, bp, n))
#endif

#if !defined(chFileStreamWrite)
/**
 * @brief   File Stream write.
 * @details The function writes data from a buffer to a file.
 *
 * @param[in] ip        pointer to a @p BaseSequentialStream or derived class
 * @param[in] bp        pointer to the data buffer
 * @param[in] n         the maximum amount of data to be transferred
 * @return              The number of bytes transferred. The return value can
 *                      be less than the specified number of bytes if the
 *                      stream reaches a physical end of file and cannot be
 *                      extended.
 *
 * @api
 */
#define chFileStreamWrite(ip, bp, n) (chSequentialStreamWrite(ip, bp, n))
#endif

uint8_t  EepromReadByte(EepromFileStream *efs);
uint16_t EepromReadHalfword(EepromFileStream *efs);
uint32_t EepromReadWord(EepromFileStream *efs);
size_t EepromWriteByte(EepromFileStream *efs, uint8_t data);
size_t EepromWriteHalfword(EepromFileStream *efs, uint16_t data);
size_t EepromWriteWord(EepromFileStream *efs, uint32_t data);

#include "eeprom/eeprom_i2c.h"
#include "eeprom/eeprom_spi.h"

#endif /* __EEPROM_H__ */
