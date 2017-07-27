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

/**
 * Driver init for Atmel SPI EEProm  AT25080 -> AT25640
 * CarTransporter Parkbrake uses AT25160 (2048bytes)
 */

/**
 * @description settings struct that gets populated from EEPROM
 * is changeable via CAN
 */
typedef enum {
    // time under which a higher current is allowed is milliseconds
    // ----------------------------------------------------------------
    // start Release settings
    // see S_CurrentHigherReleaseCurrent
    S_TimeRevupRelease = 0,

    // the time the motor is allowed to run after current has passed currentFreeThreshold
    S_TimeContinueAfterFreed = 1,

    // below this current brake pad is considered free from disc
    // in milliamps
    S_CurrentFreeThreshold = 2,

    // the maximum current allowed before current limit steps in
    // on release action
    // NOTE see about currentHigherMaxRelease
    S_CurrentMaxRelease = 3,

    // The current that is allowed during torque up release
    // see S_TimeRevupReleaseCurrent
    S_CurrentRevupMaxRelease = 4,

    // ----------------------------------------------------------------
    // start tighten settings

    // During this time under revup a higher current is allowed
    // see S_CurrentHigherMaxTighten
    S_TimeRevupTighten = 5,

    // time after current has reached tightened threshold which motor is holding before current off
    S_TimeContinueAfterTightened = 6,

    // above this current brake is considered tightened
    S_CurrentTightenedThreshold = 7,

    // During revup time a higher current is allowed
    // see S_TimeHigherTightenCurrent
    S_CurrentRevupMaxTighten = 8,

    // The maximum allowed current before current limit steps in
    S_CurrentMaxTighten = 9,

    // maximum 50 settings!

    // END marker
    S_EOF
} settings_e;



extern EepromFileStream *stateFile;
extern EepromFileStream *dtcFile;

extern uint16_t settings[S_EOF];

// initializes this module
void ee_initEeprom(void);
// save current setting for idx in eeprom
void ee_saveSetting(settings_e settingIdx);


#endif /* EEPROM_SETUP_H_ */
