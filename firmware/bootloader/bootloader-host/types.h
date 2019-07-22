/*
 * types.h
 *
 *  Created on: 7 jul 2019
 *      Author: jof
 */#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>


/**
 * @brief CAN_Speeds_t, these are the available can speeds
 */
typedef enum {
    _CAN_speed_start_marker,
    CAN_speed_socketspeed, // only available in socketcan
                           // when interface is already
                           // set up with correct speed
    CAN_speed_10Kbit,
    CAN_speed_20Kbit,
    CAN_speed_50Kbit,
    CAN_speed_100Kbit,
    CAN_speed_125Kbit,
    CAN_speed_250Kbit,
    CAN_speed_500Kbit,
    CAN_speed_800Kbit,
    CAN_speed_1Mbit,
    // any added speeds must be accompanied with addition
    // of corresponding string name in canbridge.c

    // used as end iterator marker, must be last in enum
    _CAN_speed_end_marker
} CAN_Speeds_t;

#endif // TYPES_H
