#ifndef __EEPROM_CONFIG_H__
#define __EEPROM_CONFIG_H__

/* Configuration for test suit. */

#define USE_EEPROM_TEST_SUIT    1           /* build test suit for EEPROM abstraction layer */
#define EEPROM_PAGE_SIZE        32          /* page size in bytes. Consult datasheet. */
#define EEPROM_SIZE             4096        /* total amount of memory in bytes */
#define EEPROM_SPID             SPID1       /* ChibiOS SPI driver used to communicate with EEPROM */
#define EEPROM_SPIDCONFIG       SPI1Config  /* Config of SPI device driver */
#define EEPROM_WRITE_TIME_MS    10          /* time to write one page in ms. Consult datasheet! */
#define EEPROM_DRIVER_NAME      "25xx"

#endif /* __EEPROM_CONFIG_H__ */
