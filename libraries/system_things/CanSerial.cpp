/*
 * Serial.cpp
 *
 * Based loosly on HarwareSerial.cpp
 *
 *  Created on: 25 jul 2018
 *      Author: jof
 */

#include "CanSerial.h"

#include "wiring_private.h"

#define RX_BUFFER_SIZE 64
static volatile uint8_t rx_buffer[RX_BUFFER_SIZE];
static volatile uint8_t rx_buffer_head = 0;
static volatile uint8_t rx_buffer_tail = 0;

#define TX_BUFFER_SIZE 0x7F
static volatile uint8_t tx_buffer[TX_BUFFER_SIZE+1];
static volatile uint8_t tx_buffer_head = 0;
static volatile uint8_t tx_buffer_tail = 0;

// Public Methods //////////////////////////////////////////////////////////////

namespace CAN {

Serial1::Serial1(CAN::ControllerBase *controller, uint16_t sendIdx) :
    m_controller(controller),
    m_sendIdx(sendIdx)
{
}

Serial1::~Serial1() {
}


void Serial1::end(void)
{
    rx_buffer_head = 0;
    rx_buffer_tail = 0;
}

// not implemented!
int Serial1::available(void)
{
  // not implemented!
    uint8_t head, tail;

    head = rx_buffer_head;
    tail = rx_buffer_tail;
    if (head >= tail) return head - tail;
    return RX_BUFFER_SIZE + head - tail;
}


// not implemented!
int Serial1::availableForWrite(void)
{
    uint8_t head, tail;

    head = rx_buffer_head;
    tail = rx_buffer_tail;
    if (head >= tail)
      return TX_BUFFER_SIZE - 1 - head + tail; // not flipped
    return tail - head - 1; // has flipped
}


// not implemented!
int Serial1::peek(void)
{
    uint8_t head, tail;

    head = rx_buffer_head;
    tail = rx_buffer_tail;
    if (head == tail) return -1;
    if (++tail >= RX_BUFFER_SIZE) tail = 0;
    return rx_buffer[tail];
}


// not implemented!
int Serial1::read(void)
{
    uint8_t c, i;

    if (rx_buffer_head == rx_buffer_tail) return -1;
    i = rx_buffer_tail + 1;
    if (i >= RX_BUFFER_SIZE) i = 0;
    c = rx_buffer[i];
    rx_buffer_tail = i;
    return c;
}

void Serial1::flush()
{
  static uint8_t nFrames;
  static uint8_t strlen;
  nFrames = 0;
  if  (tx_buffer_tail <= tx_buffer_head)
    strlen =  tx_buffer_head - tx_buffer_tail;
  else
    // flipped around
    strlen = TX_BUFFER_SIZE - tx_buffer_tail + tx_buffer_head;
  // Serial.printf("strl:%d th:%d tt:%d\r\n", strlen, tx_buffer_head, tx_buffer_tail);
  static CAN_message_t txf;
  m_controller->init_CAN_message_t(&txf, 2);
  txf.id = CAN_MSG_TYPE_DIAG | m_sendIdx;

  txf.len = 1;
  txf.buf[0] = 0x80 | strlen; // 1st frame

  while (strlen--) {
    // tx_buffer_tail should never pass tx_buffer_head due to strlen check in loop
    if (tx_buffer_tail >= TX_BUFFER_SIZE)
      tx_buffer_tail = 0; // flip around

    txf.buf[txf.len++] = *(tx_buffer + tx_buffer_tail);
    tx_buffer_tail++;

    if (txf.len > 7) {
      m_controller->send(txf);
      txf.len = 1;
      txf.buf[0] = ++nFrames;
    }
  }

  // we might get here with a unposted frame
  if (txf.len > 1)
    m_controller->send(txf);

  //Serial.printf("Wr no f:%u\r\n", nFrames);
}

void Serial1::clear()
{
    rx_buffer_head = rx_buffer_tail;
}

size_t Serial1::write(const uint8_t *buffer, size_t size)
{
  size_t res = Stream::write(buffer, size);
  flush();
  return res;
}

size_t Serial1::write(uint8_t c)
{
  if (TX_BUFFER_SIZE - tx_buffer_head <= 0) {
    // flip around
    if (tx_buffer_head - tx_buffer_tail == TX_BUFFER_SIZE)
      return 0; // full, not yet cleared
    tx_buffer_head = 0;
  }

  *(tx_buffer + tx_buffer_head) = c;
  tx_buffer_head += 1;
  return 1;
}

} /* namespace CAN */

#ifndef DEBUG_CAN_SERIAL
_CanSerialSquelsher CanSerial;
#endif
