
# for ubuntu
ifneq ($(wildcard $(HOME)/arduino),)
    ARDUINO_DIR=$(wildcard $(HOME)/arduino)
else ifneq ($(wildcard /Applications/arduino),)
    ARDUINO_DIR=$(wildcard /Applications/arduino)
endif

ARDMK_DIR=${HOME}/elektronik/Arduino-Makefile

BOARD_TAG    = teensy31
LINKER_SCRIPTS := -T$(realpath ../../../../../suspension_ECU/mk20dx256.ld) 
F_CPU=72000000
MONITOR_BAUDRATE = 115200


# name resulting bin correctly
TARGET=testapp-teensy

CPPFLAGS+=-g
CFLAGS+=-g
LDFLAGS+=-g

all:

	
upload: error_teensy_loader error_exit
	$(info *************************)
	 

error_teensy_loader:
	@echo "\n\n**********************************************************"
	@echo "Can't use teensy uploader with can_bootloader active"
	@echo "You should be using 'bootloader-host' in firmware/bootloader dir."
	@echo "  +++ USB loading clears can-bootloader +++"
	@echo "**********************************************************\n\n"
	
error_exit:
	$(error Abort upload through USB)
	
.PHONY: error_teensy_loader error_exit


include ${ARDMK_DIR}/Teensy.mk

