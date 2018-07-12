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
  m_compressorADC = adc1;
  m_sensorADC1 = adc0;
}

void Measurement::initForCompressor()
{
  // current sense for compressor
  m_compressorADC->setAveraging(0);
  m_compressorADC->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED);
  m_compressorADC->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED);
  m_compressorADC->setResolution(8);
  m_compressorADC->setReference(ADC_REFERENCE::REF_EXT);
  m_compressorADC->enableCompare(0, 1);

  // somehow set up DMA read for sensorADC
  m_sensorADC1->setAveraging(4);
  m_sensorADC1->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED);
  m_sensorADC1->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED);
  m_sensorADC1->setResolution(12);
  m_sensorADC1->setReference(ADC_REFERENCE::REF_EXT);
  m_sensorADC1->enableCompare(0, 1);
}

Measurement::~Measurement ()
{
}
