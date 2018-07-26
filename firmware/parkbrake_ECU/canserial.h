/*
 * canserial.h
 *
 *  Created on: 24 jul 2018
 *      Author: jof
 */

#ifndef CANSERIAL_H_
#define CANSERIAL_H_
#include <ch.h>


/*
 * CanSerialStream specific data.
 */
#define _canserial_stream_data                                                 \
  _base_sequential_stream_data  /* should be empty implementation? hal_streams.h */ \
  /* Pointer to the stream buffer.*/                                        \
  uint8_t               *buffer;                                            \
  /* Size of the stream.*/                                                  \
  size_t                size;                                               \
  /* Current end of stream.*/                                               \
  size_t                eos;                                                \
  /* Current read offset.*/                                                 \
  size_t                offset;

/*
 * MemStream virtual methods table, nothing added.
 */
struct CanSerialStreamVMT {
  _base_sequential_stream_methods
  /**
   * @brief use after Stream methods have been acting upon obj
   * arg ip   pointer to CanSerialObject
   */
  void (*flush)(void *instance);
};

/**
 * Memory stream object.
 */
typedef struct {
  /** @brief Virtual Methods Table.*/
  const struct CanSerialStreamVMT *vmt;
  _canserial_stream_data
} CanSerialStream;



extern CanSerialStream CANSerial0;

// post pointers to \0 terminated strings
//extern mailbox_t canserialMBSend;

void canserial_init(void);
void canserial_thdsTerminate(void);
void canserial_doShutdown(void);

size_t canserialAsynchronousWrite(uint8_t *msg, uint8_t len);

#endif /* CANSERIAL_H_ */
