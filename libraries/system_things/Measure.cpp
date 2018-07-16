/*
 * Measure.cpp
 *
 *  Created on: 1 apr 2015
 *      Author: jof
 */

#include "Measure.h"

// this one is global
Measurement measure;

Measurement::Measurement () :
    ADC()
{
  m_sensorADC1 = adc1;
  m_sensorADC0 = adc0;
}

void Measurement::initADC()
{
  // current sense for compressor (current pcb don't work with ADC1)
  m_sensorADC1->setAveraging(0);
  m_sensorADC1->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED);
  m_sensorADC1->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED);
  m_sensorADC1->setResolution(12);
  m_sensorADC1->setReference(ADC_REFERENCE::REF_EXT);
  m_sensorADC1->enableCompare(0, 1);

  // somehow set up DMA read for sensorADC
  m_sensorADC0->setAveraging(4);
  m_sensorADC0->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED);
  m_sensorADC0->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED);
  m_sensorADC0->setResolution(12);
  m_sensorADC0->setReference(ADC_REFERENCE::REF_EXT);
  m_sensorADC0->enableCompare(0, 1);
}

Measurement::~Measurement ()
{
}
