/*
 * crc32.h
 *
 *  Created on: 29 jul 2018
 *      Author: jof
 */

#ifndef CRC32_H_
#define CRC32_H_

#include <stdint.h>
#include <stddef.h>

#define BOOTLOADER_CRC32_POLYNOMIAL 0xEDB88320  // bit reversed 0x04C11DB7

uint32_t crc32(uint32_t init, const void *data, size_t length);


#endif /* CRC32_H_ */
