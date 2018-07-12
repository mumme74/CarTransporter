/*
 * Height.cpp
 *
 *  Created on: 6 apr 2015
 *      Author: jof
 */

#include "Height.h"
#include <PID.h>
#include <storage.h>
#include <EEPROM.h>

static const float
  defaultP = 2.0,
  defaultI = 1.0,
  defaultD = 1.0;

static const uint16_t
  defaultLowPoint    = 200,
  defaultNormalPoint = 1500,
  defaultHighPoint   = 3000,
  defaultCylArea     = 7800,  // 50 *50 *3.14
  defaultDeadWeight  = 200,   // 200 kg dead weight
  defaultDeadBand    = 5;     // how many % of total throw that is within limits and we wont regulate outputs


HeightController::HeightController(
    IOutput *leftFillDrv,  IOutput *leftDumpDrv,  IOutput *leftSuckDrv,
    IOutput *rightFillDrv, IOutput *rightDumpDrv, IOutput *rightSuckDrv,
    AnalogIn *leftPressureDrv,   AnalogIn *rightPressureDrv,
    AnalogIn *systemPressureDrv, AnalogIn *leftHeightDrv,
    AnalogIn *rightHeightDrv,    PID::State *heightControllerPID,
    PID::Base *loadWeigth_PID) :
   m_statePid(heightControllerPID),
   m_weightPid(loadWeigth_PID),
   m_leftFillDrv(leftFillDrv),
   m_leftDumpDrv(leftDumpDrv),
   m_leftSuckDrv(leftSuckDrv),
   m_rightFillDrv(rightFillDrv),
   m_rightDumpDrv(rightDumpDrv),
   m_rightSuckDrv(rightSuckDrv),
   m_leftPressureDrv(leftPressureDrv),
   m_rightPressureDrv(rightPressureDrv),
   m_systemPressureDrv(systemPressureDrv),
   m_leftHeightDrv(leftHeightDrv),
   m_rightHeightDrv(rightHeightDrv),
   m_slowSample(false),
   m_sucked(false),
   m_leftLowSetpoint(defaultLowPoint),
   m_leftNormalSetpoint(defaultNormalPoint),
   m_leftHighSetpoint(defaultHighPoint),
   m_rightLowSetpoint(defaultLowPoint),
   m_rightNormalSetpoint(defaultNormalPoint),
   m_rightHighSetpoint(defaultHighPoint),
   m_deadWeight(defaultDeadWeight),
   m_deadBand(defaultDeadBand),
   m_leftInputVar(100),
   m_rightInputVar(100),
   m_leftOutputVar(100),
   m_rightOutputVar(100),
   m_leftSetpointVar(100),
   m_rightSetpointVar(100),
   m_p(defaultP), m_i(defaultI), m_d(defaultD),
   m_cylArea_m2(defaultCylArea),
   m_leftPidLoop (m_leftInputVar, m_leftOutputVar, m_leftSetpointVar,
                  m_p, m_i, m_d, PidLoop::pidDirections::Direct),
   m_rightPidLoop(m_rightInputVar, m_rightOutputVar, m_rightSetpointVar,
                  m_p, m_i, m_d, PidLoop::pidDirections::Direct)
{
  m_leftPidLoop.setOutputLimits(-100.0, 100.0);
  m_rightPidLoop.setOutputLimits(-100.0, 100.0);
  m_leftPidLoop.setSampleTime(50); // 50ms
  m_rightPidLoop.setSampleTime(50); // 50ms
}

HeightController::~HeightController ()
{
}

