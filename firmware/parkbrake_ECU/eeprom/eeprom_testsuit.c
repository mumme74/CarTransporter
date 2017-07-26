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

#include <string.h>

#include "ch.h"
#include "hal.h"

#include "chprintf.h"

#include "eeprom/eeprom_testsuit.h"
#include "eeprom/eeprom.h"

#if USE_EEPROM_TEST_SUIT

#define TEST_AREA_START     0
#define TEST_AREA_SIZE      8192
#define TEST_AREA_END       (TEST_AREA_START + TEST_AREA_SIZE)

/* Shortcut to print OK message. */
#define OK() do { \
    chprintf((chp), " ... OK\r\n"); \
    chThdSleepMilliseconds(20); \
  } while (0)


/* buffer containing control data */
static uint8_t referencebuf[TEST_AREA_SIZE];
/* buffer for read data */
static uint8_t checkbuf[TEST_AREA_SIZE];

static SPIEepromFileStream ifile;
static SPIEepromFileStream ofile;

extern const SPIConfig EEPROM_SPIDCONFIG;

static SPIEepromFileConfig ocfg = {
  0,
  0,
  EEPROM_SIZE,
  EEPROM_PAGE_SIZE,
  MS2ST(EEPROM_WRITE_TIME_MS),
  &EEPROM_SPID,
  &EEPROM_SPIDCONFIG,
};

static SPIEepromFileConfig icfg = {
  0,
  0,
  EEPROM_SIZE,
  EEPROM_PAGE_SIZE,
  MS2ST(EEPROM_WRITE_TIME_MS),
  &EEPROM_SPID,
  &EEPROM_SPIDCONFIG,
};

/*
 *******************************************************************************
 * LOCAL FUNCTIONS
 *******************************************************************************
 */
/**
 *
 */
static void printfileinfo(BaseSequentialStream *chp, EepromFileStream *efsp) {
  chprintf(chp, "size = %u, position = %u, barrier_low = %u, barrier_hi = %u\r\n",
           fileStreamGetSize(efsp),
           fileStreamGetPosition(efsp),
           efsp->cfg->barrier_low,
           efsp->cfg->barrier_hi);
  chThdSleepMilliseconds(100);
}

/**
 * Fills eeprom area with pattern, than read it back and compare
 */
static void pattern_fill(EepromFileStream *EfsTest, uint8_t pattern) {
  msg_t i = 0;
  msg_t status = 0;
  msg_t pos = 0;
  msg_t len = fileStreamGetSize(EfsTest);

  /* fill buffer with pattern */
  for (i = 0; i < len; i++)
    referencebuf[i] = pattern;

  /* move to begin of test area */
  pos = 0;
  fileStreamSeek(EfsTest, pos);
  if (pos != fileStreamGetPosition(EfsTest))
    chSysHalt("file seek error");

  /* write */
  status = fileStreamWrite(EfsTest, referencebuf, len);
  if (status != len)
    chSysHalt("write failed");

  /* check */
  pos = fileStreamGetPosition(EfsTest);
  if (pos != len)
    chSysHalt("writing error");

  pos = 0;
  fileStreamSeek(EfsTest, pos);
  status = fileStreamRead(EfsTest, checkbuf, len);
  if (status != len)
    chSysHalt("reading back failed");
  if (memcmp(referencebuf, checkbuf, len) != 0)
    chSysHalt("veryfication failed");
}

/**
 * Create overlapped files like this:
 *
 *       |<--------- outer file ------------>|
 *       |                                   |
 * ======b1==b2========================b3===b4======
 * |         |                          |          |
 * |         |<------ inner file ------>|          |
 * |<----------------- EEPROM -------------------->|
 */
