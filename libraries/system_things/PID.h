/*
 * PID.h
 *
 *  Created on: 4 apr 2015
 *      Author: jof
 */

#ifndef PID_H_
#define PID_H_

#include <List.h>
#include <Ntc.h>


namespace PIDs {
  enum IDs : uint16_t;
};


namespace PID {

/**
 * Different states that can occur
 * is a bitmask so several states can happen at once if needed
 */
enum States : uint16_t {
  Off              = 0x0000,
  On               = 0x0001,
  ActuatorTest     = 0x0002, // for diagnosis

  Working          = 0x0004, // when a input works

  RevUp            = 0x0008, // when a motor revs up
  FullRev          = 0x0010, // full speed of motor
  Airdryer         = 0x0020, // Airdryer activated
  RevDown          = 0x0040, // Motor revs down

  LowState         = 0x0080, // Suspension at lowest state
  ToLowState       = 0x0100, // Suspension going to lowest state
  NormalState      = 0x0200, // Suspension at normal state
  ToNormalState    = 0x0400, // Suspension going to normal state
  HighState        = 0x0800, // suspension at highest state
  ToHighState      = 0x1000, // suspension going to highest state

  Error            = 0x2000, // an error has occured
};

enum Types :uint16_t {
  input_RawData     = 0x0000,
  input_Digital     = 0x0001,
  input_Steps       = 0x0002,
  input_Volts       = 0x0004,
  input_Pressure    = 0x0008,
  input_Temperature = 0x0010,
  input_Current     = 0x0020,

  input_Clone       = 0x0080, // used to create another PID, ie. reads volts from steps PID

  output_RawData    = 0x0100,
  output_Digital    = 0x0200,
  output_Duty       = 0x0400,
  output_Steps      = 0x0800,

  // used for statemachines
  states            = 0x1000,
};

enum byteSizes : uint8_t {
  oneByte,
  twoByte,
  fourByte,
};

union PidVlu {
  float decimal;
  uint32_t uint32;
  uint16_t uint16;
  uint8_t  uint8;
  bool boolean;
};

class Base {
public:
  Base (PIDs::IDs id, Types availableTypes, Types preferedType, byteSizes size);
  virtual ~Base() {}
  inline Types types() const { return m_availableTypes; }
  inline Types preferedType() const { return m_preferedType; }
  inline PIDs::IDs id() const { return m_id; }
  inline bool     digital() const { return m_vlu.uint32 != 0; }
  inline virtual uint16_t rawValue() const { return m_vlu.uint16; }
  void   virtual setRawValue(uint16_t value);

  inline uint32_t rawData32() const { return m_vlu.uint32; }
  void setRawData32(uint32_t data);

  inline byteSizes size() const { return m_byteSize; }
  inline void setUpdated(bool update) { m_updated = update; } // should only be called by the node that "owns" this pid
  inline bool updated() { return m_updated; }

  bool     handlesType(Types type) const { return m_availableTypes & type; }
  States    valueAsState(bool &ok);
  float    valueAsFloat(Types type, bool &ok);
  uint16_t valueAsUInt16(Types type, bool &ok);
  uint8_t  valueAsUInt8(Types type, bool &ok);
  bool     valueAsBool(Types type, bool &ok) const;

  Base *next; // used internally for iteration

protected:
  byteSizes m_byteSize;
  bool      m_updated;
  PidVlu    m_vlu;
  PIDs::IDs m_id;
  Types     m_availableTypes;
  Types     m_preferedType;

};

class sensor_Base : public Base
{
public:
  sensor_Base (PIDs::IDs id, Types availableTypes, Types preferedType,
               uint16_t ADmax, byteSizes size);
  float    volt() const;
  uint16_t ADmax() const { return m_ADmax; }

protected:
  uint16_t m_ADmax;
};

class sensor_Current : public sensor_Base
{
public:
  sensor_Current(PIDs::IDs id, Types availableTypes, Types preferedType,
                 uint16_t ADmax, byteSizes size) :
    sensor_Base(id, availableTypes, preferedType, ADmax, size) {}
  uint8_t current() const;
  void setCurrent(uint8_t current);
  inline virtual byteSizes size() const { return byteSizes::oneByte; }
};

class sensor_Pressure : public sensor_Base
{
public:
  sensor_Pressure(PIDs::IDs id, Types availableTypes, Types preferedType,
                  uint16_t ADmax, byteSizes size) :
    sensor_Base(id, availableTypes, preferedType, ADmax, size) {}
  uint16_t pressureKPa() const;
};

class sensor_NTC : public sensor_Base
{
public:
  sensor_NTC(PIDs::IDs id, Types availableTypes,
             Types preferedType, uint16_t ADmax, Ntc *ntcObj, byteSizes size);

  float    celcius() const;

protected:
  Ntc *m_ntcObj;
};


// states
class State : public Base
{
public:
  State(PIDs::IDs id);