void HeightController::loop()
{
  updateInputs();

  static bool
    leftOnPoint = true,
    rightOnPoint = true;

  // pidLoop updates outputVar automatically
  if (m_rightPidLoop.compute()) {
    // allow for some dead band
    if (m_rightOutputVar > m_rightInputVar + m_deadBand ||
        m_rightOutputVar < m_rightInputVar - m_deadBand)
    {
      leftOnPoint = false;
      if (m_rightOutputVar < 0.0)
        m_rightFillDrv->setValue(static_cast<uint8_t>(m_rightOutputVar * -1)); // reverse value as it is negative
      else
        m_rightDumpDrv->setValue(static_cast<uint8_t>(m_rightOutputVar));
    } else {
        leftOnPoint = true;
    }
  }

  if (m_leftPidLoop.compute()) {
    // left side
    if (m_leftOutputVar > m_leftInputVar + m_deadBand ||
         m_leftOutputVar < m_leftInputVar - m_deadBand)
    {
      rightOnPoint = false;
      if (m_leftOutputVar < 0.0)
        m_leftFillDrv->setValue(static_cast<uint8_t>(m_leftOutputVar * -1)); // reverse value as it is negative
      else
        m_leftDumpDrv->setValue(static_cast<uint8_t>(m_leftOutputVar));
    } else {
        rightOnPoint = true;
    }
  }

  // we reached our setpoints
  if (leftOnPoint && rightOnPoint) {
      switch (m_statePid->state()) {
        case PID::States::ToLowState:
          m_statePid->setState(PID::States::LowState);
          m_statePid->setUpdated(true);
          break;
        case PID::States::ToNormalState:
          m_statePid->setState(PID::States::NormalState);
          m_statePid->setUpdated(true);
          break;
        case PID::ToHighState:
          m_statePid->setState(PID::States::HighState);
          m_statePid->setUpdated(true);
          break;
        default: break;
      }
  }
}

can_userError_e HeightController::setState(PID::States state)
{
  if (state != m_statePid->state()) {
      switch(state){
        case PID::States::ToLowState:
          m_leftSetpointVar = m_leftLowSetpoint;
          m_rightSetpointVar = m_rightLowSetpoint;
          m_statePid->setState(PID::States::ToLowState);
          m_statePid->setUpdated(true);
          break;
        case PID::States::ToNormalState:
          m_leftSetpointVar = m_leftNormalSetpoint;
          m_rightSetpointVar = m_rightNormalSetpoint;
          m_statePid->setState(PID::States::ToNormalState);
          m_statePid->setUpdated(true);
          break;
        case PID::States::ToHighState:
          m_leftSetpointVar = m_leftHighSetpoint;
          m_rightSetpointVar = m_rightHighSetpoint;
          m_statePid->setState(PID::States::ToHighState);
          m_statePid->setUpdated(true);
          break;
        default:
            return C_userErrorHeightNonValidState;
      }
  }
  return C_userErrorNone;
}

can_userError_e HeightController::setRearWheels(bool lift)
{
  if (m_statePid->state() == PID::States::LowState) {
      m_sucked = lift;
      uint8_t vlu = 0;
      if (m_sucked) {
          vlu = 100;
      }

      m_leftSuckDrv->setValue(vlu);
      m_rightSuckDrv->setValue(vlu);
      return C_userErrorNone;
  }

  return C_userErrorSuckedRearWheelBlocked;
}

