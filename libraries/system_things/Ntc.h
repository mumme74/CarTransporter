/*
 * Ntc.h
 *
 *  Created on: 1 apr 2015
 *      Author: jof
 */

#ifndef NTC_H_
#define NTC_H_

#include <stdint.h>

class Ntc
{
  uint16_t m_seriesOhm;
  uint16_t m_beta;
  uint16_t m_ntcOhm;
  uint16_t m_atTemp;
  uint16_t m_maxValue;
public:
  Ntc (uint16_t seriesOhm, uint16_t beta, uint16_t ntcOhm, uint16_t atTemp, uint16_t maxValue);
  virtual ~Ntc ();

  float valueToTemp(uint16_t vlu); // feed a AD value get back a temp
  //uint16_t tempToValue(float temp); // feed a temp get back a AD steps
};

#endif /* NTC_H_ */