  inline void setState(States state) { m_vlu.uint16 = static_cast<uint16_t>(state); }
  inline States state() const { return static_cast<States>(m_vlu.uint16); }
};

// outputs
class actuator_Base : public Base
{
public:
  actuator_Base (PIDs::IDs id, Types availableTypes, Types preferedType) :
    Base(id, availableTypes, preferedType, byteSizes::oneByte) {}
protected:
};

class actuator_PWM : public actuator_Base
{
public:
  actuator_PWM(PIDs::IDs id, Types availableTypes, Types preferedType) :
    actuator_Base(id, availableTypes, preferedType) { }
  uint8_t duty() const; // in percent
};

class clone_Sensor : public sensor_Base
{
	sensor_Base *m_clonePID;
public:
	explicit clone_Sensor(sensor_Base *pid, Types preferedType);
};


}; // namespace PID



namespace PIDs {

/**
 * These are unique system wide IDs of all PIDs avaliable in the system
 * ie must be the same in all ECUs
 *
 * These must also be updated in CAN.h as PID update frames is hardcoded
 *
 */
static const uint16_t MAX_PIDS = 300;
enum IDs : uint16_t {
    // suspension ecu outputs
    Nothing                     = 0,
    leftFillPWM_8bit            = 1, //8bit
    leftDumpPWM_8bit            = 2, //8bit
    leftSuckPWM_8bit            = 3, //8bit
    rightFillPWM_8bit           = 4,
    rightDumpPWM_8bit           = 5,
    rightSuckPWM_8bit           = 6,
    airdryerPWM_8bit            = 7,
    compressorRelayPWM_8bit     = 8,
    compressorFanPWM_8bit       = 9,
    outputsEnd,

    // suspension ecu inputs
    systemPressure_12bit         = 20, //12bit
    leftPressure_12bit           = 21, //12bit
    leftHeight_12bit             = 22, //12bit
    rightPressure_12bit          = 23, //12bit
    rightHeight_12bit            = 24, //12bit
    compressorTemp_12bit         = 25, //12bit
    suspensionSpareAnalog1_12bit = 26, //12bit
    suspensionSpareTemp1_12bit   = 27, //12bit
    compressorCurrent_8bit       = 28, //8bit

    // suspension ecu system pressure regulator
    airFeedState_16bit           = 40,
    heightState_16bit            = 41,

    // suspension calculated values
    loadWeight_16bit             = 52,

    // displaynode things starts at 200
    upButton_8bit               = 200,
};



template<typename T>
class _Handler : public List<T>
{
	PID::Base *m_updatePid;
public:
  _Handler<T>() :
  	  List<T>(),
	  m_updatePid(nullptr)
  {
  }

  PID::Base* find(IDs id) {
    // a cache speedup if consecutive finds are in order
    if (this->m_current != nullptr &&
        this->m_current->next != nullptr &&
        this->m_current->next->id() == id)
    {
        this->m_current = this->m_current->next;
        return this->m_current;
    }
    // do a full seek from beginning
    for (PID::Base *pid = this->first();
         pid != nullptr;
         pid = this->next())
    {
      if (pid->id() == id) {
          return pid;
      }
    }
    return nullptr;
  }

  PID::Base *nextUpdated() {
	  if (m_updatePid == nullptr)
		m_updatePid = this->m_first;
	  else
	    m_updatePid = m_updatePid->next; // avoid the same PID to eat up bandwith

	  if (m_updatePid == nullptr)
		  return nullptr;

	  // find first updated
	  while (m_updatePid && !m_updatePid->updated())
		  m_updatePid = m_updatePid->next;

	  // avoid that the same PID eats up
	  return m_updatePid;
  }
};



/**
 * This list all used PIDs that this ECU is interested in
 */
extern _Handler<PID::Base> collection;


}; // namespace PIDs


// ------------------ PIDs originating in suspension ECU ------------------------
// outputs
#define PID_PWM_AVAILABLE static_cast<PID::Types>(PID::Types::output_Digital | \
                                 PID::Types::output_Duty | PID::Types::output_RawData)
#define USE_leftFill_PID \
    PID::actuator_PWM leftFill_PID(PIDs::IDs::leftFillPWM_8bit, PID_PWM_AVAILABLE, \
                                   PID::Types::output_Duty);
#define USE_rightFill_PID \
    PID::actuator_PWM rightFill_PID(PIDs::IDs::rightFillPWM_8bit, PID_PWM_AVAILABLE, \
                                    PID::Types::output_Duty);
#define USE_leftDump_PID \
    PID::actuator_PWM leftDump_PID(PIDs::IDs::leftDumpPWM_8bit, PID_PWM_AVAILABLE, \
                                   PID::Types::output_Duty);
#define USE_rightDump_PID \
    PID::actuator_PWM rightDump_PID(PIDs::IDs::rightDumpPWM_8bit, PID_PWM_AVAILABLE, \
                                    PID::Types::output_Duty);
#define USE_leftSuck_PID \
    PID::actuator_PWM leftSuck_PID(PIDs::IDs::leftSuckPWM_8bit, PID_PWM_AVAILABLE, \
                                   PID::Types::output_Duty);
