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


/*
 * Controller Area Network Identifier structure
 *
 * bit 0-28	: CAN identifier (11/29 bit)
 * bit 29	: error message frame flag (0 = data frame, 1 = error message)
 * bit 30	: remote transmission request flag (1 = rtr frame)
 * bit 31	: frame format flag (0 = standard 11 bit, 1 = extended 29 bit)
 */
typedef uint32_t canid_t;

#define CAN_SFF_ID_BITS		11
#define CAN_EFF_ID_BITS		29


/* special address description flags for the CAN_ID */
#define CAN_EFF_FLAG 0x80000000U /* EFF/SFF is set in the MSB */
#define CAN_RTR_FLAG 0x40000000U /* remote transmission request */
#define CAN_ERR_FLAG 0x20000000U /* error message frame */

/* valid bits in CAN ID for frame formats */
#define CAN_SFF_MASK 0x000007FFU /* standard frame format (SFF) */
#define CAN_EFF_MASK 0x1FFFFFFFU /* extended frame format (EFF) */
#define CAN_ERR_MASK 0x1FFFFFFFU /* omit EFF, RTR, ERR flags */

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
    uint8_t data[CAN_MAX_DLEN]; // __attribute((aligned(8)));
} canframe_t;

/**
  * @breif can_filter_t, can filter struct
  */
typedef struct {
    canid_t can_id; // filter ID
    canid_t can_mask; // filter mask
} can_filter_t;


#endif // CANTYPES_H
