#ifndef CAN_TYPES_H
#define CAN_TYPES_H

#include <stdint.h>
#include <stdbool.h>



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



#endif // CAN_TYPES_H