#define USE_rightSuck_PID \
    PID::actuator_PWM rightSuck_PID(PIDs::IDs::rightSuckPWM_8bit, PID_PWM_AVAILABLE, \
                                    PID::Types::output_Duty);
#define USE_airdryer_PID \
    PID::actuator_PWM airdryer_PID(PIDs::IDs::airdryerPWM_8bit, PID_PWM_AVAILABLE, \
                                   PID::Types::output_Duty);
#define USE_compressorFan_PID \
    PID::actuator_PWM compressorFan_PID(PIDs::IDs::compressorFanPWM_8bit, PID_PWM_AVAILABLE, \
                                     PID::Types::output_Duty);
#define USE_compressorRelay_PID \
    PID::actuator_PWM compressorRelay_PID(PIDs::IDs::compressorRelayPWM_8bit, PID_PWM_AVAILABLE, \
                                           PID::Types::output_Duty);

// pressure inputs
#define PID_PRESSURE_AVAILABLE static_cast<PID::Types>(PID::Types::input_Pressure | \
                                     PID::Types::input_RawData | PID::Types::input_Volts)
#define USE_systemPressure_PID \
    PID::sensor_Pressure systemPressure_PID(PIDs::IDs::systemPressure_12bit, PID_PRESSURE_AVAILABLE, \
                                            PID::Types::input_Pressure, 4095, PID::byteSizes::twoByte);
#define USE_leftPressure_PID \
    PID::sensor_Pressure leftPressure_PID(PIDs::IDs::leftPressure_12bit, PID_PRESSURE_AVAILABLE, \
                                          PID::Types::input_Pressure, 4095, PID::byteSizes::twoByte);
#define USE_rightPressure_PID \
    PID::sensor_Pressure rightPressure_PID(PIDs::IDs::rightPressure_12bit, PID_PRESSURE_AVAILABLE, \
                                           PID::Types::input_Pressure, 4095, PID::byteSizes::twoByte);

// height sensors
#define PID_HEIGHTAVAILABLE static_cast<PID::Types>(PID::Types::input_Steps | \
                                   PID::Types::input_RawData | PID::Types::input_Volts)
#define USE_leftHeight_PID \
    PID::sensor_Base leftHeight_PID(PIDs::IDs::leftHeight_12bit, PID_HEIGHTAVAILABLE, \
                                    PID::Types::input_Steps, 4095, PID::byteSizes::twoByte);
#define USE_rightHeight_PID \
    PID::sensor_Base rightHeight_PID(PIDs::IDs::rightHeight_12bit, PID_HEIGHTAVAILABLE, \
                                     PID::Types::input_Steps, 4095, PID::byteSizes::twoByte);
// analog sensors
#define USE_suspensionSpareAnalog1_PID \
    PID::sensor_Base suspensionSpareAnalog1_PID(PIDs::IDs::suspensionSpareAnalog1_12bit, PID_HEIGHTAVAILABLE, \
                                     PID::Types::input_Steps, 4095, PID::byteSizes::twoByte);


// temperature sensors
#define PID_TEMPAVAILABLE static_cast<PID::Types>(PID::Types::input_Temperature | \
                                 PID::Types::input_Volts | PID::Types::input_RawData)
#define USE_compressorTemp_PID \
    Ntc compressorNtc(2000, 3960, 5000, 25, 4096); /* sensor AVX  NI24MA0502H-- 12bits AD precision */ \
    PID::sensor_NTC compressorTemp_PID(PIDs::IDs::compressorTemp_12bit, PID_TEMPAVAILABLE, \
                                       PID::Types::input_Temperature, 4095, &compressorNtc, \
                                       PID::byteSizes::twoByte);
#define USE_suspensionSpareTemp1_PID \
    Ntc suspensionSpareNtc1(2000, 3960, 5000, 25, 4096); /* sensor AVX  NI24MA0502H-- 12bits AD precision */ \
    PID::sensor_NTC suspensionSpareTemp1_PID(PIDs::IDs::suspensionSpareTemp1_12bit, PID_TEMPAVAILABLE, \
                                             PID::Types::input_Temperature, 4095, &suspensionSpareNtc1, \
                                             PID::byteSizes::twoByte);

// current sensors
#define USE_compressorCurrent_PID \
    PID::sensor_Current compressorCurrent_PID(PIDs::IDs::compressorCurrent_8bit, static_cast<PID::Types>( \
                                              PID::Types::input_Current | PID::Types::input_RawData), \
                                              PID::Types::input_Current, 255, PID::byteSizes::oneByte);

// states
#define USE_airFeedState_PID \
    PID::State airFeedState_PID(PIDs::IDs::airFeedState_16bit);
#define USE_heightState_PID \
    PID::State heightState_PID(PIDs::IDs::heightState_16bit);


// calculated values
#define USE_loadWeight_PID \
    PID::Base loadWeight_PID(PIDs::IDs::loadWeight_16bit, PID::Types::input_RawData, \
                             PID::Types::input_RawData, PID::byteSizes::twoByte);



#endif /* PID_H_ */
