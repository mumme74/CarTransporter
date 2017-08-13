/*
 * Outpin.h
 *
 *  Created on: 31 mar 2015
 *      Author: jof
 */

#ifndef OUTPUTS_H_
#define OUTPUTS_H_
#include <Arduino.h>

#include <List.h>
#include <suspension_errors.h>
#include <PID.h>
#include <ADC.h>

class IOutput {
public:
  //enum States : uint8_t { Off, On, ActuatorTest, Error };
  IOutput(PID::Base *pid);
  virtual ~IOutput();
  virtual void init() = 0;
  virtual void setActuatorTest(const uint8_t duty) = 0;
  virtual void setValue(uint8_t vlu) = 0;
  virtual void interval() = 0; // called by isr each 0.1ms
  inline errorTypes error() const { return m_errType; }
  inline PID::States state() const { return m_state; }
  virtual void resetError();
  inline PID::Base *pid() { return m_pid; }

  IOutput *next; // used internally to iterate the outpins

protected:
  PID::Base *m_pid;
  PID::States m_state;
  errorTypes m_errType;
};

/**
 * This class acts as a driver for ITS5215L highside driver chip
 *
 */
class ITS5215L : public IOutput {
public:
  ITS5215L(const uint8_t outPin, const uint8_t diagPin, PID::actuator_PWM *pid);
  virtual ~ITS5215L();

  void init();
  void setValue(uint8_t duty);
  void setActuatorTest(uint8_t duty);

  inline void interval(); // called by isr each 0.1ms

protected:
  uint8_t m_outPin;
  uint8_t m_diagPin;
  uint8_t m_pwmCnt; // used for software pwm, software pwm and timeouts are
                    // needed as we need to check diagpin when outpin is high
  bool m_pinHigh;
};


/**
 * This class acts as a driver BTS6133D highside driver
 */
class BTS6133D : public ITS5215L {
  uint8_t m_csPin;
  PID::sensor_Current *m_pidCs;
  ADC_Module *m_compressorADC;
public:
  BTS6133D(uint8_t outPin, uint8_t diagPin, uint8_t csPin,
           PID::actuator_PWM *pidPwm, PID::sensor_Current *pidCs,
           ADC_Module *compressorADC);
  ~BTS6133D();
  void init();
  void setValue(uint8_t duty);
  void setActuatorTest(uint8_t duty);
  PID::sensor_Current *pidCs() const { return m_pidCs; }

  inline void interval(); // called by isr each 0.1ms
};

/**
 * Handles outpins and makes makes it easy to iterate trough them
 */
extern List<IOutput> OutputsController;

// used to initialize outputs, start pwm loop etc
void init_outputs();


#endif /* OUTPUTS_H_ */
