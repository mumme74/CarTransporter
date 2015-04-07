/*
 * Outputs.cpp
 *
 *  Created on: 31 mar 2015
 *      Author: jof
 */

#include "Outputs.h"
#include "Measure.h"


// stores the different outpins
// globals
List<IOutput> OutputsController;
IntervalTimer pwmTmr;


// -------------------- begin IOutput (Interface class)
IOutput::IOutput(PID::Base *pid) :
    next(nullptr),
    m_pid(pid),
    m_state(PID::States::Off),
    m_errType(errorTypes::NoError)
{
}

IOutput::~IOutput()
{
}

void IOutput::resetError()
{
  m_errType = errorTypes::NoError;
}



// ----------------------- begin ITS5215L driver --------------------------


ITS5215L::ITS5215L(const uint8_t outPin, const uint8_t diagPin, PID::actuator_PWM *pid) :
    IOutput(pid),
  m_outPin(outPin),
  m_diagPin(diagPin),
  m_pwmCnt(0),
  m_pinHigh(false)
{

}

ITS5215L::~ITS5215L()
{
}

void ITS5215L::setValue(uint8_t duty)
{
  m_pid->setRawValue(duty);
  if (duty == 0) {
    m_state = PID::States::Off;
    m_pwmCnt = 0;
  } else {
    m_state = PID::States::On;
    if (duty > 100) {
      duty = 100;
    }
  }

  m_pwmCnt = duty;
}

void ITS5215L::setActuatorTest(uint8_t duty)
{
  setValue(duty);
  m_state = PID::States::ActuatorTest;
}

void ITS5215L::init()
{
  pinMode(m_outPin, OUTPUT);
  pinMode(m_diagPin, INPUT_PULLUP);
  OutputsController.addItem(static_cast<IOutput*>(this));
}


/* truth table for trouble on ITS5215L
                   | IN| OUT| ST(diagpin)
-------------------|---|----|----
Normal operation   | L |  L |  H
                   | H |  H |  H
-------------------|---|----|----
Open load          | L |  Z |  L ->  L on output, if potential at the Output exceeds the OpenLoad detection voltage
                   | H |  H |  H
-------------------|---|----|----
Overtemperature    | L |  L |  H
                   | H |  L |  L
*/

inline void ITS5215L::interval()
{
  if (m_state != PID::States::Error) {
    if (m_pwmCnt == 0) {
      // change state
      uint8_t duty = static_cast<uint8_t>(m_pid->rawValue());
      if (m_pinHigh) {
        // check for over temp
        if (digitalReadFast(m_diagPin) == LOW){
          m_errType = errorTypes::SwitchOvertemp;
          m_state = PID::States::Error;
          digitalWriteFast(m_outPin, LOW);
        } else if (duty < 100) {
          // change to low state
          digitalWriteFast(m_outPin, LOW);
          m_pinHigh = false;
          m_pwmCnt = 100 - duty;
        }
      } else { // m_pinHigh == false
        // check for open load
        if (digitalReadFast(m_diagPin) == LOW) {
          m_errType = errorTypes::OpenLoad;
          m_state = PID::States::Error;
        } else if (duty > 0) {
          digitalWriteFast(m_outPin, HIGH);
          m_pinHigh = true;
          m_pwmCnt = duty;
        }
      }
    } else {
      // normal increment pwmCnt
      --m_pwmCnt;
    }
  }
}


// -------------------------- begin BTS6133D driver --------------------
BTS6133D::BTS6133D(uint8_t outPin, uint8_t diagPin, uint8_t csPin,
                   PID::actuator_PWM  *pidPwm, PID::sensor_Current *pidCs,
                   ADC_Module *compressorADC) :
    ITS5215L(outPin, diagPin, pidPwm),
    m_csPin(csPin),
    m_pidCs(pidCs),
    m_compressorADC(compressorADC)
{
}

BTS6133D::~BTS6133D()
{
}

void BTS6133D::init()
{
  // configure as analog input
  pinMode(m_csPin, INPUT);
  m_compressorADC->analogRead(m_csPin);
}

void BTS6133D::setValue(uint8_t duty)
{
  if (duty == 0) {
    m_pidCs->setCurrent(0);
  }

  ITS5215L::setValue(duty);
}

void BTS6133D::setActuatorTest(uint8_t duty)
{
  setValue(duty);
  m_state = PID::States::ActuatorTest;
}

/*
                       IN   OUT   DIAG   CS
 Normal operation       0    0      0   0V
                        1    1      0   30A->3V, 5A->0,5V

 Overtemp, Overload,    0    0      0   0V
 Short to gnd           1    0      1   3,3V

 Open load              0    Z      0   0V
                        1    1      0   0V (No current flow)
 */
inline void BTS6133D::interval()
{
  if (m_state != PID::States::Error) {
    if (m_pwmCnt == 0) {
      uint8_t duty = static_cast<uint8_t>(m_pid->rawValue());
      // change state
      if (m_pinHigh) {
        // read current just before we set PWM to low
        uint8_t current = map(0, 255, 0, 33,
                        m_compressorADC->analogRead(m_csPin));
        m_pidCs->setCurrent(current);
        m_pidCs->setUpdated(true);

        // check for over temp
        if (digitalReadFast(m_diagPin) == HIGH){
          m_errType = errorTypes::Overload;
          m_state = PID::States::Error;
          digitalWriteFast(m_outPin, LOW);
        } else if (current == 0) {
          m_state = PID::States::Error;
          m_errType = errorTypes::OpenLoad;
        } else if (duty < 100) {
          // change to low state
          digitalWriteFast(m_outPin, LOW);
          m_pinHigh = false;
          m_pwmCnt = 100 - duty;
        }
      } else { // m_pinHigh == false
        // check for open load
        if (digitalReadFast(m_diagPin) == HIGH) {
          m_errType = errorTypes::HardwareFault;
          m_state = PID::States::Error;
        } else if (duty > 0) {
          digitalWriteFast(m_outPin, HIGH);
          m_pinHigh = true;
          m_pwmCnt = duty;
        }
      }
    }

    // normal increment pwmCnt
    --m_pwmCnt;
  }
}

// -------------------------- begin isr function ----------------------

void isr_each200us() // PWM 50Hz
{
  // iterate through all pins and update pwms
  for (IOutput *drv = OutputsController.first();
      drv != nullptr;
      drv = OutputsController.next())
  {
    drv->interval();
  }
}

// ----------------------- begin init function -----------------------

void init_outputs()
{
  pwmTmr.begin(isr_each200us, 200);
}
