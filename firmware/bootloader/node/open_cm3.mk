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



ifndef NODE_ID
	NODE_ID := 0     # parkbrakeNode 
	#NODE_ID := 1     # suspensionNode
endif


CSTD = -std=gnu11
DEFS += -DNODE_ID=$(NODE_ID)
DEFS += -Icrc32/ -I$(CAR_TRANSPORTER_ROOT)/firmware/system_common/


$(info NODE_ID=$(NODE_ID))


ifeq ($(STM32F103),)
# parkbrakeNode, stm32f373
  ARCH_FLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 
  LDSCRIPT = stm32f373xRB.ld
  DEFS += -DSTM32F3 -DSTM32F373RB
  LIBNAME += opencm3_stm32f3
else
  ARCH_FLAGS = -mcpu=cortex-m3
  LDSCRIPT = stm32f103x8t6.ld
  DEFS += -DSTM32F1 -DSTM32F103RB
  LIBNAME += opencm3_stm32f1
endif

ARCH_FLAGS += -O0 #-Os
LDLIBS += -lc -lgcc -lnosys
SRCS = can_stm32f373.c \
       system_stm32f373.c \
	   main.c \
       $(SRCS_common) 
#DEVICE = stm32f373rB


# for ubuntu
ifneq ($(wildcard $(HOME)/elektronik/libopencm3),)
 OPENCM3_DIR=$(wildcard $(HOME)/elektronik/libopencm3)
else ifneq ($(wildcard $(HOME)/Source/libopencm3),)
 OPENCM3_DIR=$(wildcard $(HOME)/Source/libopencm3)
endif

include rules.mk
