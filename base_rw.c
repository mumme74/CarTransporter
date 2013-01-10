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

#include "eeprom/base_rw.h"

fileoffset_t eepfs_getsize(void *ip) {

  uint32_t h, l;

  chDbgCheck((ip != NULL) && (((EepromFileStream *)ip)->vmt != NULL) &&
             (((EepromFileStream *)ip)->cfg != NULL), "eepfs_getsize");

  h = ((EepromFileStream *)ip)->cfg->barrier_hi;
  l = ((EepromFileStream *)ip)->cfg->barrier_low;
  return  h - l;
}

fileoffset_t eepfs_getposition(void *ip) {

  chDbgCheck((ip != NULL) && (((EepromFileStream *)ip)->vmt != NULL),
             "eepfs_getposition");

  return ((EepromFileStream *)ip)->position;
}

fileoffset_t eepfs_lseek(void *ip, fileoffset_t offset) {

  uint32_t size;

  chDbgCheck((ip != NULL) && (((EepromFileStream *)ip)->vmt != NULL),
             "eepfs_lseek");

  size = eepfs_getsize(ip);
  if (offset > size)
    offset = size;
  ((EepromFileStream *)ip)->position = offset;
  return offset;
}

uint32_t eepfs_close(void *ip) {

  chDbgCheck((ip != NULL) && (((EepromFileStream *)ip)->vmt != NULL),
             "eepfs_close");

  ((EepromFileStream *)ip)->errors   = FILE_OK;
  ((EepromFileStream *)ip)->position = 0;
  ((EepromFileStream *)ip)->vmt      = NULL;
  ((EepromFileStream *)ip)->cfg      = NULL;
  return FILE_OK;
}

int eepfs_geterror(void *ip) {

  chDbgCheck((ip != NULL) && (((EepromFileStream *)ip)->vmt != NULL),
             "eepfs_geterror");
  return ((EepromFileStream *)ip)->errors;
}

msg_t eepfs_put(void *ip, uint8_t b) {

  (void)ip;
  (void)b;
  return 0;
}

msg_t eepfs_get(void *ip) {

  (void)ip;
  return 0;
}
