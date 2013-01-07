#ifndef __25XX_H__
#define __25XX_H__

#include "ch.h"
#include "hal.h"

#include "eeprom/eeprom.h"


msg_t ll_eeprom_read(const SPIEepromFileConfig *spefcp, uint32_t offset,
                     uint8_t *data, size_t len);

msg_t ll_eeprom_write(const SPIEepromFileConfig *spefcp, uint32_t offset,
                      const uint8_t *data, size_t len);


#endif /* __25XX_H__ */
