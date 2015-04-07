/*
 * Inputs.h
 *
 *  Created on: 1 apr 2015
 *      Author: jof
 */

#ifndef INPUTS_H_
#define INPUTS_H_

#include <Arduino.h>
#include <List.h>
#include <errors.h>
#include <Ntc.h>
#include <PID.h>

/**
 * Interface class for inputs
 */
class IInput
{
public:
  IInput (PID::Base *pid);
  virtual ~IInput ();

  //enum States : uint8_t { Working, Error };
  virtual void init() = 0;
  inline errorTypes error() const { return m_errType; }
  virtual void interval() = 0;
  PID::States state() const { return m_state; }
  virtual void resetError();
  inline PID::Base *pid() const { return m_pid; }


  IInput *next; // used internally to iterate the outpins

protected:
  PID::Base *m_pid;
  PID::States m_state;
  errorTypes m_errType;
};


/**
 * Ordinary input
 */
class AnalogIn : public IInput
{
public:
  AnalogIn(uint8_t inPin, uint16_t minVlu, uint16_t maxVlu, uint8_t slowFactor, PID::Base *pid);
  ~AnalogIn();
  void init();
  void setReversed(bool reversed) { m_reversed = reversed; } // reversed -> when pot down gives increasing value
  bool reversed() const { return m_reversed; }
  void interval();

protected:
  bool m_reversed;
  uint8_t m_inPin;
  uint8_t m_factor;
  uint16_t m_maxVlu;  // used to make sure we have a valid reading
  uint16_t m_minVlu;  // else we set an error
};

/*
class TempIn : public AnalogIn
{
  Ntc *m_ntcObj;
public:
  TempIn(uint8_t inPin, uint16_t minVlu, uint16_t maxVlu, uint8_t slowFactor, Ntc* ntcObj, Adresses adress);
  ~TempIn();
  float temperature() const;
};
*/

void init_inputs();

extern List<IInput> InputsController;
extern IntervalTimer inputTmr;

#endif /* INPUTS_H_ */
