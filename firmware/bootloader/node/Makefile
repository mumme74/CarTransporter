# this file selects between arduino (teensy) based core 
# or opencm3 (stm32f73rb) based


CAR_TRANSPORTER_ROOT = ../../../

SRCS_common = commands.c \
	          crc32/crc32.c \
	          fifo.c \
	          can.c

BINARY = can_bootloader_$(CORE)

$(info using core $(CORE))
ifeq ($(strip $(CORE)),teensy3)
  include teensy31.mk
else ifeq ($(strip $(CORE)),stm32)
  include open_cm3.mk
else
  $(error Must select core: .. CORE=teensy3 .. or CORE=stm32)
endif

