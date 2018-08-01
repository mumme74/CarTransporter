/*
 * crc32.c
 *
 *  Created on: 29 jul 2018
 *      Author: jof
 */

#include <can_protocol.h>
#include "crc32.h"

// this function is grabbed from https://github.com/cvra/CRC/blob/56cc8e841085bc3cca5b0f608c85841a2b135677/crc32.c
uint32_t crc32(uint32_t init, const void *data, size_t length)
{
    // static to save some stackspace,
    // not sure if its needed due to non recursive, but bss (better save space)...
    static const uint8_t *p;
    static uint32_t i, crc, bit;

    p = (const uint8_t *)data;
    crc = ~init;
    for (i = 0; i < length; i++) {
        for (bit = 0; bit < 8; bit++) {
            if ((crc & 1) != ((p[i] >> bit) & 1)) {
                crc = (crc >> 1) ^ BOOTLOADER_CRC32_POLYNOMIAL;
            } else {
                crc = crc >> 1;
            }
        }
    }
    return ~crc;
}
