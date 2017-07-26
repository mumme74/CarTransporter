include $(CHIBIOS)/os/hal/lib/streams/streams.mk # for chprinf
include eeprom/eeprom.mk

EEPROM_TESTSUITSRC = $(EEPROMSRC) \
					 eeprom/eeprom_testsuit.c

EEPROM_TESTSUITINC = $(EEPROMINC)