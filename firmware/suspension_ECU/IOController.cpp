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
    CAN::ControllerBase(CAN::senderIds::suspensionNode)
{}

CAN::IOController::~IOController ()
{}

void CAN::IOController::_recievedCommand(CAN_message_t *msg,
                                         senderIds /*senderId*/, msgIdsCommand msgId)
{
  HeightController::Configs cfgId = static_cast<HeightController::Configs>(msg->buf[0]);
  store::byte4 valueB4;
  valueB4.buf[0] = msg->buf[1];
  valueB4.buf[1] = msg->buf[2];
  valueB4.buf[2] = msg->buf[3];
  valueB4.buf[3] = msg->buf[4];

  switch (msgId) {
    case msgIdsCommand::suspensionSetHigh:
      heightStateMachine.setState(PID::States::HighState);
      break;
    case msgIdsCommand::suspensionSetNormal:
      heightStateMachine.setState(PID::States::NormalState);
      break;
    case msgIdsCommand::suspensionSetLow:
      heightStateMachine.setState(PID::States::LowState);
      break;
    case msgIdsCommand::suspensionSetRearWheelsUp:
      this->_init_CAN_message_t(msg, 10);
      if (!heightStateMachine.setRearWheels(true)) {
          msg->id = msgIdsError::suspensionUserError;
          msg->buf[msg->len++] = (msgId & 0xFF00) >> 8;
          msg->buf[msg->len++] = msgId & 0x00FF;
          send(*msg);
      }
      break;
    case msgIdsCommand::suspensionSetRearWheelsDown:
      this->_init_CAN_message_t(msg, 10);
      if (!heightStateMachine.setRearWheels(false)) {
          msg->id = msgIdsError::suspensionUserError;
          msg->buf[msg->len++] = (msgId & 0xFF00) >> 8;
          msg->buf[msg->len++] = msgId & 0x00FF;
          send(*msg);
      }
      break;
    case msgIdsCommand::suspensionSetConfig:
      heightStateMachine.setConfig(cfgId, valueB4);
      break;
    case msgIdsCommand::suspensionGetConfig:
      valueB4 = heightStateMachine.getConfig(cfgId);

      this->_init_CAN_message_t(msg, 10);
      msg->id = msgIdsCommand::suspensionRepondConfig;
      msg->buf[msg->len++] = cfgId;
      msg->buf[msg->len++] = valueB4.buf[0];
      msg->buf[msg->len++] = valueB4.buf[1];
      msg->buf[msg->len++] = valueB4.buf[2];
      msg->buf[msg->len++] = valueB4.buf[3];
      send(*msg);
      break;
    case msgIdsCommand::suspensionSaveConfig:
      heightStateMachine.saveSettings();
      break;
    default: // ignore
      ;
  }
}

void CAN::IOController::_recievedError(CAN_message_t *msg,
                                       senderIds /*senderId*/, msgIdsError /*msgId*/)
{
  // Do nothing, we dont care about others error in this node
}


/* in base class
void CAN::IOController::_recievedDiagnose(CAN_message_t *msg,
                                          senderIds senderId, msgIdsDiag msgId)
{

}
*/

bool CAN::IOController::sendDTC(DTC *dtc)
{
  CAN::msgIdsError msgId = msgIdsError::suspensionDTC;
  return CAN::ControllerBase::sendDTC(dtc, msgId);
}
