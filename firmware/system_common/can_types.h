#ifndef CAN_TYPES_H
#define CAN_TYPES_H

#include <stdint.h>
#include <stdbool.h>

/*  NOTE !! this file should be included after definition header for can frame type */

#if defined(STM32F3) || (defined(__MK20DX256__) && defined(NODE_ID))
// bootloader or parkbrake node, use can_frame_t
# define CAN_FRM_TYPE can_frame_t
# define CAN_FRM_DATA data8
# define CAN_FRM_DLC  DLC

#elif defined(__MK20DX256__)
// suspension node use FlexCAN frame type
# define CAN_FRM_TYPE Frame_t
# define CAN_FRM_DATA buf
# define CAN_FRM_DLC  len

#else
// displaynode use linux canframe_t
// make sure to include after linux canframe header
# define CAN_FRM_TYPE canframe_t
# define CAN_FRM_DATA data
# define CAN_FRM_DLC  can_dlc

#endif


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
    uint8_t *ptr8;
    uint16_t *ptr16;
    uint32_t *ptr32;
    uint64_t *ptr64;
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
    uint8_t *ptr8;
    uint16_t *ptr16;
    uint32_t *ptr32;
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
    uint16_t *ptr16;
    struct {
#ifdef IS_LITTLE_ENDIAN
      uint8_t b0, b1;
#else
      uint8_t b1, b0;
#endif
    };
} byte2_t;

typedef struct {
  union {
    uint8_t  data8[8];
    uint16_t data16[4];
    uint32_t data32[2];
    uint64_t data64;
  };
  union {
    uint16_t IDE;
    uint32_t EID;
  };
  uint8_t DLC;
  bool ext;
  bool rtr;
} can_frame_t;


typedef union {
  uint32_t v32;
  uint16_t v16[2];
  uint8_t  v8[4];
} u32to8_t;

/**
 * @brief fills frm with values from byt in correct byte order
 * @param[in:out] frm, the can frame to take the value in byt
 *                  frm.DLC should be set to first insert point
 *                  frm.DLC gets updated to last insert pos +1
 * @returns number of bytes filled
 */
extern uint8_t fillbytes(CAN_FRM_TYPE *frm, uint8_t nrBytes,
                         const byte8_t *byt);
#define fill2bytes(frm, byt) fillbytes(frm, 2, (byte8_t*)byt)
#define fill3bytes(frm, byt) fillbytes(frm, 3, (byte8_t*)byt)
#define fill4bytes(frm, byt) fillbytes(frm, 4, (byte8_t*)byt)
#define fill5bytes(frm, byt) fillbytes(frm, 5, byt)
#define fill6bytes(frm, byt) fillbytes(frm, 6, byt)
#define fill7bytes(frm, byt) fillbytes(frm, 7, byt)
#define fill8bytes(frm, byt) fillbytes(frm, 8, byt)

/*
extern uint8_t fill2bytes(CAN_FRM_TYPE *frm, const byte2_t *byt);
extern uint8_t fill3bytes(CAN_FRM_TYPE *frm, const byte4_t *byt);
extern uint8_t fill4bytes(CAN_FRM_TYPE *frm, const byte4_t *byt);

extern uint8_t fill5bytes(CAN_FRM_TYPE *frm, const byte8_t *byt);
extern uint8_t fill6bytes(CAN_FRM_TYPE *frm, const byte8_t *byt);
extern uint8_t fill7bytes(CAN_FRM_TYPE *frm, const byte8_t *byt);
extern uint8_t fill8bytes(CAN_FRM_TYPE *frm, const byte8_t *byt);

*/

/**
 * @brief reads from frame into byt with correct byte order
 * @param[in] frm, the can frame to read from
 * @param[in] startpos, start from this pos
 * @param[out] byt, the value filled
 * @returns number of bytes read
 */
uint8_t readbytes(CAN_FRM_TYPE *frm, uint8_t nrBytes,
                  uint8_t startPos, byte8_t *byt);
#define read2bytes(frm, startPos, byt) readbytes(frm, 2, startPos, (byte8_t*)byt)
#define read3bytes(frm, startPos, byt) readbytes(frm, 3, startPos, (byte8_t*)byt)
#define read4bytes(frm, startPos, byt) readbytes(frm, 4, startPos, (byte8_t*)byt)
#define read5bytes(frm, startPos, byt) readbytes(frm, 5, startPos, byt)
#define read6bytes(frm, startPos, byt) readbytes(frm, 6, startPos, byt)
#define read7bytes(frm, startPos, byt) readbytes(frm, 7, startPos, byt)
#define read8bytes(frm, startPos, byt) readbytes(frm, 8, startPos, byt)

/*
extern uint8_t read2bytes(CAN_FRM_TYPE *frm, uint8_t startPos, byte2_t *byt);
extern uint8_t read3bytes(CAN_FRM_TYPE *frm, uint8_t startPos, byte4_t *byt);
extern uint8_t read4bytes(CAN_FRM_TYPE *frm, uint8_t startPos, byte4_t *byt);
extern uint8_t read5bytes(CAN_FRM_TYPE *frm, uint8_t startPos, byte8_t *byt);
extern uint8_t read6bytes(CAN_FRM_TYPE *frm, uint8_t startPos, byte8_t *byt);
extern uint8_t read7bytes(CAN_FRM_TYPE *frm, uint8_t startPos, byte8_t *byt);
extern uint8_t read8bytes(CAN_FRM_TYPE *frm, uint8_t startPos, byte8_t *byt);
*/
#endif // CAN_TYPES_H
