#ifndef __EEPROM_CONFIG_H__
#define __EEPROM_CONFIG_H__


#define USE_EEPROM_TEST_SUIT    1           /* build test suit for EEPROM abstraction layer */

#define EEPROM_PAGE_SIZE        32          /* page size in bytes. Consult datasheet. */
#define EEPROM_SIZE             4096        /* total amount of memory in bytes */
#define EEPROM_SPID             SPID1       /* ChibiOS SPI driver used to communicate with EEPROM */
#define EEPROM_WRITE_TIME_MS    10          /* time to write one page in ms. Consult datasheet! */


#endif /* __EEPROM_CONFIG_H__ */
