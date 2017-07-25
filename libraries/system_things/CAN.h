/*
 * CAN.h
 *
 *  Created on: 5 apr 2015
 *      Author: jof
 */

#ifndef CAN_H_
#define CAN_H_

#include <stdint.h>
#include <FlexCAN.h>
#include <PID.h>
#include <DTC.h>

namespace CAN {
  /**
   * Enums acts as a CAN message database
   * priority Highest to lowest:
   */

enum senderIds : uint8_t {
  // eight possible Nodes i network
  parkbrakeNode  = 0x00,
  suspensionNode = 0x01,
  displayNode    = 0x02,
  node4          = 0x03,
  node5          = 0x04,
  node6          = 0x05,
  node7          = 0x06,
  node8          = 0x07,
  //maxNodeId      = 0x07,
};

static const uint16_t
  senderIdMask          = 0x0007,
  msgIdMask             = 0x01F8,
  msgTypeControlMask    = 0x0000,
  msgTypeErrorMask      = 0x0200,
  msgTypeUpdateMask     = 0x0400,
  msgTypeDiagMask       = 0x0600,
  msgTypeMask           = 0x0600;

/*
 * documentation CAN ID field
 *  11bits -> 0x7FF
 *  last 3 bits is sender node id -> 0xx7 ->0bxxxxxxxx111
 *
 *  first 2 bits is msg control (priority)
 *  Can be: 0b00xxxxxxxxxxx -> control command, ie change setting or state
 *          0b01xxxxxxxxxxx -> error broadcast, ie a DTC has been set
 *          0b10xxxxxxxxxxx -> normal pid update
 *          0b11xxxxxxxxxxx -> diagnosis command or response
 *
 *  bit 4-9 is message identifier is 64different identifiers
 *          0bxx000000xxx
 */
static const uint8_t MAX_FRAME_COUNT = 64;
enum msgIdsCommand : uint16_t {
  NoCommandFrame              = 0,
  highestPrioCommand          = 0x0008, // should not be used

  parkbrakeSet                = 0x0028,
  parkbrakeRelease            = 0x0038,
  parkbrakeServiceSet         = 0x0048,
  parkbrakeServiceUnset       = 0x0058,

  suspensionSetLow            = 0x0088,
  suspensionSetNormal         = 0x0098,
  suspensionSetHigh           = 0x00A8,
  suspensionSetRearWheelsUp   = 0x00B8,
  suspensionSetRearWheelsDown = 0x00C8,

  suspensionSetConfig         = 0x0108,  // configure, byte0=command, byte 1-2 or 1-4 = value
  suspensionGetConfig         = 0x0118,  // get current config, byte 0=command
  suspensionRepondConfig      = 0x0128,  // response to above config, byte0=command, byte 1-2 or 1-4 = value
  suspensionSaveConfig        = 0x0138,  // Save config to suspensionECU EEPROM

  displayBeep                 = 0x0158,

  lowestPrioCommand           = 0x01F8 // should not be used
};

// each node start at 0 or 6 to get the revecer node id
// (msgId & msgIdMask) >> 3 to remove senderID bits
// recieverNodeId = msgId & 0xF0
// if (msgId & 0x0F > 5)
//    msgId -= 6
// action = msgId & 0x0F
enum msgIdsError : uint16_t {
  NoErrorFrame              = 0,
  highestPrioError          = 0x0008, // should not be used

  parkbrakeDTC              = 0x06 << 3,
  parkbrakeUserError        = 0x07 << 3,   // action wasnt allowed like when parkbrake during moving

  suspensionDTC             = 0x08 << 3,   // a errorcode has been set in suspension
  suspensionUserError       = 0x09 << 3,   // user did something stupid
  suspensionGotoLowFirst    = 0x0A << 3,   // when user wants sucked rear wheels

  displayDTC                = 0x10 << 3,
  displayUserEror           = 0x11 << 3,
  displayCustomError        = 0x12 << 3,

