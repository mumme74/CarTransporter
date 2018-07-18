/*
 * Inputs.cpp
 *
 *  Created on: 1 apr 2015
 *      Author: jof
 */

#include "Inputs.h"
#include "Measure.h"

// a global
List<IInput> InputsController;
IntervalTimer inputTmr;


// ------------------ begin IInput interface class -----------------------
IInput::IInput (PID::Base *pid) :
    next(nullptr),
    m_pid(pid),
    m_state(PID::States::Working),
    m_errType(errorTypes::NoError)
{
}

IInput::~IInput ()
{
}


void IInput::resetError()
{
  m_errType = errorTypes::NoError;
}

// ---------------- begin AnalogIn --------------------
AnalogIn::AnalogIn(uint8_t inPin, uint16_t minVlu,
                   uint16_t maxVlu, uint8_t slowFactor, PID::Base *pid) :
      IInput(pid),
    m_reversed(false),
    m_inPin(inPin),
    m_factor(slowFactor),
    m_maxVlu(maxVlu),
    m_minVlu(minVlu)
{
}

AnalogIn::~AnalogIn()
{
}


void AnalogIn::init()
{
  InputsController.addItem(this);
}

void AnalogIn::interval()
{
  static bool startup = true;

  uint16_t oldVlu = m_pid->rawValue();
  uint16_t vlu = measure.sensorADC0()->analogRead(m_inPin);
  if (m_reversed) {
      vlu = 4095 - vlu;
  }
  m_pid->setRawValue(vlu);
  m_pid->setUpdated(oldVlu != vlu);
  if (vlu < m_minVlu) {
      m_errType = errorTypes::ToLowValue;
  } else if (vlu > m_maxVlu) {
      m_errType = errorTypes::ToHighValue;
  } else if (!startup) {
      // more than allowed difference since last reading is an error
      int32_t low = (oldVlu * (100 - m_factor)) / 100;
      int32_t high = (oldVlu * (100 + m_factor)) / 100;

      if (oldVlu > vlu && low > vlu) {
          // rapid decrease in value
          m_errType = errorTypes::ToFastFalling;
      } else if (oldVlu < vlu && high < vlu) {
          // rapid increase in value
          m_errType = errorTypes::ToFastRising;
      }
  }
  startup = true;
}




// -------------------- begin isr function ------------------------------
void isr_scan_inputs()
{
  // scan each inputs in a sequence
  static IInput *in = InputsController.first();
  in = in->next;

  if (in == nullptr)
	  in = InputsController.first();
  in->interval();
}


// -------------------- begin input tmr function ------------------------
void init_inputs()
{
  // each 20 ms -> 6 inputs ->120 ms cycle or scan 8 times a sec
  inputTmr.begin(isr_scan_inputs, 20000);
}



