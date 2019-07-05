/**
 * cantypes.h, propose of this file is to provide custom canframe types
 * when socketcan driver not available
 *
 *  Created on: 4 jul 2019
 *      Author: jof
 */
#ifndef CANTYPES_H
#define CANTYPES_H

#include <stdint.h>

typedef uint32_t canid_t;

#define CAN_SFF_ID_BITS		11
#define CAN_EFF_ID_BITS		29

/*
 * Controller Area Network Error Message Frame Mask structure
 *
 * bit 0-28	: error class mask
 * bit 29-31	: set to zero
 */
typedef uint32_t can_err_mask_t;

/* CAN payload length and DLC definitions */
#define CAN_MAX_DLC 8
#define CAN_MAX_DLEN 8

/**
  * @breif canframe_t, can frame struct (NON FD)
  */
typedef struct {
    canid_t can_id;
    uint8_t can_dlc;
    uint8_t __padding;
    uint8_t __reserved0;
    uint8_t __reserved1;
    uint8_t data[CAN_MAX_DLEN] __attribute((aligned(8)));
} canframe_t;

/**
  * @breif can_filter_t, can filter struct
  */
typedef struct {
    canid_t can_id; // filter ID
    canid_t can_mask; // filter mask
} can_filter_t;


#endif // CANTYPES_H
