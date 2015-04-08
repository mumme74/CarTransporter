/*
 * Height.h
 *
 *  Created on: 6 apr 2015
 *      Author: jof
 */

#ifndef HEIGHT_H_
#define HEIGHT_H_

#include <PID.h>
#include <PidLoop.h>
#include <Outputs.h>
#include <Inputs.h>
#include <storage.h>

/**
 * This is the state machine that controls the suspension height
 *
 * It also implements the failsafe methods
 *
 */
class HeightController
{
  PID::State *m_statePid;
  PID::Base *m_weightPid;
  IOutput
    *m_leftFillDrv,
    *m_leftDumpDrv,
    *m_leftSuckDrv,
    *m_rightFillDrv,
    *m_rightDumpDrv,
    *m_rightSuckDrv;

  AnalogIn
    *m_leftPressureDrv,
    *m_rightPressureDrv,
    *m_systemPressureDrv,
    *m_leftHeightDrv,
    *m_rightHeightDrv;

  bool m_slowSample,
       m_sucked;     // if rear wheels are lifted

  uint16_t
    m_leftLowSetpoint,
    m_leftNormalSetpoint,
    m_leftHighSetpoint,
    m_rightLowSetpoint,
    m_rightNormalSetpoint,
    m_rightHighSetpoint,
    m_deadWeight,       // the chassis dead weight
    m_deadBand;         // how many % of total throw that we wont regulate height within

  float
    m_leftInputVar,
    m_rightInputVar,
    m_leftOutputVar,
    m_rightOutputVar,
    m_leftSetpointVar,
    m_rightSetpointVar,
    m_p, m_i, m_d, // the pid values
    m_cylArea;  // the bellows (cylinders) area, calculated from cyl dia. in settings


  PidLoop
    m_leftPidLoop,
    m_rightPidLoop;

public:
  enum Configs : uint8_t {
      KP_factor_float, KI_factor_float, KD_factor_float,
      LeftLowSetpoint_uint16, LeftNormalSetpoint_uint16, LeftHighSetpoint_uint16,
      RightLowSetpoint_uint16, RightNormalSetpoint_uint16, RightHighSetpoint_uint16,
      DeadWeight_uint16, DeadBand_uint16,
  };

  HeightController(IOutput *leftFillDrv,  IOutput *leftDumpDrv,  IOutput *leftSuckDrv,
                   IOutput *rightFillDrv, IOutput *rightDumpDrv, IOutput *rightSuckDrv,
                   AnalogIn *leftPressureDrv,   AnalogIn *rightPressureDrv,
                   AnalogIn *systemPressureDrv, AnalogIn *leftHeightDrv,
                   AnalogIn *rightHeightDrv,    PID::State *heightControllerPID,
                   PID::Base *loadWeigth_PID);
  ~HeightController ();

  PID::State *pid() const { return m_statePid; }
  void setState(PID::States state);
  bool setRearWheels(bool lift);
  bool rearWheels() const { return m_sucked; }

  void setCylDia(uint16_t cylDiaIn_mm); // changes in settings the diameter of the air bellows
  void setConfig(Configs cfg, store::byte4 value);
  void saveSettings(); // stores settings in eeprom
  store::byte4 getConfig(Configs cfg);

  void init();
  void loop();

private:
  void resetPidLoop();
  void readSettings();
  void updateInputs();
};

#endif /* HEIGHT_H_ */
