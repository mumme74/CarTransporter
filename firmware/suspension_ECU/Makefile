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
F_CPU=96000000

#USER_DEFINES += -DDEBUG_BLINK_ON
USER_DEFINES += -DDEBUG_UART_ON
#USER_DEFINES += -DDEBUG_NTC_ON
#USER_DEFINES += -DDEBUG_PRESSURES_ON

# user settings
CPPFLAGS := -I $(realpath ../system_headers/) $(USER_DEFINES)

all:
	@echo "${USER_INCLUDES}"

include ${ARDMK_DIR}/Teensy.mk