int HeightController::setConfig(Configs cfg, store::byte4 value)
{
  switch (cfg) {
    case Configs::KP_factor_float: {
      m_p = value.decimal;
      store::write4_to_eeprom(store::Suspension::HEIGHT_KP_ADR, value);
    } break;
    case Configs::KI_factor_float: {
      m_i = value.decimal;
      store::write4_to_eeprom(store::Suspension::HEIGHT_KI_ADR, value);
    } break;
    case Configs::KD_factor_float: {
      m_d = value.decimal;
      store::write4_to_eeprom(store::Suspension::HEIGHT_KD_ADR, value);
    } break;
    case Configs::LeftLowSetpoint_uint16: {
      m_leftLowSetpoint = static_cast<uint16_t>(value.uint32);
      store::byte2 b2;
      store::write2_to_eeprom(store::Suspension::HEIGHT_LEFT_LOW_ADR, b2);
    } break;
    case Configs::LeftNormalSetpoint_uint16: {
      m_leftNormalSetpoint = static_cast<uint16_t>(value.uint32);
      store::byte2 b2;
      store::write2_to_eeprom(store::Suspension::HEIGHT_LEFT_NORMAL_ADR, b2);
    } break;
    case Configs::LeftHighSetpoint_uint16: {
      m_leftHighSetpoint = static_cast<uint16_t>(value.uint32);
      store::byte2 b2;
      store::write2_to_eeprom(store::Suspension::HEIGHT_LEFT_HIGH_ADR, b2);
    } break;
    case Configs::RightLowSetpoint_uint16: {
      m_rightLowSetpoint = static_cast<uint16_t>(value.uint32);
      store::byte2 b2;
      store::write2_to_eeprom(store::Suspension::HEIGHT_RIGHT_LOW_ADR, b2);
    } break;
    case Configs::RightNormalSetpoint_uint16: {
      m_rightNormalSetpoint = static_cast<uint16_t>(value.uint32);
      store::byte2 b2;
      store::write2_to_eeprom(store::Suspension::HEIGHT_RIGHT_NORMAL_ADR, b2);
    } break;
    case Configs::RightHighSetpoint_uint16: {
      m_rightHighSetpoint = static_cast<uint16_t>(value.uint32);
      store::byte2 b2;
      store::write2_to_eeprom(store::Suspension::HEIGHT_RIGHT_HIGH_ADR, b2);
    } break;
    case Configs::DeadWeight_uint16: {
      m_deadWeight = static_cast<uint16_t>(value.uint32);
      store::byte2 b2;
      store::write2_to_eeprom(store::Suspension::HEIGHT_DEAD_WEIGHT_ADR, b2);
    } break;
    case Configs::DeadBand_uint16: {
      m_deadBand = static_cast<uint16_t>(value.uint32);
      store::byte2 b2;
      store::write2_to_eeprom(store::Suspension::HEIGHT_DEAD_BAND_ADR, b2);
    } break;
    case Configs::CylDia_mm: {
      store::byte2 b2;
      b2.uint16 = static_cast<uint16_t>(value.uint32);
      store::write2_to_eeprom(store::Suspension::HEIGHT_CYL_DIA_MM_ADR, b2);
      calcCylArea(); // update weight calc params

    }  break;
    case Configs::LeftHeightSensorReversed: {
    	uint8_t curVlu = EEPROM.read(store::Suspension::HEIGHT_SENSORS_REVERSED_ADR);
    	uint8_t	newVlu = curVlu & 0xFE; // save all but bit0
    	newVlu &= value.buf[0] & 0x01;  // set the left sensor bit
    	EEPROM.write(store::Suspension::HEIGHT_SENSORS_REVERSED_ADR, newVlu);
    	m_leftHeightDrv->setReversed(value.buf[0] & 0x01);

    } break;
    case Configs::RightHeightSensorReversed: {
    	uint8_t curVlu = EEPROM.read(store::Suspension::HEIGHT_SENSORS_REVERSED_ADR);
    	uint8_t	newVlu = curVlu & 0xFD; // save all but bit1
    	newVlu &= value.buf[0] & 0x02;  // set the right sensor bit
    	EEPROM.write(store::Suspension::HEIGHT_SENSORS_REVERSED_ADR, newVlu);
    	m_rightHeightDrv->setReversed(value.buf[0] & 0x02);

    } break;
    default:
        return 0; // ignore
  }
  return 1;
}

