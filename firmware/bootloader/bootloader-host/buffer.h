/*
 * buffer.h
 *
 *  Created on: 7 jul 2019
 *      Author: jof
 */
#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>

/**
 * This file implements a circular, self expanding buffer
 */

/**
 * @brief Buffer_t, struct which holds buffer data
 */
typedef struct {
    uint32_t head,
             tail,
             size;
    char *buf;
} Buffer_t;

/**
 * @brief buffer_init, initialize buffer
 * @param b, ptr to a Buffer obj
 * @param size, buffer initial size
 * @return 1 on success, 0 on failure ad sets canbridge_errmsg
 */
int buffer_init(Buffer_t *b, uint32_t size);

/**
 * @brief buffer_free, frees (de-init) bufferobj b
 * @param b, ptr to Buffer obj
 */
void buffer_free(Buffer_t *b);

/**
 * @brief buffer_clear, clears content in buffer
 * @param b, ptr to Buffer obj
 */
void buffer_clear(Buffer_t *b);

/**
 * @brief buffer_available, how many bytes there is stored
 * @param b, ptr to Buffer obj
 * @return how many chars there currently is in buffer
 */
uint32_t buffer_available(const Buffer_t *b);

/**
 * @brief buffer_available, how many bytes is available before resize
 * @param b, ptr to Buffer obj
 * @return how many bytes there is left before resize occurs
 */
uint32_t buffer_available_sz(const Buffer_t *b);

/**
 * @brief buffer_put, puts bytes into buffer
 * @param b, ptr to Buffer obj
 * @param srcbuf, data to insert into buffer
 * @param len, number of bytes to store
 * @return 1 on success, 0 on failure (for example mem. alloc.)
 */
int buffer_put(Buffer_t *b, const char *srcbuf, uint32_t len);

/**
 * @brief buffer_get, gets bytes from buffer (removes from buffer)
 * @param b, ptr to buffer obj
 * @param destbuf, data is inserted here
 * @param len, number of bytes to get,
 *              if len > available of gets available number
 * @return number of bytes moved into destbuf
 */
int buffer_get(Buffer_t *b, char *destbuf, uint32_t len);

/**
 * @brief buffer_peek, peeks into buffer.
 *                      same as buffer_get but without remove bytes
 * @param b, ptr to Buffer obj
 * @param peekbuf, store peek data here
 * @param len, number of bytes to peek
 *              if len > available of gets available number
 * @return number of bytes copied into peekbuf
 */
int buffer_peek(Buffer_t *b, char *peekbuf, uint32_t len);

/**
 * @brief buffer_index_of, looks for needle i available bytes
 * @param b, ptr to buffer obj
 * @param needle, byte to search for
 * @param startpos, the startpos for looking, usually 0
 * @return pos of needle if found or -1
 */
int buffer_index_of(Buffer_t *b, char needle, uint32_t startpos);

#endif // BUFFER_H
