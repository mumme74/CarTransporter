# for ubuntu
ifneq ("$(wildcard $($HOME/arduino))","")
ARDUINO_DIR="$(wildcard $($HOME/arduino))"
else ifneq ("$(wildcard $(/Applications/arduino))","")
ARDUINO_DIR="$(wildcard $(/Applications/arduino))"
endif

ARDMK_DIR=${HOME}/elektronik/Arduino-Makefile
USER_LIB_PATH := $(realpath ../../libraries)

BOARD_TAG    = teensy31
ARDUINO_LIBS = FlexCAN system_things ADC EEPROM
#F_CPU = 720000000

# user settings
CPPFLAGS := -I $(realpath ../system_headers/)

all:
	@echo "${USER_INCLUDES}"

include ${ARDMK_DIR}/Teensy.mk


