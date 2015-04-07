/*
 * AirFeed.h
 *
 *  Created on: 1 apr 2015
 *      Author: jof
 */

#ifndef AIRFEED_H_
#define AIRFEED_H_

#include "Outputs.h"
#include "Inputs.h"
#include <errors.h>
#include <PID.h>

/**
 * Controls the compressor and the airdryer
 */
class AirFeed
{
  BTS6133D *m_compressorOutDrv;
  ITS5215L *m_airdryerOutDrv;
public:
  AirFeed(PID::State *pid, BTS6133D *compressorDrv, ITS5215L *airdryerDrv,
          PID::sensor_Pressure *systemPressurePID, PID::sensor_NTC *compressorTemp);
  virtual ~AirFeed ();

  const uint16_t highestPressure = 900; // 900kpa = 9bar -> above this turn off compressor
  const uint16_t lowestPressure = 800; // 800kpa ->below this= turn on compressor
  const uint8_t maxCurrent = 25; // 20A max when starting up
  const uint8_t incDuty = 3; // how much each cycle should increment duty
  const uint16_t airdryerTimeMs = 5000;
  const uint8_t maxCompressorTemp = 100; // max 100 deg celsius
 // enum State : uint8_t { Off, RevUp, FullRev, Airdryer, RevDown, Error };


  void loop(); // called from mainloop
  errorTypes error() const { return m_errType; }
  void resetError();
  PID::State *pid() const { return m_pid; }

private:
  errorTypes m_errType;
  PID::State *m_pid;
  PID::sensor_Pressure *m_systemPressure;
  PID::sensor_NTC      *m_compressorTemp;
};

#endif /* AIRFEED_H_ */
