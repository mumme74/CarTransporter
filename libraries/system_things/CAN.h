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
#include "can_protocol.h"

namespace CAN {

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
  virtual void  _recievedCommand(CAN_message_t *msg, can_senderIds_e senderId, can_msgIdsCommand_e msgId) = 0;
  virtual void  _recievedException(CAN_message_t *msg, can_senderIds_e senderId, can_msgIdsException_e msgId) = 0;
  virtual void  _recievedUpdate(CAN_message_t *msg, can_senderIds_e senderId, can_msgIdsUpdate_e msgId);
  virtual void  _recievedDiagnose(CAN_message_t *msg, can_senderIds_e senderId, can_msgIdsDiag_e msgId);
  bool          sendDTC(DTC *dtc, can_msgIdsException_e msgId);
  CAN_message_t _buildDTC_Msg(DTC *dtc, uint8_t idx) const;
  void          _init_CAN_message_t(CAN_message_t *msg, uint16_t timeout = 0) const;
  void          _parseID(uint16_t id, uint16_t &targetNode, uint16_t &action) const;
  uint16_t      _normalizeAction(uint16_t action) const;

public:
  static const uint32_t baudRate = 250000;
  explicit ControllerBase (can_senderIds_e nodeId);
  virtual ~ControllerBase ();
  virtual void init();
  bool send(CAN_message_t &msg);
  bool sendNewDTC(DTC *dtc, can_msgIdsException_e msgId);
  bool sendDiagnoseCommand(can_msgIdsDiag_e msgId, PIDs::IDs id = PIDs::IDs::Nothing,
                           uint8_t vlu = 0);

  void loop(); // called from Main event loop, checks for new msg
};

}; // namespace CAN

#endif /* CAN_H_ */
