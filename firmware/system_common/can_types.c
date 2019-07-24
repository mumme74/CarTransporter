/*
 * can_types.c
 *
 *  Created on: 23 jul 2019
 *      Author: jof
 */


#include "can_types.h"


/**
 * @brief fills frm with values from byt in correct byte order
 * @param[in:out] frm, the can frame to take the value in byt
 *                  frm.DLC should be set to first insert point
 *                  frm.DLC gets updated to last insert pos +1
 * @returns number of bytes read or -1 if failure
 */
uint8_t fillbytes(CAN_FRM_TYPE *frm, uint8_t nrBytes, const byte8_t *byt) {
  uint8_t i = 0;
  for (; i < nrBytes; ++i) {
    if (frm->CAN_FRM_DLC > 7)
      return i;
#ifdef IS_LITTLE_ENDIAN
    // we want to pack canframe big endian
    frm->CAN_FRM_DATA[frm->CAN_FRM_DLC++] = byt->arr[nrBytes - i -1];
#else
    frm->CAN_FRM_DATA[frm->CAN_FRM_DLC++] = byt->arr[i];
#endif
  }
  return i;
}

/**
 * @brief reads from frame into byt with correct byte order
 * @param[in] frm, the can frame to read from
 * @param[in] startpos, start from this pos
 * @param[out] byt, the value filled
 * @returns number of bytes read
 */
uint8_t readbytes(CAN_FRM_TYPE *frm, uint8_t nrBytes,
                  uint8_t startPos, byte8_t *byt) {
  uint8_t i = 0;
  for (; i < 8 - startPos; ++i) {
    if (i >= nrBytes || i + startPos > 7)
      return i;
#ifdef IS_LITTLE_ENDIAN
    // can data should always be packed big endian
    byt->arr[i] = frm->CAN_FRM_DATA[startPos + (nrBytes - i)];
#else
    byt->arr[i] = frm->CAN_FRM_DATA[startPos + i];
#endif
  }
  return i;
}