/*
void HeightController::saveSettings()
{
  store::byte2 b2;
  b2.uint16 = m_leftLowSetpoint;
  store::write2_to_eeprom(store::Suspension::HEIGHT_LEFT_LOW_ADR, b2);
  b2.uint16 = m_leftNormalSetpoint;
  store::write2_to_eeprom(store::Suspension::HEIGHT_LEFT_NORMAL_ADR, b2);
  b2.uint16 = m_leftHighSetpoint;
  store::write2_to_eeprom(store::Suspension::HEIGHT_LEFT_HIGH_ADR, b2);
  b2.uint16 = m_rightLowSetpoint;
  store::write2_to_eeprom(store::Suspension::HEIGHT_RIGHT_LOW_ADR, b2);
  b2.uint16 = m_rightNormalSetpoint;
  store::write2_to_eeprom(store::Suspension::HEIGHT_RIGHT_NORMAL_ADR, b2);
  b2.uint16 = m_rightHighSetpoint;
  store::write2_to_eeprom(store::Suspension::HEIGHT_RIGHT_HIGH_ADR, b2);
  b2.uint16 = m_deadWeight;
  store::write2_to_eeprom(store::Suspension::HEIGHT_DEAD_WEIGHT_ADR, b2);
  b2.uint16 = m_deadBand;
  store::write2_to_eeprom(store::Suspension::HEIGHT_DEAD_BAND_ADR, b2);

  // NOTE store::Suspension::HEIGHT_CYL_DIA_MM_ADR is not saved here,
  // we need to special case this one as it has no live value stored on RAM
  // only used in calculation for Cyl area

  store::byte4 b4;
  b4.decimal = m_p;
  store::write4_to_eeprom(store::Suspension::HEIGHT_KP_ADR, b4);
  b4.decimal = m_i;
  store::write4_to_eeprom(store::Suspension::HEIGHT_KI_ADR, b4);
  b4.decimal = m_d;
  store::write4_to_eeprom(store::Suspension::HEIGHT_KD_ADR, b4);
}
*/

store::byte4 HeightController::getConfig(Configs cfg)
{
  store::byte4 value;
  switch (cfg) {
    case Configs::KP_factor_float:
      value.decimal = m_p;
      resetPidLoop();
      break;
    case Configs::KI_factor_float:
      value.decimal = m_i;
      resetPidLoop();
      break;
    case Configs::KD_factor_float:
      value.decimal = m_d;
      resetPidLoop();
      break;
    case Configs::LeftLowSetpoint_uint16:
      value.uint32 = m_leftLowSetpoint;
      break;
    case Configs::LeftNormalSetpoint_uint16:
      value.uint32 = m_leftNormalSetpoint;
      break;
    case Configs::LeftHighSetpoint_uint16:
      value.uint32 = m_leftHighSetpoint;
      break;
    case Configs::RightLowSetpoint_uint16:
      value.uint32 = m_rightLowSetpoint;
      break;
    case Configs::RightNormalSetpoint_uint16:
      value.uint32 = m_rightNormalSetpoint;
      break;
    case Configs::RightHighSetpoint_uint16:
      value.uint32 = m_rightHighSetpoint;
      break;
    case Configs::DeadWeight_uint16:
      value.uint32 = m_deadWeight;
      break;
    case Configs::DeadBand_uint16:
      value.uint32 = m_deadBand;
      break;
    case Configs::CylDia_mm:
      value.uint32 = store::read2_from_eeprom(store::Suspension::HEIGHT_CYL_DIA_MM_ADR).uint16;
      break;
    case Configs::LeftHeightSensorReversed:
    	value.buf[0] = EEPROM.read(store::Suspension::HEIGHT_SENSORS_REVERSED_ADR) & 0x01;
    	break;
    case Configs::RightHeightSensorReversed:
    	value.buf[0] = (EEPROM.read(store::Suspension::HEIGHT_SENSORS_REVERSED_ADR) & 0x02) >> 1;
    	break;
    default: ;
      value.uint32 = 0;
  }

  return value;
}

void HeightController::init()
{
  readSettings();
  resetPidLoop();
}

