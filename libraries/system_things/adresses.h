/*
 * adresses.h
 *
 *  Created on: 1 apr 2015
 *      Author: jof
 */

#ifndef ADRESSES_H_
#define ADRESSES_H_

/**
 * This file acts as a common database for inputs-outputs adresses systemwide
 * adresses are a system wide unique ID to each thing
 */
enum struct Adresses : uint16_t {
    // suspension ecu outputs
    leftFill = 1,
    leftDump = 2,
    leftSuck = 3,
    rightFill = 4,
    rightDump = 5,
    rightSuck = 6,
    airdryer = 7,
    spare1 = 8,
    compressor = 9,

    // suspension ecu inputs
    airPressure = 20,
    leftPressure = 21,
    leftHeight = 22,
    rightPressure = 23,
    rightHeight = 24,
    compressorTemp = 25,
    systemPressure = 26,
    spareAnalog1 = 27,
    spareTemp1 = 28,

    // suspension ecu system pressure regulator
    airFeed = 40,

    // displaynode things starts at 200
    upButton = 200,
};


#endif /* ADRESSES_H_ */
