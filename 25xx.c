/*
Copyright 2012 Uladzimir Pylinski aka barthess.
You may use this work without restrictions, as long as this notice is included.
The work is provided "as is" without warranty of any kind, neither express nor implied.
*/

/*****************************************************************************
 * DATASHEET NOTES
 *****************************************************************************
Write cycle time (byte or page) - 5 ms

Note:
  Page write operations are limited to writing bytes within a single physical
  page, regardless of the number of bytes actually being written. Physical page
  boundaries start at addresses that are integer multiples of the page buffer
  size (or page size and end at addresses that are integer multiples of
  [page size]. If a Page Write command attempts to write across a physical
  page boundary, the result is that the data wraps around to the beginning of
  the current page (overwriting data previously stored there), instead of
  being written to the next page as might be expected.
*********************************************************************/

#include <string.h>

#include "ch.h"
#include "hal.h"

#include "eeprom/25xx.h"

/**
 * @name Commands of 25XX chip.
 * @{
 */
#define CMD_READ    0x03  /**< @brief Read data from memory array beginning at
                               selected address. */
#define CMD_WRITE   0x02  /**< @brief Write data to memory array beginning at
                               selected address. */
#define CMD_WRDI    0x04  /**< Reset the write enable latch (disable write
                               operations). */
#define CMD_WREN    0x06  /**< Set the write enable latch (enable write
                               operations). */
#define CMD_RDSR    0x05  /**< Read STATUS register. */
#define CMD_WRSR    0x01  /**< Write STATUS register. */

/** @} */

/**
 * @name Status of 25XX chip.
 * @{}
 */
#define STAT_BP1    0x08  /**< @brief Block protection (high). */
#define STAT_BP0    0x04  /**< @brief Block protection (low). */
#define STAT_WEL    0x02  /**< @brief Write enable latch. */
#define STAT_WIP    0x01  /**< @brief Write-In-Progress. */

/** @} */

/**
 * @brief 25XX chip low level write then read rountine.
 *
 * @param[in]  spip   pointer to SPI device.
 * @param[in]  txbuf  pointer to buffer to be transfered.
 * @param[in]  txlen  number of bytes to be transfered.
 * @param[out] rxbuf  pointer to buffer to be received.
 * @param[in]  rxlen  number of bytes to be received.
 */
static void ll_25xx_transmit_receive(SPIDriver *spip,
                                     const uint8_t *txbuf, size_t txlen,
                                     uint8_t *rxbuf, size_t rxlen) {

#if SPI_USE_MUTUAL_EXCLUSION
  spiAcquireBus(spip);
#endif

  spiSelect(spip);
  spiSend(spip, txlen, &txbuf);
  if (rxlen) /* Check if receive is needed. */
    spiReceive(spip, rxlen, &rxbuf);
  spiUnselect(spip);

#if SPI_USE_MUTUAL_EXCLUSION
  spiReleaseBus(spip);
#endif
}

/**
 * @brief Lock device.
 *
 * @param[in] spip  pointer to SPI device.
 */
static void ll_25xx_lock(SPIDriver *spip) {

  uint8_t cmd = CMD_WRDI;
  ll_25xx_transmit_receive(spip, &cmd, 1, NULL, 0);
}

/**
 * @brief Unlock device.
 *
 * @param[in] spip  pointer to SPI device.
 */
static void ll_25xx_unlock(SPIDriver *spip) {

  uint8_t cmd = CMD_WREN;
  ll_25xx_transmit_receive(spip, &cmd, 1, NULL, 0);
}

static bool_t ll_25xx_is_busy(SPIDriver *spip) {

  uint8_t cmd = CMD_RDSR;
  uint8_t stat;
  ll_25xx_transmit_receive(spip, &cmd, 1, &stat, 1);
  if (stat & STAT_WIP)
    return TRUE;
  return FALSE;
}

/**
 * @brief   Prepare byte sequence for command and address
 *
 * @param[in] seq   pointer to first 3byte sequence
 * @param[in] size  size of the eeprom device
 * @param[in] cmd   command
 * @param[in] addr  address
 * @return number of bytes of this sequence
 */
static uint8_t ll_eeprom_prepare_seq(uint8_t *seq, uint32_t size, uint8_t cmd,
                                     uint32_t addr) {

  seq[0] = ((uint8_t)cmd & 0xff);

  if (size > 0xffffUL) {
    /* High density, 24bit address. */
    seq[1] = (uint8_t)((addr >> 16) & 0xff);
    seq[2] = (uint8_t)((addr >> 8) & 0xff);
    seq[3] = (uint8_t)(addr & 0xff);
    return 4;
  }
  else if (size > 0x00ffUL) {
    /* Medium density, 16bit address. */
    seq[1] = (uint8_t)((addr >> 8) & 0xff);
    seq[2] = (uint8_t)(addr & 0xff);
    return 3;
  }

  /* Low density, 8bit address. */
  seq[1] = (uint8_t)(addr & 0xff);
  return 2;
}

msg_t ll_eeprom_read(const SPIEepromFileConfig *spefcp, uint32_t offset,
                     uint8_t *data, size_t len) {

  uint8_t txbuff[4];
  uint8_t txlen;

  chDbgCheck(((len <= spefcp->size) && ((offset + len) <= spefcp->size)),
             "out of device bounds");

  txlen = ll_eeprom_prepare_seq(txbuff, spefcp->size, CMD_READ,
                                (offset + spefcp->barrier_low));
  ll_25xx_transmit_receive(spefcp->spip, txbuff, txlen, data, len);

  return RDY_OK;
}

msg_t ll_eeprom_write(const SPIEepromFileConfig *spefcp, uint32_t offset,
                      const uint8_t *data, size_t len) {

  uint8_t txbuff[4];
  uint8_t txlen;
  systime_t now;

  chDbgCheck(((len <= spefcp->size) && ((offset + len) <= spefcp->size)),
             "out of device bounds");
  chDbgCheck((((offset + spefcp->barrier_low) / spefcp->pagesize) ==
              (((offset + spefcp->barrier_low) + len - 1) / spefcp->pagesize)),
             "data can not be fitted in single page");

  /* Unlock array for writting. */
  ll_25xx_unlock(spefcp->spip);

#if SPI_USE_MUTUAL_EXCLUSION
  spiAcquireBus(spefcp->spip);
#endif

  spiSelect(spefcp->spip);
  txlen = ll_eeprom_prepare_seq(txbuff, spefcp->size, CMD_WRITE,
                                (offset + spefcp->barrier_low));
  spiSend(spefcp->spip, txlen, txbuff);
  spiSend(spefcp->spip, len, data);
  spiUnselect(spefcp->spip);

#if SPI_USE_MUTUAL_EXCLUSION
  spiReleaseBus(spefcp->spip);
#endif

  /* Wait until EEPROM process data. */
  now = chTimeNow();
  while (ll_25xx_is_busy(spefcp->spip)) {
    if ((chTimeNow() - now) > spefcp->write_time) {
      return RDY_TIMEOUT;
    }

    chThdYield();
  }

  /* Lock array preventing unexpected access */
  ll_25xx_lock(spefcp->spip);
  return RDY_OK;
}
