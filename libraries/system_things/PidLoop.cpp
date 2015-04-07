/*
 * PidLoop.cpp
 *
 *  Created on: 6 apr 2015
 *      Author: jof
 */

#include "PidLoop.h"

#include <Arduino.h>

/*Constructor (...)*********************************************************
 *    The parameters specified here are those for for which we can't set up
 *    reliable defaults, so we need to have the user set them.
 ***************************************************************************/
PidLoop::PidLoop(float &inputVar, float &outputVar, float &setpointVar,
        float Kp, float Ki, float Kd, pidDirections direction)
{

    m_outputVar = &outputVar;
    m_inputVar = &inputVar;
    m_setpointVar = &setpointVar;
    m_inAuto = false;

    PidLoop::setOutputLimits(0, 255);               //default output limit corresponds to
                                                //the arduino pwm limits

    m_sampleTime = 100;                           //default Controller Sample Time is 0.1 seconds

    PidLoop::setControllerDirection(direction);
    PidLoop::setTunings(Kp, Ki, Kd);

    m_lastTime = millis() - m_sampleTime;
}


/* Compute() **********************************************************************
 *     This, as they say, is where the magic happens.  this function should be called
 *   every time "void loop()" executes.  the function will decide for itself whether a new
 *   pid Output needs to be computed.  returns true when the output is computed,
 *   false when nothing has been done.
 **********************************************************************************/
bool PidLoop::compute()
{
   if(!m_inAuto) return false;
   unsigned long now = millis();
   unsigned long timeChange = (now - m_lastTime);
   if(timeChange >= m_sampleTime)
   {
      /*Compute all the working error variables*/
      float input = *m_inputVar;
      float error = *m_setpointVar - input;
      m_ITerm += (m_ki * error);
      if(m_ITerm > m_outMax)
        m_ITerm = m_outMax;
      else if(m_ITerm < m_outMin)
        m_ITerm= m_outMin;
      float dInput = (input - m_lastInput);

      /*Compute PID Output*/
      float output = m_kp * error + m_ITerm - m_kd * dInput;

      if(output > m_outMax)
        output = m_outMax;
      else if(output < m_outMin)
        output = m_outMin;
      *m_outputVar = output;

      /*Remember some variables for next time*/
      m_lastInput = input;
      m_lastTime = now;
      return true;
   }
   else return false;
}


/* SetTunings(...)*************************************************************
 * This function allows the controller's dynamic performance to be adjusted.
 * it's called automatically from the constructor, but tunings can also
 * be adjusted on the fly during normal operation
 ******************************************************************************/
void PidLoop::setTunings(float Kp, float Ki, float Kd)
{
   if (Kp<0 || Ki<0 || Kd<0) return;

   m_dispKp = Kp; m_dispKi = Ki; m_dispKd = Kd;

   float SampleTimeInSec = ((float)m_sampleTime)/1000;
   m_kp = Kp;
   m_ki = Ki * SampleTimeInSec;
   m_kd = Kd / SampleTimeInSec;

  if(m_direction == pidDirections::Reverse) {
      m_kp = (0 - m_kp);
      m_ki = (0 - m_ki);
      m_kd = (0 - m_kd);
   }
}

/* SetSampleTime(...) *********************************************************
 * sets the period, in Milliseconds, at which the calculation is performed
 ******************************************************************************/
void PidLoop::setSampleTime(int newSampleTime)
{
   if (newSampleTime > 0)
   {
      float ratio  = (float)newSampleTime
                      / (float)m_sampleTime;
      m_ki *= ratio;
      m_kd /= ratio;
      m_sampleTime = (unsigned long)newSampleTime;
   }
}

/* SetOutputLimits(...)****************************************************
 *     This function will be used far more often than SetInputLimits.  while
 *  the input to the controller will generally be in the 0-1023 range (which is
 *  the default already,)  the output will be a little different.  maybe they'll
 *  be doing a time window and will need 0-8000 or something.  or maybe they'll
 *  want to clamp it from 0-125.  who knows.  at any rate, that can all be done
 *  here.
 **************************************************************************/
void PidLoop::setOutputLimits(float Min, float Max)
{
   if(Min >= Max) return;
   m_outMin = Min;
   m_outMax = Max;

   if(m_inAuto)
   {
       if(*m_outputVar > m_outMax) {
         *m_outputVar = m_outMax;
       } else if(*m_outputVar < m_outMin) {
         *m_outputVar = m_outMin;
       }

       if(m_ITerm > m_outMax) {
           m_ITerm= m_outMax;
       } else if(m_ITerm < m_outMin) {
           m_ITerm= m_outMin;
       }
   }
}

/* SetMode(...)****************************************************************
 * Allows the controller Mode to be set to manual (0) or Automatic (non-zero)
 * when the transition from manual to auto occurs, the controller is
 * automatically initialized
 ******************************************************************************/
void PidLoop::setMode(pidModes mode)
{
    bool newAuto = (mode == pidModes::Automatic);
    if(newAuto == !m_inAuto)
    {  /*we just went from manual to auto*/
        PidLoop::init();
    }
    m_inAuto = newAuto;
}

/* Initialize()****************************************************************
 *  does all the things that need to happen to ensure a bumpless transfer
 *  from manual to automatic mode.
 ******************************************************************************/
void PidLoop::init()
{
   m_ITerm = *m_outputVar;
   m_lastInput = *m_inputVar;
   if(m_ITerm > m_outMax) {
       m_ITerm = m_outMax;
   } else if(m_ITerm < m_outMin) {
       m_ITerm = m_outMin;
   }
}

/* SetControllerDirection(...)*************************************************
 * The PID will either be connected to a DIRECT acting process (+Output leads
 * to +Input) or a REVERSE acting process(+Output leads to -Input.)  we need to
 * know which one, because otherwise we may increase the output when we should
 * be decreasing.  This is called from the constructor.
 ******************************************************************************/
void PidLoop::setControllerDirection(pidDirections direction)
{
   if(m_inAuto && direction != m_direction)
   {
      m_kp = (0 - m_kp);
      m_ki = (0 - m_ki);
      m_kd = (0 - m_kd);
   }
   m_direction = direction;
}

/* Status Funcions*************************************************************
 * Just because you set the Kp=-1 doesn't mean it actually happened.  these
 * functions query the internal state of the PID.  they're here for display
 * purposes.  this are the functions the PID Front-end uses for example
 ******************************************************************************/
float PidLoop::getKp() const { return  m_dispKp; }
float PidLoop::getKi() const { return  m_dispKi;}
float PidLoop::getKd() const { return  m_dispKd;}

PidLoop::pidModes      PidLoop::getMode() const { return  m_inAuto ? Automatic : Manual;}
PidLoop::pidDirections PidLoop::getDirection() const { return m_direction; }