static void overflow_check(uint32_t b1, uint32_t b2, uint32_t b3, uint32_t b4,
                           uint32_t istart, uint32_t ilen,
                           uint8_t pattern, bool pat_autoinc,
                           BaseSequentialStream *chp) {

  uint32_t status, i, n;
  EepromFileStream *iefs;
  EepromFileStream *oefs;

  chDbgAssert(ilen < (b4 - b1), "sequences more than length of outer file can not be verified");

  chprintf(chp, "b1=%u, b2=%u, b3=%u, b4=%u, istart=%u, ilen=%u, ",
           b1, b2, b3, b4, istart, ilen);
  chprintf(chp, "autoinc=");
  if (pat_autoinc)
    chprintf(chp, "TRUE");
  else
    chprintf(chp, "FALSE");
  chThdSleepMilliseconds(50);

  /* open outer file and clear it */
  ocfg.barrier_low  = b1;
  ocfg.barrier_hi   = b4;
  oefs = SPIEepromFileOpen(&ofile, &ocfg, EepromFindDevice(EEPROM_DRIVER_NAME));
  pattern_fill(oefs, 0x00);

  /* open inner file */
  icfg.barrier_low  = b2;
  icfg.barrier_hi   = b3;
  iefs = SPIEepromFileOpen(&ifile, &icfg, EepromFindDevice(EEPROM_DRIVER_NAME));

  /* reference buffer */
  memset(referencebuf, 0x00, b4 - b1);
  n = b2 - b1 + istart;
  if ((ilen + istart) > (b3 - b2))
    i = b3 - b2 - istart;
  else
    i = ilen;
  while (i > 0) {
    referencebuf[n] = pattern;
    n++;
    i--;
    if (pat_autoinc)
      pattern++;
  }

  /* check buffer */
  n = 0;
  while (n < ilen) {
    checkbuf[n] = pattern;
    n++;
    if (pat_autoinc)
      pattern++;
  }

  /* now write check buffer content into inner file */
  chThdSleepMilliseconds(20);
  fileStreamSeek(iefs, istart);
  status = fileStreamWrite(iefs, checkbuf, ilen);

  if ((istart + ilen) > (b3 - b2)) { /* data must be clamped */
    if (status != (b3 - b2 - istart))
      chSysHalt("not all data written or overflow ocrred");
  }
  else {   /* data fitted in file */
    if (status != ilen)
      chSysHalt("not all data written or overflow ocrred");
  }

  /* read outer file and compare content with reference buffer */
  memset(checkbuf, 0x00, b4 - b1);
  fileStreamSeek(oefs, 0);
  status = fileStreamRead(oefs, checkbuf, b4 - b1);
  if (status != (b4 - b1))
    chSysHalt("reading back failed");
  if (memcmp(referencebuf, checkbuf, b4 - b1) != 0)
    chSysHalt("verification failed");

  fileStreamClose(oefs);
  fileStreamClose(iefs);
  OK();
}

/**
 *
 */
