/*
 * PID.cpp
 *
 *  Created on: 4 apr 2015
 *      Author: jof
 */

#include <Arduino.h>
#include <Ntc.h>
#include <PID.h>

using namespace PID;

Base::Base (PIDs::IDs id, PID::Types availableTypes,
                 PID::Types preferedType, PID::byteSizes size) :
  next(nullptr),
  m_byteSize(size),
  m_updated(false),
  m_id(id),
  m_availableTypes(availableTypes),
  m_preferedType(preferedType)
{
  m_vlu.uint32 = 0;
}

void Base::setRawValue(uint16_t value)
{
  m_vlu.uint16 = value;
}


void Base::setRawData32(uint32_t data) {
  m_vlu.uint32 = data;
}

PID::States Base::valueAsState(bool &ok)
{
  if (m_availableTypes & PID::Types::states) {
      ok = true;
      return static_cast<State*>(this)->state();
  }
  ok = false;
  return PID::States::Off;
}

float Base::valueAsFloat(PID::Types type, bool &ok)
{
  if (m_availableTypes & type) {
      ok = true;
      if (type == Types::input_Temperature) {
          PID::sensor_NTC *ntc = static_cast<PID::sensor_NTC*>(this);
          return ntc->celcius();
      } else if (type == Types::input_Volts) {
          PID::sensor_Base *base = static_cast<PID::sensor_Base*>(this);
          return base->volt();
      } else if (type == Types::input_Digital ||
                 type == Types::output_Digital)
      {
          return digital();
      }
      return m_vlu.decimal;
  }

  ok = false;
  return 0.0;
}

uint16_t Base::valueAsUInt16(PID::Types type, bool &ok)
{
  if (m_availableTypes & type) {
      ok = true;
      if (type < Types::output_RawData) { // its a input
          if (type == Types::input_Pressure) {
              PID::sensor_Pressure *kpa = static_cast<PID::sensor_Pressure*>(this);
              return kpa->pressureKPa();
          } else if (type == Types::input_Current) {
              PID::sensor_Current *cs = static_cast<PID::sensor_Current*>(this);
              return cs->current();
          } else if (type == Types::input_Digital) {
              return digital();
          }
          return m_vlu.uint16;
      } else {
          // its an output
          if (type == Types::output_Duty){
              PID::actuator_PWM *pwm = static_cast<PID::actuator_PWM*>(this);
              return pwm->duty();
          } else if (type == Types::output_Digital) {
              return digital();
          }
          return m_vlu.uint16;
      }
  }

  ok = false;
  return 0;
}

uint8_t Base::valueAsUInt8(PID::Types type, bool &ok)
{
  if (m_availableTypes & type) {
    if (type < Types::output_RawData) { // its a input
        if (type == Types::input_Current) {
          ok = true;
          PID::sensor_Current *cs = static_cast<PID::sensor_Current*>(this);
          return cs->current();
        } else if (type == Types::input_Digital) {
          ok = true;
          return this->digital();
        } else if (type == Types::input_Steps) {
            PID::sensor_Base *sensor = static_cast<PID::sensor_Base*>(this);
            if (sensor->ADmax() < 256) {
                ok = true;
                return m_vlu.uint8;
            }
        }
        // fallthrough to error
    } else {
        // its a output
        if (type == Types::output_Duty) {
            ok = true;
            PID::actuator_PWM *pwm = static_cast<PID::actuator_PWM*>(this);
            return pwm->duty();
        } else if (type == Types::output_Digital) {
            ok = true;
            return digital();
        } else if (m_vlu.uint16 < 256) {
            ok = true;
            return m_vlu.uint8;
        }
    }
  }

  ok = false;
  return 0;
}

bool Base::valueAsBool(Types type, bool &ok) const
{
  if (m_availableTypes & type) {
      if (type == Types::input_Digital || type == Types::output_Digital) {
          ok = true;
          return digital();
      }
  }
  ok = false;
  return false;
}


// -------------------------------------------
sensor_Base::sensor_Base (PIDs::IDs id, PID::Types availableTypes,
                               PID::Types preferedType, uint16_t ADmax, PID::byteSizes size) :
    PID::Base(id, availableTypes, preferedType, size),
  m_ADmax(ADmax)
{}



float sensor_Base::volt() const
{
  static float factor = 5.0 / m_ADmax;
  return factor * m_vlu.uint16;
}

// ------------------------------------------

uint8_t sensor_Current::current() const
{
  return map(0, 255, 0, 33, m_vlu.uint8); // 0-3.3V -> 0-255 steps, 1A = 0.1V
}

void sensor_Current::setCurrent(uint8_t current)
{
  m_vlu.uint8 = map(0, 33, 0, 255, current);
}

// ------------------------------------------------

uint16_t sensor_Pressure::pressureKPa() const
{
  // assumes sensor is https://www1.elfa.se/data1/wwwroot/assets/datasheets/PSE530_eng_datasheet.pdf
  // 1-5 volts range 0->1000kpa (0-10bar)
  static int stepsIn1Volt = m_ADmax / 5; // 819;
  int p = m_vlu.uint16 - stepsIn1Volt; // remove 1V (range 1-5V)
  if (p < 0) return 0; // below range

  //1000 / 4 = 250
  //1000 / (4096 - 819) = 1000 / 3277 = 0.30515715
  static float factor = 1000.0 / (m_ADmax - stepsIn1Volt); //0.30515715;
  //Serial.printf("res: %f p:%i factor:%f admax:%u", p*factor, p, factor, m_ADmax);
  return static_cast<uint16_t>(p * factor);
}

// ----------------------- begin PID_sensor_NTC ----------------
sensor_NTC::sensor_NTC(PIDs::IDs id, PID::Types availableTypes,
                            PID::Types preferedType, uint16_t ADmax, Ntc *ntcObj, PID::byteSizes size) :
    sensor_Base(id, availableTypes, preferedType, ADmax, size),
    m_ntcObj(ntcObj)
{
}

float sensor_NTC::celcius() const
{
  return m_ntcObj->valueToTemp(m_vlu.uint16);
}


// -----------------------------------------------------------
State::State(PIDs::IDs id) :
    Base(id, static_cast<PID::Types>(PID::Types::states | PID::Types::input_RawData |
             PID::Types::output_RawData),
         PID::Types::states, PID::byteSizes::twoByte)
{
}

// ------------------------ begin outputs ---------------------
uint8_t actuator_PWM::duty() const
{
  return map(0, 255, 0, 100, m_vlu.uint8);
}


// ------------------------ begin PIDs namespace ---------------
PIDs::_Handler<PID::Base> PIDs::collection;


