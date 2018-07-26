/*
 * Serial.h
 *
 *  Created on: 25 jul 2018
 *      Author: jof
 */

#ifndef SYSTEM_THINGS_CANSERIAL_H_
#define SYSTEM_THINGS_CANSERIAL_H_

#include <CAN.h>
#include <Stream.h>
#include <inttypes.h>

// based on HardwareSerial
// prints on CAN instead of a serialport

namespace CAN {

class Serial1 : public ::Stream {
  CAN::ControllerBase *m_controller;
  uint8_t m_sendIdx,
          m_recvIdx;
public:
  explicit Serial1(CAN::ControllerBase *controller, uint16_t sendIdx);
  virtual ~Serial1();

  size_t write(const uint8_t *buffer, size_t size);
  void end(void);
  int available(void);
  int peek(void);
  int read(void);
  void flush(void);
  int availableForWrite(void);
  void clear(void);
  size_t write(uint8_t);
  using Print::write;
};


} /* namespace CAN */

#ifdef DEBUG_CAN_SERIAL
extern CAN::Serial1 CanSerial; // defined in .ino file
#else
typedef struct {
  /*squelsher*/
  static void print(...) {}
  static void println(...) {}
  static void printf(...) {}
  static flush(){}

} _CanSerialSquelsher;
extern _CanSerialSquelsher CanSerial;
#endif


#endif /* SYSTEM_THINGS_CANSERIAL_H_ */
