/*
 * IOController.h
 *
 *  Created on: 5 apr 2015
 *      Author: jof
 */

#ifndef IOCONTROLLER_H_
#define IOCONTROLLER_H_

#include <CAN.h>
#include <can_protocol.h>

namespace CAN {

class IOController : public ControllerBase
{
protected:
  void _recievedCommand(CAN_message_t *msg, can_senderIds_e senderId, can_msgIdsCommand_e msgId);
  void _recievedException(CAN_message_t *msg, can_senderIds_e senderId, can_msgIdsException_e msgId);
  //void _recievedDiagnose(CAN_message_t *msg, senderIds senderId, msgIdsDiag msgId);
public:
  explicit IOController ();
  virtual ~IOController ();

  bool sendNewDTC(DTC *dtc);

private:
  void _sendUserError(can_userError_e exc);

};

} /* namespace CAN */

#endif /* IOCONTROLLER_H_ */
