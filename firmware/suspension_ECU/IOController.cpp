/*
 * IOController.cpp
 *
 *  Created on: 5 apr 2015
 *      Author: jof
 */

#include "IOController.h"
#include "Height.h"

// defined in suspension_ECU.ino
extern HeightController heightStateMachine;

CAN::IOController::IOController () :
    CAN::ControllerBase(can_senderIds_e::C_suspensionNode)
{}

CAN::IOController::~IOController ()
{}

void CAN::IOController::_recievedCommand(CAN_message_t *msg,
                                         can_senderIds_e /*senderId*/, can_msgIdsCommand_e msgId)
{
#ifdef DEBUG_UART_ON
    Serial.println("can recvCmd");
#endif
  Configs cfgId = Configs::NoConfig;
  store::byte4 valueB4;
  if (msg->len >0)
      cfgId = static_cast<Configs>(msg->buf[0]);
  if (msg->len > 4) {
      valueB4.buf[0] = msg->buf[1];
      valueB4.buf[1] = msg->buf[2];
      valueB4.buf[2] = msg->buf[3];
      valueB4.buf[3] = msg->buf[4];
  }

  can_userError_e exc = C_userErrorNone;

  switch (msgId) {
    case C_suspensionCmdSetHigh: {
      exc = heightStateMachine.setState(PID::States::ToHighState);
      if (exc != C_userErrorNone) {
          _sendUserError(exc);
      } else {
          _init_CAN_message_t(msg, 10);
          msg->id = CAN_MSG_TYPE_COMMAND | C_suspensionCmdSetHigh | m_senderId;
          msg->buf[msg->len++] = 0xAA;
          send(*msg);
      }
    } break;
    case C_suspensionCmdSetNormal: {
      exc = heightStateMachine.setState(PID::States::ToNormalState);
      if (exc != C_userErrorNone) {
    	  _sendUserError(exc);
      } else {
          _init_CAN_message_t(msg, 10);
          msg->id = CAN_MSG_TYPE_COMMAND | C_suspensionCmdSetNormal | m_senderId;
          msg->buf[msg->len++] = 0xAA;
          send(*msg);
      }
    } break;
    case C_suspensionCmdSetLow: {
      exc = heightStateMachine.setState(PID::States::ToLowState);
      if (exc != C_userErrorNone) {
          _sendUserError(exc);
      } else {
    	  _init_CAN_message_t(msg, 10);
          msg->id = CAN_MSG_TYPE_COMMAND | C_suspensionCmdSetLow | m_senderId;
          msg->buf[msg->len++] = 0xAA;
          send(*msg);
      }
    } break;
    case C_suspensionCmdSetRearWheelsUp: {
      bool up = msg->buf[0] > 0;
      exc = heightStateMachine.setRearWheels(up);
      if (exc != C_userErrorNone)
          _sendUserError(exc);
    } break;
    case C_suspensionCmdSetConfig: {
      int res = heightStateMachine.setConfig(cfgId, valueB4);
      _init_CAN_message_t(msg, 10);
      msg->id = CAN_MSG_TYPE_COMMAND | C_suspensionCmdSetConfig | m_senderId;
      msg->buf[msg->len++] = cfgId;
      msg->buf[msg->len++] = res & 0xFF;
      send(*msg);
    } break;
    case C_suspensionCmdGetConfig: {
      valueB4 = heightStateMachine.getConfig(cfgId);

      _init_CAN_message_t(msg, 10);
      msg->id = CAN_MSG_TYPE_COMMAND | C_suspensionCmdGetConfig | m_senderId;
      msg->buf[msg->len++] = cfgId;
      // need to make sure it send little endian
      uint32_t vlu = valueB4.uint32;
      msg->buf[msg->len++] = (vlu & 0x000000FF) >> 0;
      msg->buf[msg->len++] = (vlu & 0x0000FF00) >> 8;
      if (valueB4.buf[2] > 0 || valueB4.buf[3] > 0) {
          msg->buf[msg->len++] = (vlu & 0x00FF0000) >> 16;
          msg->buf[msg->len++] = (vlu & 0xFF000000) >> 24;
      }
      send(*msg);
    } break;
//
//    case C_suspensionCmdSaveConfig: {
//      heightStateMachine.saveSettings();
//    } break;
//
    default: // ignore
      break;
  }
}

void CAN::IOController::_recievedException(CAN_message_t *msg,
                                           can_senderIds_e /*senderId*/, can_msgIdsException_e /*msgId*/)
{
  // Do nothing, we don't care about others error in this node

#ifdef DEBUG_UART_ON
    Serial.println("can recvExc");
#endif
}


// is implemented in base class
//void CAN::IOController::_recievedDiagnose(CAN_message_t *msg,
//                                          senderIds senderId, msgIdsDiag msgId)
//{
//
//}


bool CAN::IOController::sendNewDTC(DTC *dtc)
{
  can_msgIdsException_e msgId = C_suspensionExcDTC;
  return CAN::ControllerBase::sendNewDTC(dtc, msgId);
}

void CAN::IOController::_sendUserError(can_userError_e exc)
{
    CAN_message_t msg;
    _init_CAN_message_t(&msg, 10);
    msg.id = CAN_MSG_TYPE_EXCEPTION | C_suspensionExcUserError | m_senderId;
    // little endian
    msg.buf[msg.len++] = (exc & 0x00FF) >> 0;
    msg.buf[msg.len++] = (exc & 0xFF00) >> 8;
    send(msg);
}
