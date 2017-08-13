# for ubuntu
ARDUINO_DIR=${HOME}/arduino-1.8.3
ARDMK_DIR=${HOME}/elektronik/Arduino-Makefile
USER_LIB_PATH := $(realpath ../../libraries)

BOARD_TAG    = teensy31
ARDUINO_LIBS = FlexCAN system_things ADC EEPROM
#F_CPU = 720000000

include ${ARDMK_DIR}/Teensy.mk


# user settings
USER_INCLUDES += -I $(realpath ../system_headers/)
