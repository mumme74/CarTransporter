/*
 * PidLoop.h
 *
 *  Created on: 6 apr 2015
 *      Author: jof
 */

#ifndef PIDLOOP_H_
#define PIDLOOP_H_

#include <stdint.h>

// this one comes from
// https://github.com/br3ttb/Arduino-PID-Library/blob/master/PID_v1/PID_v1.h
// Imported here to avoid namespace clasches

class PidLoop
{
public:

  //Constants used in some of the functions below
  enum pidModes : uint8_t {
    Automatic = 1,
    Manual = 0
  };
  enum pidDirections : uint8_t {
    Direct = 0,
    Reverse = 1
  };

  //commonly used functions **************************************************************************
    PidLoop(float &inputVar, float &outputVar, float &setpointVar,
            float Kp, float Ki, float Kd, pidDirections direction);     //   Setpoint.  Initial tuning parameters are also set here

    void setMode(pidModes mode);               // * sets PID to either Manual (0) or Auto (non-0)

    bool compute();                       // * performs the PID calculation.  it should be
                                          //   called every time loop() cycles. ON/OFF and
                                          //   calculation frequency can be set using SetMode
                                          //   SetSampleTime respectively

    void setOutputLimits(float, float); //clamps the output to a specific range. 0-255 by default, but
                                          //it's likely the user will want to change this depending on
                                          //the application



  //available but not commonly used functions ********************************************************
    void setTunings(float, float,       // * While most users will set the tunings once in the
                    float);              //   constructor, this function gives the user the option
                                          //   of changing tunings during runtime for Adaptive control

    // * Sets the Direction, or "Action" of the controller. DIRECT
    //   means the output will increase when error is positive. REVERSE
    //   means the opposite.  it's very unlikely that this will be needed
    //   once it is set in the constructor.
    void setControllerDirection(pidDirections direction);

    void setSampleTime(int);              // * sets the frequency, in Milliseconds, with which
                                          //   the PID calculation is performed.  default is 100



  //Display functions ****************************************************************
    float getKp() const;                       // These functions query the pid for integral values.
    float getKi() const;                       //  they were created mainly for the pid front-end,
    float getKd() const;                       // where it's important to know what is actually
    pidModes      getMode() const;                        //  inside the PID.
    pidDirections getDirection() const;                   //

  private:
    void init();

    float m_dispKp;              // * we'll hold on to the tuning parameters in user-entered
    float m_dispKi;              //   format for display purposes
    float m_dispKd;              //

    float m_kp;                  // * (P)roportional Tuning Parameter
    float m_ki;                  // * (I)ntegral Tuning Parameter
    float m_kd;                  // * (D)erivative Tuning Parameter

    pidDirections m_direction;

    float *m_inputVar;              // * Pointers to the Input, Output, and Setpoint variables
    float *m_outputVar;             //   This creates a hard link between the variables and the
    float *m_setpointVar;           //   PID, freeing the user from having to constantly tell us
                                  //   what these values are.  with pointers we'll just know.

    unsigned long m_lastTime;
    float m_ITerm, m_lastInput;

    unsigned long m_sampleTime;
    float m_outMin, m_outMax;
    bool m_inAuto;
};

#endif /* PIDLOOP_H_ */
