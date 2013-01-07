#ifndef __25XX_H__
#define __25XX_H__

#include "ch.h"
#include "hal.h"

#include "eeprom/eeprom.h"

/**
 * @brief   EEPROM read routine.
 *
 * @param[in]  spefcp   pointer to configuration structure of eeprom file.
 * @param[in]  offset   addres of 1-st byte to be read.
 * @param[out] data     pointer to buffer with data to be written.
 * @param[in]  len      number of bytes to be red.
 */
msg_t ll_eeprom_read(const SPIEepromFileConfig *spefcp, uint32_t offset,
                     uint8_t *data, size_t len);

/**
 * @brief   EEPROM write routine.
 * @details Function writes data to EEPROM.
 * @pre     Data must be fit to single EEPROM page.
 *
 * @param[in] spefcp  pointer to configuration structure of eeprom file.
 * @param[in] offset  addres of 1-st byte to be writen.
 * @param[in] data    pointer to buffer with data to be written.
 * @param[in] len     number of bytes to be written.
 */
msg_t ll_eeprom_write(const SPIEepromFileConfig *spefcp, uint32_t offset,
                      const uint8_t *data, size_t len);


#endif /* __25XX_H__ */
