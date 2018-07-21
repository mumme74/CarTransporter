/*
 * eeprom.h
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

#ifndef EEPROM_SETUP_H_
#define EEPROM_SETUP_H_

#include "ch.h"
#include "park_settings.h"

/**
 * Driver init for Atmel SPI EEProm  AT25080 -> AT25640
 * CarTransporter Parkbrake uses AT25160 (2048bytes)
 */


#define ee_changeSetting(idx,vlu) { \
    uint16_t *s = (uint16_t*)&settings[idx]; \
    *s = vlu;   \
}


extern EepromFileStream *stateFile;
extern EepromFileStream *dtcFile;

// all settings, should be considered readonly except for this module
extern uint16_t settings[S_EOF];

extern event_source_t ee_settingsChanged;


// initializes this module
void ee_initEeprom(void);

// save current setting for idx in eeprom
int ee_saveSetting(settings_e settingIdx);


#endif /* EEPROM_SETUP_H_ */