  lowestPrioError           = 0x01F8 // should not be used
};

enum msgIdsDiag : uint16_t {
  NoDiagFrame               = 0,
  highestPrioDiag           = 0x0008, // should not be used

  parkbrakeGetDTCLength     = 0x06 << 3,  // command to get DTC length
  parkbrakeDTCLength        = 0x07 << 3,  // the response to above
  parkbrakeGetDTC           = 0x08 << 3,
  parkbrakeDTCResponse      = 0x09 << 3,
  parkbrakeActuatorTest     = 0x0A << 3,
  parkbrakeActuatorStatus   = 0x0B << 3,

  suspensionGetDTCLength    = 0x10 << 3,
  suspensionDTCLength       = 0x11 << 3,
  suspensionGetDTC          = 0x12 << 3,
  suspensionDTCResponse     = 0x13 << 3,
  suspensionActuatorTest    = 0x14 << 3,
  suspensionActuatorStatus  = 0x15 << 3,

  displayGetDTCLength       = 0x16 << 3,
  displayDTCLength          = 0x17 << 3,
  displayGetDTC             = 0x18 << 3,
  displayDTCResponse        = 0x19 << 3,
  displayActuatorTest       = 0x1A << 3,
  displayActuatorStatus     = 0x1B << 3,

  lowestPrioDiag            = 0x01F8 // should not be used
};

enum msgIdsUpdate : uint16_t {
  NoUpdateFrame             = 0,
  highestPrioUpdate         = 0x0008, // should not be used

  parkbrakePID_1            = 0x0048,
  parkbrakePID_2            = 0x0058,
  parkbrakePID_3            = 0x0068,

  suspensionPID_1           = 0x0148,
  suspensionPID_2           = 0x0158,
  suspensionPID_3           = 0x0168,
  suspensionPID_4           = 0x0178,

  lowestPrioUpdate          = 0x01F8 // should not be used
};

/**
 * These defines how the PIDs are packed on the CAN frames
 * are defined in CAN.cpp
 */
/*extern PIDs::IDs SuspensionPID_1_Frame8bit[];
extern PIDs::IDs SuspensionPID_2_FrameMixed[];
extern PIDs::IDs SuspensionPID_3_FrameMixed[];
extern PIDs::IDs SuspensionPID_4_Frame32bit[];
*/

struct CAN_DTC_send : public CAN_message_t
{
  CAN_DTC_send(PID::Base *pid, errorTypes err, uint8_t occurences);
};

class ControllerBase
{
protected:
  FlexCAN       m_can;
  uint8_t       m_senderId;
  virtual void  _recievedCommand(CAN_message_t *msg, senderIds senderId, msgIdsCommand msgId) = 0;
  virtual void  _recievedError(CAN_message_t *msg, senderIds senderId, msgIdsError msgId) = 0;
  virtual void  _recievedUpdate(CAN_message_t *msg, senderIds senderId, msgIdsUpdate msgId);
  virtual void  _recievedDiagnose(CAN_message_t *msg, senderIds senderId, msgIdsDiag msgId);
  bool          sendDTC(DTC *dtc, msgIdsError msgId);
  CAN_message_t _buildDTC_Msg(DTC *dtc) const;
  void          _init_CAN_message_t(CAN_message_t *msg, uint16_t timeout = 0) const;
  void          _parseID(uint16_t id, uint16_t &targetNode, uint16_t &action) const;
  uint16_t      _normalizeAction(uint16_t action) const;

public:
  static const uint32_t baudRate = 250000;
  ControllerBase (senderIds nodeId);
  virtual ~ControllerBase ();
  virtual void init();
  bool send(CAN_message_t &msg);
  virtual bool sendDTC(DTC *dtc) = 0;
  bool sendDiagnoseCommand(msgIdsDiag msgId, PIDs::IDs id = PIDs::IDs::Nothing,
                           uint8_t vlu = 0);

  void loop(); // called from Main event loop, checks for new msg
};

}; // namespace CAN

#endif /* CAN_H_ */