void HeightController::resetPidLoop()
{
  m_leftPidLoop.setTunings(m_p, m_i, m_d);
  m_rightPidLoop.setTunings(m_p, m_i, m_d);
  m_leftPidLoop.setMode(PidLoop::pidModes::Automatic);
  m_rightPidLoop.setMode(PidLoop::pidModes::Automatic);
}

void HeightController::readSettings()
{
  m_leftLowSetpoint     = store::read2_from_eeprom(store::Suspension::HEIGHT_LEFT_LOW_ADR).uint16;
  m_leftNormalSetpoint  = store::read2_from_eeprom(store::Suspension::HEIGHT_LEFT_NORMAL_ADR).uint16;
  m_leftHighSetpoint    = store::read2_from_eeprom(store::Suspension::HEIGHT_LEFT_HIGH_ADR).uint16;
  m_rightLowSetpoint    = store::read2_from_eeprom(store::Suspension::HEIGHT_RIGHT_LOW_ADR).uint16;
  m_rightNormalSetpoint = store::read2_from_eeprom(store::Suspension::HEIGHT_RIGHT_NORMAL_ADR).uint16;
  m_rightHighSetpoint   = store::read2_from_eeprom(store::Suspension::HEIGHT_RIGHT_HIGH_ADR).uint16;

  if (m_leftLowSetpoint == m_leftNormalSetpoint && m_leftLowSetpoint == m_leftHighSetpoint &&
      m_rightLowSetpoint == m_rightNormalSetpoint && m_rightLowSetpoint == m_rightHighSetpoint)
  {
      // first use no defaults
      m_leftLowSetpoint = defaultLowPoint;
      m_leftNormalSetpoint = defaultNormalPoint;
      m_leftHighSetpoint = defaultHighPoint;
      m_rightLowSetpoint = defaultLowPoint;
      m_rightNormalSetpoint = defaultNormalPoint;
      m_rightHighSetpoint = defaultHighPoint;
  }

  // use some defaults if values are 0
  float tmp = store::read4_from_eeprom(store::Suspension::HEIGHT_KD_ADR).decimal;
  if (tmp != defaultP && tmp != 0.0)
    m_p = tmp;
  tmp = store::read4_from_eeprom(store::Suspension::HEIGHT_KI_ADR).decimal;
  if (tmp != defaultI && tmp != 0.0)
    m_i = tmp;
  tmp = store::read4_from_eeprom(store::Suspension::HEIGHT_KP_ADR).decimal;
  if (tmp != defaultD && tmp != 0.0)
    m_d = tmp;


  m_deadWeight = store::read2_from_eeprom(store::Suspension::HEIGHT_DEAD_WEIGHT_ADR).uint16;
  if (m_deadWeight == 0)
    m_deadWeight = defaultDeadWeight;

  m_deadBand = store::read2_from_eeprom(store::Suspension::HEIGHT_DEAD_BAND_ADR).uint16;
  if (m_deadBand == 0)
      m_deadBand = defaultDeadBand;

  uint8_t revByte = EEPROM.read(store::Suspension::HEIGHT_SENSORS_REVERSED_ADR);
  if (revByte & 0x01)
	  m_leftHeightDrv->setReversed(true);
  if (revByte & 0x02)
	  m_rightHeightDrv->setReversed(true);

  calcCylArea();
}

void HeightController::calcCylArea()
{
    // calculate cylinder area
    uint16_t dia = store::read2_from_eeprom(store::Suspension::HEIGHT_CYL_DIA_MM_ADR).uint16;
    if (dia == 0)
      dia = defaultCylArea;
    dia /= 2;
    m_cylArea_m2 = (dia * dia * 3.14159264) / 1000000; // area in mm to area in m
}


