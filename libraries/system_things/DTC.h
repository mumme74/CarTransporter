/*
 * DTC.h
 *
 *  Created on: 4 apr 2015
 *      Author: jof
 */

#ifndef DTC_H_
#define DTC_H_

#include <stdint.h>
#include <PID.h>
#include <suspension_errors.h>
#include <List.h>

/**
 * Storage class Diagnostic Trouble Codes
 */
struct DTC {
  DTC (uint16_t id, errorTypes err, uint8_t times = 1);
  ~DTC ();
  static const uint8_t pendingMask = 0x80;
  inline uint16_t pidID() const { return m_pidID; }
  inline errorTypes errType() const { return m_errType; }
  inline uint8_t mask() const { return m_mask; }
  inline void setMask(uint8_t mask) { m_mask = mask; }
  inline bool isPending() const { return m_mask & pendingMask; }
  inline uint8_t occurences() const { return m_mask & ~pendingMask; }
  void increment();
  uint8_t storedIndex() const;

  DTC *next; // next item used internally for iteration

private:

  uint16_t m_pidID;
  errorTypes m_errType;
  uint8_t m_mask; // a dtc with a 1 on 8th bit is a real code, not pending
                  // some codes doesnt classify as a real DTC directly,
                  // they need to occur several times during drivecycle
};

/**
 * Storage Freeze Frame data
 */
class FreezeFrame {
public:
  FreezeFrame(PID::Base *pid, errorTypes err);
};

/**
 * Stores errors in EEPROM
 */
class DTC_controller
{
public:
  List<DTC> store;
  DTC_controller ();
  virtual ~DTC_controller ();
  void init();
  DTC *setDTC(PID::Base *pid, errorTypes errType);
  DTC *setDTC(uint16_t id, errorTypes errType);
  uint8_t clear();
};

extern DTC_controller DTCs;

#endif /* DTC_H_ */
