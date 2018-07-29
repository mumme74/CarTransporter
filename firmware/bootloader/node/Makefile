##
## This file is part of the libopencm3 project.
##
## Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
## Copyright (C) 2010 Piotr Esden-Tempski <piotr@esden.net>
##
## This library is free software: you can redistribute it and/or modify
## it under the terms of the GNU Lesser General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public License
## along with this library.  If not, see <http://www.gnu.org/licenses/>.
##


BINARY = can_bootloader

#DEVICE = stm32f373rB
LDSCRIPT = stm32f373xRB.ld

NODE_ID = 0 #parkbrakeNode
#NODE_NAME = 1 # suspensionNode

CAR_TRANSPORTER_ROOT = ../../../


SRCS = can_stm32f373.c \
	   system_stm32f373.c \
	   commands.c \
	   crc32/crc32.c \
	   main.c


CSTD = -std=gnu11
DEFS += -DNODE_ID=$(NODE_ID)
DEFS += -Icrc32/ -I$(CAR_TRANSPORTER_ROOT)/firmware/system_headers/

ifeq ($(DEVICE),)
ARCH_FLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
DEFS += -DSTM32F3 -DSTM32F373RB
LIBNAME += opencm3_stm32f3
LDLIBS += -lc -lgcc -lnosys
endif


OPENCM3_DIR = /Users/jof/Source/libopencm3


include rules.mk
