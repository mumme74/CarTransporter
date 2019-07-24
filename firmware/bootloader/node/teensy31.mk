# for ubuntu
ifneq ($(wildcard $(HOME)/arduino),)
ARDUINO_DIR=$(wildcard $(HOME)/arduino)
else ifneq ($(wildcard /Applications/arduino),)
ARDUINO_DIR=$(wildcard /Applications/arduino)
endif

ARDMK_DIR=${HOME}/elektronik/Arduino-Makefile
#USER_LIB_PATH := $(realpath ../../system_common)

BOARD_TAG    = teensy31
LINKER_SCRIPTS := -T$(realpath ./mk20dx256.ld) 
ARDUINO_LIBS = #FlexCAN
F_CPU=96000000
MONITOR_BAUDRATE = 115200


ifndef NODE_ID
	#NODE_ID := 0     # parkbrakeNode 
	NODE_ID := 1     # suspensionNode
endif

USER_DEFINES += -DNODE_ID=$(NODE_ID)

ifdef DEBUG_PRINT
	USER_DEFINES += -DDEBUG_PRINT
endif

# name resulting bin correctly
TARGET=$(BINARY)



# C files for this project
LOCAL_C_SRCS = $(SRCS_common) \
				system_teensy3.c \
				can_teensy3.c \
				main.c 
				
LOCAL_CPP_SRCS = 


# user settings
CFLAGS := -I $(realpath ./) \
          -I $(realpath ../../system_common/) \
          -I $(realpath crc32/) \
          -g \
          $(USER_DEFINES)
          
CPPFLAGS := $(CFLAGS)
LDFLAGS := -g
 
all:
	@echo "${USER_INCLUDES}"

include ${ARDMK_DIR}/Teensy.mk
