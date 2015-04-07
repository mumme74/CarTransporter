/*
 * IOController.cpp
 *
 *  Created on: 5 apr 2015
 *      Author: jof
 */

#include "IOController.h"


CAN::IOController::IOController () :
    CAN::ControllerBase(CAN::senderIds::suspensionNode)
{}

CAN::IOController::~IOController ()
{}

void CAN::IOController::_recievedCommand(CAN_message_t *msg,
                                         senderIds /*senderId*/, msgIdsCommand msgId)
{
  switch (msgId) {
    case msgIdsCommand::suspensionSetHigh:
      break;
    case msgIdsCommand::suspensionSetNormal:
      break;
    case msgIdsCommand::suspensionSetLow:
      break;
    case msgIdsCommand::suspensionSetSuckedNormal:
      break;
    case msgIdsCommand::suspensionSetSuckedHigh:
      break;
    case msgIdsCommand::suspensionConfigHeight:
      break;
    case msgIdsCommand::suspensionConfigPIDLoop_D:
      break;
    case msgIdsCommand::suspensionConfigPIDLoop_I:
      break;
    case msgIdsCommand::suspensionConfigPIDLoop_P:
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


/*
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
