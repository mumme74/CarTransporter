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

#define EEPROM_DRV_USE_25XX TRUE

#define EEPROM_PAGE_SIZE        32          /* page size in bytes. Consult datasheet. */
#define EEPROM_SIZE             2048        /* total amount of memory in bytes */
#define EEPROM_SPID             SPID1       /* ChibiOS SPI driver used to communicate with EEPROM */
#define EEPROM_WRITE_TIME_MS    10          /* time to write one page in ms. Consult datasheet! */
#define EEPROM_DRIVER_NAME      25

#include "hal_eeprom.h"
#include "eeprom_setup.h"
#include "ch.h"
#include "hal.h"
#include "debug.h"


// max 2.1MHz when3.3V
// write cycle time 10ms at 3.3V
// instructions start with MSB first

// Instruction set EEPROM
/*#define WREN  0b00000110U  // write enable
#define WRDI  0b00000100U  // write disable
#define RDSR  0b00000101U  // read status register
#define WRSR  0b00000001U  // write status register
#define READ  0b00000011U  // read from memory array
#define WRITE 0b00000010U  // write to memory array*/

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
/*#define NRDY 0b00000001U
#define WEN  0b00000010U
#define BP0  0b00000100U
#define BP1  0b00001000U
#define WPEN 0b10000000U*/

// -----------------------------------------------------------------------------------
// begin global exported symbols

// fill settings with sensible default
uint16_t settings[S_EOF];

event_source_t ee_settingsChanged;

// -----------------------------------------------------------------------------------
// begin private function prototypes

void initSettings(void);

// ------------------------------------------------------------------------------------
// begin config section
/*
 * Maximum speed SPI configuration (2MHz, CPHA=0, CPOL=0, MSb first).
 * CPOL=0 (SCK) low at idle, CPHA=0 (Reads bit at clock rising edge)
 */
static const SPIConfig spiConfig = {
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
    // we might have to set 8bit rx interrupt trigger, 16bit is default.. FRXTH
    SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0 // signifies 8bit transfers
};

SPIEepromFileConfig stateConfig = {
    0, // start adress
    3, // 4 bytes for state flags
    EEPROM_SIZE,
    EEPROM_PAGE_SIZE,
    MS2ST(EEPROM_WRITE_TIME_MS),
    &EEPROM_SPID,
    &spiConfig
};
static SPIEepromFileStream stateFileStream;
EepromFileStream *stateFile = NULL;

SPIEepromFileConfig settingsConfig = {
    4, // start address
    104, // end address gives us maximum of 50 settings
    EEPROM_SIZE,
    EEPROM_PAGE_SIZE,
    MS2ST(EEPROM_WRITE_TIME_MS),
    &EEPROM_SPID,
    &spiConfig
};
static SPIEepromFileStream settingsFileStream;
static EepromFileStream *settingsFile = NULL;

SPIEepromFileConfig dtcFileConfig = {
   105, // start address
   1024, // end address
   EEPROM_SIZE,
   EEPROM_PAGE_SIZE,
   MS2ST(EEPROM_WRITE_TIME_MS),
   &EEPROM_SPID,
   &spiConfig
};

static SPIEepromFileStream dtcFileStream;
EepromFileStream *dtcFile = NULL;

// ----------------------------------------------------------------------------------
// begin threads

// ----------------------------------------------------------------------------------
// begin private functions for this module
void initSettings(void)
{
    // a event for settings
    chEvtObjectInit(&ee_settingsChanged);

    // fill settings with sensible defaults
    // release
    settings[S_TimeRevupRelease]           = 150; // ms
    settings[S_TimeContinueAfterFreed]     = 300; // ms
    settings[S_CurrentFreeThreshold]       = 5000; //mA
    settings[S_CurrentRevupMaxRelease]     = 20000; // mA
    settings[S_CurrentMaxRelease]          = 15000; // mA
    // tighten
    settings[S_TimeRevupTighten]           = 50; // ms
    settings[S_TimeContinueAfterTightened] = 100; // ms
    settings[S_CurrentTightenedThreshold]  = 15000; // mA
    settings[S_CurrentMaxTighten]          = 18000; // mA
    settings[S_CurrentRevupMaxTighten]     = 19000; // mA

    // user settings
    settings[S_AntilockParkbrake]          = 1; // true

    // vehicle settings
    settings[S_RimDiameter]                = 17; // in inches
    settings[S_TireThread]                 = 225; // in mm
    settings[S_TireProfile]                = 65; // in percent

    // user settings

    // hardware error prevents this functionality
    // settings[S_AutoTighten]                = 1;

    // fill from eeprom
    msg_t i;
    for (i = 0; i < S_EOF; ++i) {
        msg_t pos = i * sizeof(settings[0]);
        if (fileStreamSeek(settingsFile, pos) == pos) {
            uint16_t vlu = EepromReadHalfword(settingsFile);
            if (vlu < 0xFFFF)
              settings[i] = vlu;
        }
    }
}



// -----------------------------------------------------------------------------------
// begin API functions

void ee_initEeprom(void)
{
    // start SPI
    spiStart(&SPID1, &spiConfig);

    // init EEprom files
    dtcFile = SPIEepromFileOpen(&dtcFileStream, &dtcFileConfig, EepromFindDevice(EEPROM_DRIVER_NAME));
    stateFile = SPIEepromFileOpen(&stateFileStream, &stateConfig, EepromFindDevice(EEPROM_DRIVER_NAME));
    settingsFile = SPIEepromFileOpen(&settingsFileStream, &settingsConfig, EepromFindDevice(EEPROM_DRIVER_NAME));

    // settings
    initSettings();
}

int ee_saveSetting(settings_e settingsIdx)
{
    msg_t pos = settingsIdx * sizeof(settings[0]);
    if (fileStreamSeek(settingsFile, pos) == pos) {
        if (EepromWriteHalfword(settingsFile, settings[settingsIdx]) == 2) {
            chEvtBroadcastFlags(&ee_settingsChanged, settingsIdx);
            return 1;
        }
    }
    return 0;
}

