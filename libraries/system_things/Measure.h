/*
 * Measure.h
 *
 *  Created on: 1 apr 2015
 *      Author: jof
 */

#ifndef MEASURE_H_
#define MEASURE_H_
#include <ADC.h>

/**
 * This initializes and sets up the ADCs for use
 */
class Measurement :public ADC
{
  ADC_Module *m_sensorADC0;
  ADC_Module *m_sensorADC1;
public:
  Measurement ();
  virtual ~Measurement ();

  void initADC();
  inline ADC_Module *sensorADC0() { return m_sensorADC0; }
  inline ADC_Module *sensorADC1() { return m_sensorADC1; }

};

// global variable
extern Measurement measure;

#endif /* MEASURE_H_ */
