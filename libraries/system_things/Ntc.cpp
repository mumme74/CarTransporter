/*
 * Ntc.cpp
 *
 *  Created on: 1 apr 2015
 *      Author: jof
 */

#include "Ntc.h"
#include <math.h>
#include <Arduino.h>

Ntc::Ntc(uint16_t seriesOhm, uint16_t beta, uint16_t ntcOhm, uint16_t atTemp, uint16_t maxValue):
  m_seriesOhm(seriesOhm),
  m_beta(beta),
  m_ntcOhm(ntcOhm),
  m_atTemp(atTemp),
  m_maxValue(maxValue)
{
}

Ntc::~Ntc ()
{
}


float Ntc::valueToTemp(uint16_t vlu)
{
  // algorithm from https://learn.adafruit.com/thermistor/using-a-thermistor
  float fVlu;
  //fVlu = (m_maxValue - 1) / (vlu - 1);
  fVlu = (4095 / vlu) -1;

  // convert the value to resistance
  fVlu = m_seriesOhm / fVlu; //SERIESRESISTOR / fVlu;

#ifdef DEBUG_UART_ON
#ifdef DEBUG_NTC_ON
  Serial.print("ADvlu:");
  Serial.println(vlu);
  Serial.print("Thermistor resistance ");
  Serial.println(fVlu);
#endif
#endif

  float steinhart;
  steinhart = fVlu / m_ntcOhm;	//THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);   // ln(R/Ro)
  steinhart /= m_beta; 			//BCOEFFICIENT;          // 1/B * ln(R/Ro)
  steinhart += 1.0 / (m_atTemp + 273.15); //(TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C
#ifdef DEBUG_UART_ON
#ifdef DEBUG_NTC_ON
  Serial.print("Temperature ");
  Serial.print(steinhart);
  Serial.println(" *C");
#endif
#endif
  return steinhart;
}

/*
 * Need to understand how to do this one
uint16_t Ntc::tempToValue(float temp)
{
  temp += 273.15;
  temp = 1.0 / temp;
  temp -=
}
*/