/*
 uint8_t read2bytes(CAN_FRM_TYPE *frm, uint8_t startPos, byte2_t *byt) {
  if (startPos - frm->CAN_FRM_DLC <= 0) return 0;
  byt->b1 = frm->CAN_FRM_DATA[startPos++];
  if (startPos - frm->CAN_FRM_DLC <= 0) return 1;
  byt->b0 = frm->CAN_FRM_DATA[startPos++];
  return 2;
}
 uint8_t read3bytes(CAN_FRM_TYPE *frm, uint8_t startPos, byte4_t *byt) {
  if (startPos - frm->CAN_FRM_DLC <= 0) return 0;
  byt->b2 = frm->CAN_FRM_DATA[startPos++];
  if (startPos - frm->CAN_FRM_DLC <= 0) return 1;
  byt->b1 = frm->CAN_FRM_DATA[startPos++];
  if (startPos - frm->CAN_FRM_DLC <= 0) return 2;
  byt->b0 = frm->CAN_FRM_DATA[startPos++];
  return 3;
}
 uint8_t read4bytes(CAN_FRM_TYPE *frm, uint8_t startPos, byte4_t *byt) {
  if (startPos - frm->CAN_FRM_DLC <= 0) return 0;
  byt->b3 = frm->CAN_FRM_DATA[startPos++];
  if (startPos - frm->CAN_FRM_DLC <= 0) return 1;
  byt->b2 = frm->CAN_FRM_DATA[startPos++];
  if (startPos - frm->CAN_FRM_DLC <= 0) return 2;
  byt->b1 = frm->CAN_FRM_DATA[startPos++];
  if (startPos - frm->CAN_FRM_DLC <= 0) return 3;
  byt->b0 = frm->CAN_FRM_DATA[startPos++];
  return 4;
}
 uint8_t read5bytes(CAN_FRM_TYPE *frm, uint8_t startPos, byte8_t *byt) {
  if (startPos - frm->CAN_FRM_DLC <= 0) return 0;
  byt->b4 = frm->CAN_FRM_DATA[startPos++];
  if (startPos - frm->CAN_FRM_DLC <= 0) return 1;
  byt->b3 = frm->CAN_FRM_DATA[startPos++];
  if (startPos - frm->CAN_FRM_DLC <= 0) return 2;
  byt->b2 = frm->CAN_FRM_DATA[startPos++];
  if (startPos - frm->CAN_FRM_DLC <= 0) return 3;
  byt->b1 = frm->CAN_FRM_DATA[startPos++];
  if (startPos - frm->CAN_FRM_DLC <= 0) return 4;
  byt->b0 = frm->CAN_FRM_DATA[startPos++];
  return 5;
}
uint8_t read6bytes(CAN_FRM_TYPE *frm, uint8_t startPos, byte8_t *byt) {
  if (startPos - frm->CAN_FRM_DLC <= 0) return 0;
  byt->b5 = frm->CAN_FRM_DATA[startPos++];
  if (startPos - frm->CAN_FRM_DLC <= 0) return 1;
  byt->b4 = frm->CAN_FRM_DATA[startPos++];
  if (startPos - frm->CAN_FRM_DLC <= 0) return 2;
  byt->b3 = frm->CAN_FRM_DATA[startPos++];
  if (startPos - frm->CAN_FRM_DLC <= 0) return 3;
  byt->b2 = frm->CAN_FRM_DATA[startPos++];
  if (startPos - frm->CAN_FRM_DLC <= 0) return 4;
  byt->b1 = frm->CAN_FRM_DATA[startPos++];
  if (startPos - frm->CAN_FRM_DLC <= 0) return 5;
  byt->b0 = frm->CAN_FRM_DATA[startPos++];
  return 6;
}
uint8_t read7bytes(CAN_FRM_TYPE *frm, uint8_t startPos, byte8_t *byt) {
  if (startPos - frm->CAN_FRM_DLC <= 0) return 0;
  byt->b6 = frm->CAN_FRM_DATA[startPos++];
  if (startPos - frm->CAN_FRM_DLC <= 0) return 1;
  byt->b5 = frm->CAN_FRM_DATA[startPos++];
  if (startPos - frm->CAN_FRM_DLC <= 0) return 2;
  byt->b4 = frm->CAN_FRM_DATA[startPos++];
  if (startPos - frm->CAN_FRM_DLC <= 0) return 3;
  byt->b3 = frm->CAN_FRM_DATA[startPos++];
  if (startPos - frm->CAN_FRM_DLC <= 0) return 4;
  byt->b2 = frm->CAN_FRM_DATA[startPos++];
  if (startPos - frm->CAN_FRM_DLC <= 0) return 5;
  byt->b1 = frm->CAN_FRM_DATA[startPos++];
  if (startPos - frm->CAN_FRM_DLC <= 0) return 6;
  byt->b0 = frm->CAN_FRM_DATA[startPos++];
  return 7;
}
uint8_t read8bytes(CAN_FRM_TYPE *frm, uint8_t startPos, byte8_t *byt) {
  // we can't use loop here as that might screw up on different
  // Little/big endian machines
  if (startPos - frm->CAN_FRM_DLC <= 0) return 0;
  byt->b7 = frm->CAN_FRM_DATA[startPos++];
  if (startPos - frm->CAN_FRM_DLC <= 0) return 1;
  byt->b6 = frm->CAN_FRM_DATA[startPos++];
  if (startPos - frm->CAN_FRM_DLC <= 0) return 2;
  byt->b5 = frm->CAN_FRM_DATA[startPos++];
  if (startPos - frm->CAN_FRM_DLC <= 0) return 3;
  byt->b4 = frm->CAN_FRM_DATA[startPos++];
  if (startPos - frm->CAN_FRM_DLC <= 0) return 4;
  byt->b3 = frm->CAN_FRM_DATA[startPos++];
  if (startPos - frm->CAN_FRM_DLC <= 0) return 5;
  byt->b2 = frm->CAN_FRM_DATA[startPos++];
  if (startPos - frm->CAN_FRM_DLC <= 0) return 6;
  byt->b1 = frm->CAN_FRM_DATA[startPos++];
  if (startPos - frm->CAN_FRM_DLC <= 0) return 7;
  byt->b0 = frm->CAN_FRM_DATA[startPos++];
  return 8;
}
*/


