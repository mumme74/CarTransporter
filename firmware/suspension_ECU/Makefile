# for ubuntu
ifneq ($(wildcard $(HOME)/arduino),)
  ARDUINO_DIR=$(wildcard $(HOME)/arduino)
else ifneq ($(wildcard /Applications/arduino),)
  ARDUINO_DIR=$(wildcard /Applications/arduino)
endif

ARDMK_DIR=$(realpath  ../../../Arduino-Makefile)
USER_LIB_PATH := $(realpath ../../libraries)

BOARD_TAG    = teensy31
ARDUINO_LIBS = FlexCAN system_things ADC EEPROM
F_CPU=96000000

ifeq ($(TEENSY_BOOTLOADER),)
	#$(error Cant't use teensy uploader with can_bootloader)
  LINKER_SCRIPTS := -T$(realpath ./mk20dx256.ld)
  BOOTLOADER_PARENT=can_bootloader

endif

#USER_DEFINES += -DDEBUG_BLINK_ON
USER_DEFINES += -DDEBUG_UART_ON
#USER_DEFINES += -DDEBUG_NTC_ON
#USER_DEFINES += -DDEBUG_PRESSURES_ON
#USER_DEFINES += -DDEBUG_QUIET_PID_UPDATES // can be noisy
USER_DEFINES += -DDEBUG_CAN_SERIAL

# user settings
CPPFLAGS := -I $(realpath ../system_common/) $(USER_DEFINES)

all:
	@echo "${USER_INCLUDES}"
	
upload: error_teensy_loader error_exit
	$(info *************************)
	 

error_teensy_loader:
	@echo "\n\n**********************************************************"
	@echo "Can't use teensy uploader with can_bootloader active"
	@echo "You should be using 'bootloader-host' in firmware/bootloader dir."
	@echo "if you must you can fordce with: (TEENSY_BOOTLOADER=1)"
	@echo "  +++ WARNING!! clears can-bootloader +++"
	@echo "**********************************************************\n\n"
	
error_exit:
	$(error Abort)
	
.PHONY: error_teensy_loader error_exit
	
include ${ARDMK_DIR}/Teensy.mk



