/*
 * AirFeed.cpp
 *
 *  Created on: 1 apr 2015
 *      Author: jof
 */

#include "AirFeed.h"

AirFeed::AirFeed (PID::State *pid, BTS6133D *compressorDrv, ITS5215L *airdryerDrv,
                  PID::sensor_Pressure *systemPressurePID, PID::sensor_NTC *compressorTemp) :
  m_compressorOutDrv(compressorDrv),
  m_airdryerOutDrv(airdryerDrv),
  m_errType(errorTypes::NoError),
  m_pid(pid),
  m_systemPressure(systemPressurePID),
  m_compressorTemp(compressorTemp)
{
}

AirFeed::~AirFeed ()
{
}

void AirFeed::loop()
{
  static const uint8_t cycleTime = 50;
  static uint32_t nextCheck = millis() + cycleTime;
  // only check this each 50ms or more
  if (nextCheck < millis()) {
    nextCheck = millis() + cycleTime;

    static uint32_t airdryerEndAt = 0;
    PID::States state = m_pid->state();
    if (m_pid->state() == PID::States::Off && m_systemPressure->pressureKPa() < lowestPressure &&
        m_compressorTemp->celcius() < maxCompressorTemp)
    {
        // softStart compressor
        m_pid->setState(PID::States::RevUp);
        m_pid->setUpdated(true);
        m_compressorOutDrv->setValue(10);
    } else if (state == PID::States::RevUp &&
        m_compressorOutDrv->pidCs()->current() < maxCurrent)
    {
        int duty = m_compressorOutDrv->pid()->rawValue() + incDuty;
        m_compressorOutDrv->setValue(duty);
        if (duty > 99) {
            m_pid->setState(PID::States::FullRev);
            m_pid->setUpdated(true);
        }
    } else if (m_pid->state() == PID::States::FullRev &&
               m_systemPressure->pressureKPa() > highestPressure)
    {
        m_pid->setState(PID::States::Airdryer);
        m_pid->setUpdated(true);
        m_airdryerOutDrv->setValue(100);
        airdryerEndAt = millis() + airdryerTimeMs;
    } else if(m_pid->state() == PID::States::Airdryer && airdryerEndAt < millis()) {
        int duty = m_compressorOutDrv->pid()->rawValue() - incDuty;
        if (duty < 0) {
          // overflow
          duty = 0;
          m_pid->setState(PID::States::Off);
          m_pid->setUpdated(true);
          airdryerEndAt = 0;
        } else {
          m_pid->setState(PID::States::RevDown);
          m_pid->setUpdated(true);
        }
        m_compressorOutDrv->setValue(duty);
    } else if (m_pid->state() != PID::States::Off && state != PID::States::Error &&
               m_compressorTemp->celcius() > maxCompressorTemp)
    {
        m_compressorOutDrv->setValue(0);
        m_pid->setState(PID::States::Error);
        m_pid->setUpdated(true);
        m_errType = errorTypes::OverHeated;
    }
  }
}

void AirFeed::resetError()
{
  m_errType = errorTypes::NoError;
  if (m_pid->state() == PID::States::Error) {
      m_pid->setState(PID::States::Off);
      m_pid->setUpdated(true);
  }
}

