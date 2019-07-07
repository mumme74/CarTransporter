/*
 * types.h
 *
 *  Created on: 7 jul 2019
 *      Author: jof
 */#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>


#ifdef _WIN32
# if REG_DWORD == REG_DWORD_LITTLE_ENDIAN
#  define IS_LITTLE_ENDIAN
# endif
#else // _WIN32
# if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#  define IS_LITTLE_ENDIAN
# endif
#endif

typedef union {
    uint64_t vlu;
    uint8_t  arr[8];
    struct {
#ifdef IS_LITTLE_ENDIAN
      uint8_t b0, b1, b2, b3, b4, b5, b6, b7;
#else
      uint8_t b7, b6, b5, b4, b3, b2, b1, b0;
#endif
    };
} byte8_t;

typedef union {
    uint32_t vlu;
    uint8_t  arr[4];
    struct {
#ifdef IS_LITTLE_ENDIAN
      uint8_t b0, b1, b2, b3;
#else
      uint8_t b3, b2, b1, b0;
#endif
    };
} byte4_t;

typedef union {
    uint16_t vlu;
    uint8_t  arr[2];
    struct {
#ifdef IS_LITTLE_ENDIAN
      uint8_t b0, b1;
#else
      uint8_t b1, b0;
#endif
    };
} byte2_t;

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