void HeightController::updateInputs()
{
  static const uint8_t sampleTime = 20; // 20 ms between samples
  static uint32_t nextCheck = millis() + sampleTime;

  if (nextCheck < millis()){
      nextCheck = millis() + sampleTime;

#ifdef DEBUG_PRESSURES_ON
      Serial.print("lr:");Serial.print(m_leftHeightDrv->pid()->rawValue());
      Serial.print(" l:");Serial.print(static_cast<PID::sensor_Pressure*>(m_leftPressureDrv->pid())->pressureKPa());
      Serial.print(" r:");Serial.print(static_cast<PID::sensor_Pressure*>(m_rightPressureDrv->pid())->pressureKPa());
      Serial.print(" system:");Serial.println(static_cast<PID::sensor_Pressure*>(m_systemPressureDrv->pid())->pressureKPa());
#endif

    // do averaging values before we set it
    static uint32_t
      leftAvg = 0,
      rightAvg = 0,
      leftPressureAvg = 0,
      rightPressureAvg = 0;
    static uint8_t sampleCnt = 0;

    leftPressureAvg += m_leftPressureDrv->pid()->rawValue();
    rightPressureAvg += m_rightPressureDrv->pid()->rawValue();

    // set heights failsafe
    if (m_leftHeightDrv->error() == errorTypes::NoError &&
        m_rightHeightDrv->error() == errorTypes::NoError)
    {
        leftAvg  += m_leftHeightDrv->pid()->rawValue();
        rightAvg += m_rightHeightDrv->pid()->rawValue();
    } else if (m_rightHeightDrv->error() == errorTypes::NoError) {
        // left sensor broken
        leftAvg  += m_rightHeightDrv->pid()->rawValue();
        rightAvg += m_rightHeightDrv->pid()->rawValue();
    } else if (m_leftHeightDrv->error() == errorTypes::NoError) {
        // right sensor broken
        leftAvg  += m_leftHeightDrv->pid()->rawValue();
        rightAvg += m_leftHeightDrv->pid()->rawValue();
    } else if (m_statePid->state() == PID::States::NormalState ||
               m_statePid->state() == PID::States::NormalState)
    {
        // both sensors broken default to read from cylinders
        // hopefully the normal position will be somewhat sensible
        // even if we read from the wrong sensor
        leftAvg += m_leftPressureDrv->pid()->rawValue();
        rightAvg += m_rightPressureDrv->pid()->rawValue();
    }

    ++sampleCnt;

    // update the input values
    static const int maxSlowSampleCnt = 2000 / sampleTime;
    if ((!m_slowSample && sampleCnt > 0) || // always sample when in fast mode
        sampleCnt >= maxSlowSampleCnt)
    {
      leftAvg /= sampleCnt;
      m_leftInputVar = leftAvg; // PidLoop read from m_leftInputVar automatically

      rightAvg /= sampleCnt;
      m_rightInputVar = rightAvg;

      leftPressureAvg /= sampleCnt;
      rightPressureAvg /= sampleCnt;

      // calculate Weight
      if (m_statePid->state() == PID::States::NormalState) {
          // average 2 pressures in calculation
          uint16_t pressureKPa = static_cast<PID::sensor_Pressure*>(m_leftPressureDrv->pid())->pressureKPa() +
                              static_cast<PID::sensor_Pressure*>(m_rightPressureDrv->pid())->pressureKPa();
          pressureKPa /= 2; // mean value of both cyls

          int nCyls = 4;
          if (m_sucked) { // rear wheels lifted
              nCyls = 2;
          }

          // F = p * A (A=in m2)
          float weight = pressureKPa * (m_cylArea_m2 * nCyls);
          weight /= 0.980665; // newtons to kg

          if (weight > m_deadWeight) { // avoid overflow due lighter chassi than stored deadweight
            weight -= m_deadWeight;
          } else
            weight = 0;

          m_weightPid->setRawValue(static_cast<uint16_t>(weight));
          m_weightPid->setUpdated(true);

      } else if (m_weightPid->rawValue() > 0){
          // set weight to 0 when we aren't at rideheight
          m_weightPid->setRawValue(0);
          m_weightPid->setUpdated(true);
      }

      sampleCnt = 0;
    }
  }
}
