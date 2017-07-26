# EEPROM files
EEPROMSRC = eeprom/eeprom.c \
            eeprom/base_rw.c \
            eeprom/drv/drv_tbl.c


EEPROMINC = $(STREAMSINC) \
			eeprom/

EEPROMDEVSRC = eeprom/drv/24xx.c \
               eeprom/drv/25xx.c
