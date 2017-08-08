/*
 * park_settings.h
 *
 *  Created on: 8 aug 2017
 *      Author: fredrikjohansson
 */

#ifndef PARK_SETTINGS_H_
#define PARK_SETTINGS_H_

/**
 * @description settings struct that gets populated from EEPROM
 * is changeable via CAN
 */
typedef enum {
    // ----------------------------------------------------------------
    // start Release settings

    // time under which a higher current is allowed is milliseconds
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

    // ---------------------------------------------------------------
    // user settings
    //S_AutoTighten = 10, Hardware error prevents it from working
    S_AntilockParkbrake = 11,

    // vehicle settings
    // how many pulses ABS sensor generates at one wheel revolution
    S_PulsesPerRevolution = 20,

    // rim diameter in inches (like on a tire 225 / 17 R 65) this would be 17
    S_RimDiameter = 21,

    // Tire width in mm (like on a tire 225 / 17 R 65) this would be 225
    S_TireThread = 22,

    // Tire Profile in percent (like on a tire 225 / 17 R 65) this would be 65
    S_TireProfile = 23,

    // maximum 50 settings!

    // END marker
    S_EOF = 50
} settings_e;



#endif /* PARK_SETTINGS_H_ */
