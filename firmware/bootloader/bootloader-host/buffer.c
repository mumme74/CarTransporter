/*
 * buffer.c
 *
 *  Created on: 7 jul 2019
 *      Author: jof
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cantypes.h"
#include "buffer.h"
#include "canbridge.h"



int buffer_init(Buffer_t *b, uint32_t size)
{
    b->size = size;
    b->buf = (char*)malloc(size * sizeof (b->buf));
    if (b->buf == NULL) {
        CANBRIDGE_SET_ERRMSG("Failed to allocate memory\n");
        return 0;
    }

    return 1;
}

void buffer_free(Buffer_t *b)
{
    if (b->buf != NULL)
        free(b->buf);
}

void buffer_clear(Buffer_t *b)
{
    b->head = b->tail = 0;
}

uint32_t buffer_available(const Buffer_t *b)
{
    if (b->head > b->tail)
        return b->head - b->tail;
    else if (b->head < b->tail)
        return b->size - b->tail + b->head;
    return 0;
}

uint32_t buffer_available_sz(const Buffer_t *b)
{
    if (b->head < b->tail)
        return (b->tail - b->head);
    else if (b->head > b->tail)
        return (b->size - b->head + b->tail);
    return b->size;
}

int buffer_put(Buffer_t *b, const char *srcbuf, uint32_t len)
{
    if (buffer_available_sz(b) < len) {
        // need to expand our buffer
        uint32_t oldHead = b->head,
                 oldTail = b->tail;
        char *oldBuf = b->buf;

        // malloc new data, double in size
        uint32_t newSz = b->size + b->size;
        if (!buffer_init(b, newSz > len ? newSz : len))
            return 0;
        if (oldTail < oldHead)
            memcpy(b->buf, oldBuf + oldTail, oldHead - oldTail);
        else {
            // has flipped over
            uint32_t upperLen = b->size - oldTail;
            memcpy(b->buf, oldBuf + oldTail, upperLen);
            memcpy(b->buf, oldBuf, oldHead);
        }
    }

    if (b->head >= b->tail) {
        // copy up to end of buffer
        uint32_t upperLen = (b->head + len > b->size) ? b->size - b->head : len;
        memcpy(b->buf + b->head, srcbuf, upperLen);
        len -= upperLen;
        b->head += upperLen;

        // if anything is left, flip over and start from bottom
        if (len > 0) {
            // flip over and copy the rest
            b->head = len;
            memcpy(b->buf, srcbuf + upperLen, len);
        }
    } else {
        // tail is higher than head, we have flipped over
        // we should never be able to cross til boundary here.
        // it is checked in the expand check above
        memcpy(b->buf + b->head, srcbuf, len);
    }

    return 1;
}

int buffer_get(Buffer_t *b, char *destbuf, uint32_t len)
{
    // make sure we don't copy more than we currently have in buffer
    uint32_t cpLen = buffer_available_sz(b);
    if (cpLen > len)
        cpLen = len;

    if (b->tail < b->head) {
        // not flipped over
        memcpy(destbuf, b->buf + b->tail, cpLen);
        b->tail += cpLen;
    } else {
        // has flipped over, copy upper bytes
        uint32_t upLen = b->size - b->tail;
        memcpy(destbuf, b->buf + b->tail, upLen);
        b->tail = 0;

        // copy whats left in lower part of buffer
        if (cpLen > upLen) {
            b->tail = cpLen - upLen;
            memcpy(destbuf + upLen, b->buf, cpLen - upLen);
        }
    }

    return (int)cpLen;
}


int buffer_peek(Buffer_t *b, char *peekbuf, uint32_t len)
{
    // make sure we don't copy more than we currently have in buffer
    uint32_t cpLen = buffer_available_sz(b);
    if (cpLen > len)
        cpLen = len;

    if (b->tail < b->head) {
        // not flipped over
        memcpy(peekbuf, b->buf + b->tail, cpLen);
    } else {
        // has flipped over, copy upper bytes
        uint32_t upLen = b->size - b->tail;
        memcpy(peekbuf, b->buf + b->tail, upLen);

        // copy whats left in lower part of buffer
        if (cpLen > upLen) {
            memcpy(peekbuf + upLen, b->buf, cpLen - upLen);
        }
    }

    return (int)cpLen;
}


int buffer_index_of(Buffer_t *b, char needle, uint32_t startpos)
{
    if (b->tail < b->head) {
        for (uint32_t i = startpos, end = b->head - b->tail;
             i < end; ++i)
        {
            if (*(b->buf + b->tail + i) == needle)
                return (int)i;
        }
    } else if (b->tail > b->head) {
        // flipped over
        uint32_t pos = startpos;
        for (; pos < b->size; ++pos) {
            if (*(b->buf + b->tail + pos) == needle)
                return (int)pos;
        }
        for(uint32_t j = 0; j < b->head; ++j, ++pos) {
            if (*(b->buf + j) == needle)
                return (int)pos;
        }
    }

    return -1;
}

