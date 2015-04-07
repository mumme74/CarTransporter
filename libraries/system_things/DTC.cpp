/*
 * DTC.cpp
 *
 *  Created on: 4 apr 2015
 *      Author: jof
 */

#include <DTC.h>
#include "storage.h"
#include <EEPROM.h>

// a global variable
DTC_controller DTCs;


bool DTC_filter_check(const DTC *dtc)
{


  // default to its always a real DTC
  return true;
}


// ------------------ begin dtc -------------------------
DTC::DTC (uint16_t id, errorTypes err, uint8_t times) :
  next(nullptr),
  m_pidID(id),
  m_errType(err),
  m_mask(times)
{}

DTC::~DTC ()
{}

void DTC::increment()
{
  if (m_mask < pendingMask) {
    ++m_mask;
  }
  // run dtc filter
  if (!(m_mask & pendingMask)){
      if (m_errType == errorTypes::ToFastFalling) {
          switch (m_pidID){
            case static_cast<int>(PIDs::IDs::systemPressure_12bit):
              m_mask |= pendingMask;
              break;
            default:
              if (m_mask > 3) {
                m_mask |= pendingMask;
              }
          }
      } else if (m_errType == errorTypes::ToHighValue) {
          // filter glitches
          if (m_mask > 2) {
            m_mask |= pendingMask;
          }
      } else if (m_errType == errorTypes::ToLowValue) {
          if (m_mask > 3){
            m_mask |= pendingMask;
          }
      }
  }
}

// ------------------ begin DTC_controller --------------
DTC_controller::DTC_controller()
{}

DTC_controller::~DTC_controller()
{
  // delete each items as they are created on heap
  for (DTC *dtc = store.first(); dtc != nullptr; dtc = store.next()) {
    delete dtc;
  }
}

/**
 * Loads from EEPROM
 */
void DTC_controller::init()
{
  uint8_t errCount = EEPROM.read(store::DTC_COUNT_ADR);
  uint16_t startAdr = EEPROM.read(store::DTC_STARTPTR_ADR_HI) << 8;
  startAdr |= EEPROM.read(store::DTC_STARTPTR_ADR_LO);


  for (unsigned int i = errCount, adr = startAdr;
       i < store::DTC_MAXCOUNT; ++i,
       adr += store::DTC_BYTESIZE)
  {
    if (adr > store::DTC_STORE_END_ADR) {
        // we continue at the beginning, ie ring store has reached its end
        adr = store::DTC_STORE_START_ADR;
    }

    // read DTC data from EEPROM
    uint16_t id = EEPROM.read(adr + store::DTC_ADRESS_HI_OFFSET) << 8;
    id |= EEPROM.read(adr + store::DTC_ADRESS_LO_OFFSET);
    errorTypes errType = static_cast<errorTypes>(EEPROM.read(adr + store::DTC_ERRTYPE_OFFSET));
    uint8_t occurences = EEPROM.read(adr + store::DTC_OCCURENCES_OFFSET);

    // create a cached object in ram
    DTC *dtc = new DTC(id, errType, occurences);
    store.addItem(dtc);
  }

}

/**
 * Used to store a DTC
 *  a DTC can be pending ie its stored but it doesnt broadcast a error on CAN bus
 *  when DTC filter is passed it converts to a real DTC
 *  @returns true if DTC filter has passed and it stored as a true DTC
 */
DTC *DTC_controller::setDTC(uint16_t id, errorTypes errType)
{

  // find out if it was previously stored
  DTC *dtc = nullptr;
  int dtcPos = -1;
  for (DTC *cached = store.first(); cached != nullptr; cached = store.next()) {
    ++dtcPos;
    if (cached->pidID() == id && cached->errType() == errType) {
        dtc = cached;
        break;
    }
  }


  uint8_t errCount = EEPROM.read(store::DTC_COUNT_ADR);
  uint16_t startAdr = EEPROM.read(store::DTC_STARTPTR_ADR_HI) << 8;
  startAdr |= EEPROM.read(store::DTC_STARTPTR_ADR_LO);

  if (dtc == nullptr) {
      // it is a new DTC
      dtc = new DTC(id, errType);
      store.addItem(dtc);

      // we should store it in EEPROM
      startAdr += store::DTC_BYTESIZE;

      // if ringstore should flip over
      if (startAdr > store::DTC_STORE_END_ADR) {
        startAdr = store::DTC_STORE_START_ADR;
      }

      if (errCount < store::DTC_MAXCOUNT) {
        ++errCount;
        // update DTC control register
        EEPROM.write(store::DTC_COUNT_ADR, errCount);
      }

      // write DTC
      EEPROM.write(startAdr + store::DTC_ADRESS_HI_OFFSET,
                   static_cast<uint8_t>((dtc->pidID() & 0xFF00) >> 8));
      EEPROM.write(startAdr + store::DTC_ADRESS_LO_OFFSET,
                   static_cast<uint8_t>(dtc->pidID() & 0x00FF));
      EEPROM.write(startAdr + store::DTC_ERRTYPE_OFFSET,
                   static_cast<uint8_t>(dtc->errType()));
      EEPROM.write(startAdr + store::DTC_OCCURENCES_OFFSET,
                   dtc->occurences());

      // update DTC control registers
      EEPROM.write(store::DTC_STARTPTR_ADR_HI,
                   static_cast<uint8_t>((startAdr & 0xFF00) >> 8));
      EEPROM.write(store::DTC_STARTPTR_ADR_LO,
                   static_cast<uint8_t>(startAdr & 0x00FF));

  } else if (dtc->occurences() < 0xFF) {
      // previously stored DTC
      dtc->increment();
      EEPROM.write(startAdr + store::DTC_OCCURENCES_OFFSET, dtc->occurences());
  }

  return dtc;
}

/**
 * Sets a new DTC or updates a old one
 * Same as above
 */
DTC *DTC_controller::setDTC(PID::Base *pid, errorTypes errType) {
  return setDTC(static_cast<uint16_t>(pid->id()), errType);
}

