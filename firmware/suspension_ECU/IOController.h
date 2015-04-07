/*
 * IOController.h
 *
 *  Created on: 5 apr 2015
 *      Author: jof
 */

#ifndef IOCONTROLLER_H_
#define IOCONTROLLER_H_

#include <CAN.h>

namespace CAN {

class IOController :public ControllerBase
{
protected:
  void _recievedCommand(CAN_message_t *msg, senderIds senderId, msgIdsCommand msgId);
  void _recievedError(CAN_message_t *msg, senderIds senderId, msgIdsError msgId);
  //void _recievedDiagnose(CAN_message_t *msg, senderIds senderId, msgIdsDiag msgId);
public:
  IOController ();
  virtual ~IOController ();

  bool sendDTC(DTC *dtc);

};

} /* namespace CAN */

#endif /* IOCONTROLLER_H_ */