//static THD_WORKING_AREA(waEepromTestThread, 1024);
static THD_FUNCTION(EepromTestThread, *ctx) {

  BaseSequentialStream *chp = (BaseSequentialStream *)ctx;
  EepromFileStream *oefs;

  chRegSetThreadName("EepromTst");

  chprintf(chp, "basic tests\r\n");
  chprintf(chp, "--------------------------------------------------------------\r\n");
  chprintf(chp, "mount aligned file sized to whole test area");
  ocfg.barrier_low  = TEST_AREA_START;
  ocfg.barrier_hi   = TEST_AREA_END;
  oefs = SPIEepromFileOpen(&ofile, &ocfg, EepromFindDevice(EEPROM_DRIVER_NAME));
  OK();
  printfileinfo(chp, oefs);
  chprintf(chp, "test fill with 0xFF");
  pattern_fill(oefs, 0xFF);
  if (chThdShouldTerminateX()) {
    goto END;
  }
  OK();
  chprintf(chp, "test fill with 0xAA");
  pattern_fill(oefs, 0xAA);
  if (chThdShouldTerminateX()) {
    goto END;
  }
  OK();
  chprintf(chp, "test fill with 0x55");
  pattern_fill(oefs, 0x55);
  if (chThdShouldTerminateX()) {
    goto END;
  }
  OK();
  chprintf(chp, "test fill with 0x00");
  pattern_fill(oefs, 0x00);
  if (chThdShouldTerminateX()) {
    goto END;
  }
  OK();
  chprintf(chp, "Closing file");
  fileStreamClose(oefs);
  OK();


  uint32_t b1, b2, b3, b4, istart, ilen;
  uint8_t pattern = 0x55;
  b1 = TEST_AREA_START;
  b2 = TEST_AREA_START + EEPROM_PAGE_SIZE;
  b3 = TEST_AREA_END - EEPROM_PAGE_SIZE;
  b4 = TEST_AREA_END;
  istart = 0;
  ilen = b3 - b2;

  chprintf(chp, "    Linear barriers testing.\r\n");
  chThdSleepMilliseconds(20);
  overflow_check( b1, b2, b3, b4, istart, ilen, pattern, FALSE, chp);
  if (chThdShouldTerminateX()) {
    goto END;
  }
  pattern++;
  overflow_check( b1, b2, b3, b4, istart + 1, ilen - 1, pattern, FALSE, chp);
  if (chThdShouldTerminateX()) {
    goto END;
  }
  pattern++;
  overflow_check( b1, b2, b3, b4, istart + 1, ilen, pattern, FALSE, chp);
  if (chThdShouldTerminateX()) {
    goto END;
  }
  pattern++;
  overflow_check( b1, b2, b3, b4, istart + 1, ilen + 23, pattern, FALSE, chp);
  if (chThdShouldTerminateX()) {
    goto END;
  }
  pattern++;
  overflow_check( b1, b2 - 1, b3 + 1, b4, istart, ilen, pattern,  FALSE, chp);
  if (chThdShouldTerminateX()) {
    goto END;
  }
  pattern++;
  overflow_check( b1, b2 - 2, b3 + 2, b4, istart + 2, ilen, pattern,  FALSE, chp);
  if (chThdShouldTerminateX()) {
    goto END;
  }
  pattern++;
  overflow_check( b1, b2 - 1, b3 + 1, b4, istart + 1, ilen + 23, pattern,  FALSE, chp);
  if (chThdShouldTerminateX()) {
    goto END;
  }
  pattern++;
  overflow_check( b1, b2 - 2, b3 + 2, b4, istart + 1, ilen + 23, pattern,  FALSE, chp);
  if (chThdShouldTerminateX()) {
    goto END;
  }
  pattern++;
  overflow_check( b1, b2 + 2, b3 - 3, b4, istart + 2, ilen, pattern, FALSE,  chp);
  if (chThdShouldTerminateX()) {
    goto END;
  }
  pattern++;


  overflow_check( b1, b2, b2 + 1, b4, istart, ilen, pattern,  FALSE,  chp);
  if (chThdShouldTerminateX()) {
    goto END;
  }
  pattern++;
  overflow_check( b1, b2, b2 + 2, b4, istart, ilen, pattern, FALSE, chp);
  if (chThdShouldTerminateX()) {
    goto END;
  }
  pattern++;
  overflow_check( b1, b2, b2 + 3, b4, istart, ilen, pattern, FALSE, chp);
  if (chThdShouldTerminateX()) {
    goto END;
  }
  pattern++;


  overflow_check( b1, b2 + 1, b2 + 2, b4, istart, ilen, pattern, FALSE, chp);
  if (chThdShouldTerminateX()) {
    goto END;
  }
  pattern++;
  overflow_check( b1, b2 + 1, b2 + 3, b4, istart, ilen, pattern, FALSE, chp);
  if (chThdShouldTerminateX()) {
    goto END;
  }
  pattern++;
  overflow_check( b1, b2 + 1, b2 + 4, b4, istart, ilen, pattern, FALSE, chp);
  if (chThdShouldTerminateX()) {
    goto END;
  }
  pattern++;

  overflow_check( b1, b2 - 1, b2,     b4, istart, ilen, pattern, FALSE, chp);
  if (chThdShouldTerminateX()) {
    goto END;
  }
  pattern++;
  overflow_check( b1, b2 - 1, b2 + 1, b4, istart, ilen, pattern, FALSE, chp);
  if (chThdShouldTerminateX()) {
    goto END;
  }
  pattern++;
  overflow_check( b1, b2 - 1, b2 + 2, b4, istart, ilen, pattern, FALSE, chp);
  if (chThdShouldTerminateX()) {
    goto END;
  }
  pattern++;

  overflow_check( b1, b2 - 1, b2 + 1, b4, istart + 1, ilen, pattern, FALSE, chp);
  if (chThdShouldTerminateX()) {
    goto END;
  }
  pattern++;
  overflow_check( b1, b2 - 1, b2 + 2, b4, istart + 1, ilen, pattern, FALSE, chp);
  if (chThdShouldTerminateX()) {
    goto END;
  }
  pattern++;
  overflow_check( b1, b2 - 1, b2 + 3, b4, istart + 1, ilen, pattern, FALSE, chp);
  if (chThdShouldTerminateX()) {
    goto END;
  }
  pattern++;

  chprintf(chp, "    Basic API testing.\r\n");
  chThdSleepMilliseconds(20);

  ocfg.barrier_low  = TEST_AREA_START;
  ocfg.barrier_hi   = TEST_AREA_END;
  oefs = SPIEepromFileOpen(&ofile, &ocfg, EepromFindDevice(EEPROM_DRIVER_NAME));
  fileStreamSeek(oefs, 0);
  EepromWriteByte(oefs, 0x11);
  EepromWriteHalfword(oefs, 0x2222);
  EepromWriteWord(oefs, 0x33333333);
  fileStreamSeek(oefs, 0);
  if (EepromReadByte(oefs) != 0x11)
    chSysHalt("");
  if (EepromReadHalfword(oefs) != 0x2222)
    chSysHalt("");
  if (EepromReadWord(oefs) != 0x33333333)
    chSysHalt("");
  fileStreamClose(oefs);
  OK();

  chprintf(chp, "All tests passed successfully.\r\n");
END:
  chThdExit(0);
}

void eeprom_cmd_test(BaseSequentialStream *chp, int argc, char *argv[]) {
  thread_t *tp;
  (void)argc;
  (void)argv;

  tp = chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(1024),
                           "EepromTestThread", NORMALPRIO,
                           (tfunc_t)EepromTestThread, chp);
  if (tp == NULL) {
    chprintf(chp, "Out of memory\r\n");
    return;
  }

  chThdWait(tp);
}

#endif /* USE_EEPROM_TEST_SUIT */
