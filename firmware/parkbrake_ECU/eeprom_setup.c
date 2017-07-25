/*
 * eeprom.c
 *
 * CarTransporter
 * Copyright (C) 2017  Fredrik Johansson mumme74@github.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "eeprom.h"
#include "ch.h"
#include "hal.h"


// max 2.1MHz when3.3V
// write cycle time 10ms at 3.3V
// instructions start with MSB first

// Instruction set EEPROM
#define WREN  0b00000110U  // write enable
#define WRDI  0b00000100U  // write disable
#define RDSR  0b00000101U  // read status register
#define WRSR  0b00000001U  // write status register
#define READ  0b00000011U  // read from memory array
#define WRITE 0b00000010U  // write to memory array

/**
 * Status register, ALL bits are 1 when writing!
 * bit0 = NRDY, 0 when device is ready, 1 when busy writing
 * bit1 = WEN, 1 indicates write enabled (hardware WP pin might affect this)
 * bit2 = BP0 see below
 * bit3 = BP1 see below
 * bit4 = 0 when not writing
 * bit5 = 0 when not writing
 * bit6 = 0 when not writing
 * bit7 = WPEN, 1 when Write protection is enabled
 *
 * BP0 | BP1 |  memory protection
 *  0  |  0  |   *no write protection*
 *  1  |  0  |   first 1/4 write protected
 *  0  |  1  |   half write protected
 *  1  |  1  |   all memory write protected
 */

// masks for Status register
#define NRDY 0b00000001U
#define WEN  0b00000010U
#define BP0  0b00000100U
#define BP1  0b00001000U
#define WPEN 0b10000000U

// -----------------------------------------------------------------------------------
// begin private function prototypes

// ------------------------------------------------------------------------------------
// begin config section
/*
 * Maximum speed SPI configuration (2MHz, CPHA=0, CPOL=0, MSb first).
 * CPOL=0 (SCK) low at idle, CPHA=0 (Reads bit at clock rising edge)
 */
static const SPIConfig spiCfg = {
    // SPI complete callback
    NULL,
    // -------------------------------------
    // device specific from here on
    // chip select line port
    GPIOC,
    // ChipSelect linepad number
    6,
    // CR1 register
    SPI_CR1_BR_2 | SPI_CR1_MSTR, // CPOL=0 && CPHA=0
       // Baud rate and master is the interesting thing here, peripheral clock running at 36Mhz
       // we need divide by 16 get us 2.25MHz which is to much in voltage between 2-7-5volts
       // so better safe than sorry, we divide by 32 = 1MHz (BR[2:0]=100)

    // CR2 register
    // we might have to set 8bit rx interupt trigger, 16bit is default.. FRXTH
    SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0 // signifies 8bit transfers
};








// -----------------------------------------------------------------------------------
// begin API functions

void ee_init()
{
    // init SPI
}
