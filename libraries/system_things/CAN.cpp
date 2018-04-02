/*
 * CAN.cpp
 *
 *  Created on: 5 apr 2015
 *      Author: jof
 */

#include "CAN.h"
#include "Outputs.h"
#include "can_protocol.h"
#include <Arduino.h>
#include "suspension_adresses.h"

namespace CAN {

static const uint8_t MAX_FRAME_COUNT = 64;

struct Frame_t {
  const can_msgIdsUpdate_e msgId;
  const uint8_t len;
  const PIDs::IDs *ids;
};
struct PidID_to_Frame_t {
  const PIDs::IDs id;
  Frame_t *frame;
};
struct msgId_to_Frame_t {
  const can_msgIdsUpdate_e msgId;
  const Frame_t *frame;
};

/**
 * These defines how the PIDs are packed on the CAN frames
 * are defined in CAN.cpp
 *
 * Adding a a frame you need to:
 *  1. add the PIDs to the frame
 *  2. add the frame to FramesDB
 *  3. add the PID.id to ID_FrameDB to point to the new frame
 */
PIDs::IDs SuspensionPID1ids [] = {
    PIDs::IDs::leftFillPWM_8bit,
    PIDs::IDs::leftDumpPWM_8bit,
    PIDs::IDs::leftSuckPWM_8bit,
    PIDs::IDs::rightFillPWM_8bit,
    PIDs::IDs::rightDumpPWM_8bit,
    PIDs::IDs::rightSuckPWM_8bit,
    PIDs::IDs::airdryerPWM_8bit,
    PIDs::IDs::compressorPWM_8bit,
};
Frame_t SuspensionPID_1_Frame8bit = {
  C_suspensionUpdPID_1,
  8, // number of pids in this frame
  SuspensionPID1ids
};
Frame_t *SuspensionPID_1_Frame8bitPtr = &SuspensionPID_1_Frame8bit;

PIDs::IDs SuspensionPID2ids [] = {
    PIDs::IDs::systemPressure_12bit, // 2byte
    PIDs::IDs::leftPressure_12bit, // 4byte
    PIDs::IDs::rightPressure_12bit, // 6byte
    PIDs::IDs::compressorCurrent_8bit, // 7byte
    PIDs::IDs::suspensionSpare1PWM_8bit, // 8byte
};
Frame_t SuspensionPID_2_FrameMixed = {
  C_suspensionUpdPID_2,
  5, // number of pids in this frame
  SuspensionPID2ids
};
Frame_t *SuspensionPID_2_FrameMixedPtr = &SuspensionPID_2_FrameMixed;

PIDs::IDs SuspensionPID3ids[] = {
    PIDs::IDs::leftHeight_12bit, // 2byte
    PIDs::IDs::rightHeight_12bit, // 4byte
    PIDs::IDs::compressorTemp_12bit, // 6byte
    PIDs::IDs::suspensionSpareAnalog1_12bit, // 8byte
};
Frame_t SuspensionPID_3_FrameMixed = {
  C_suspensionUpdPID_3,
  4, // number of pids in this frame
  SuspensionPID3ids
};
Frame_t *SuspensionPID_3_FrameMixedPtr = &SuspensionPID_3_FrameMixed;

PIDs::IDs SuspensionPID4ids[] = {
    PIDs::IDs::airFeedState_16bit, // 2byte
    PIDs::IDs::heightState_16bit, // 4byte
    PIDs::IDs::loadWeight_16bit,  // 6byte
};
Frame_t SuspensionPID_4_Frame16bit[] = {
  C_suspensionUpdPID_4,
  3, // number of pids in this frame
  SuspensionPID4ids
};
Frame_t *SuspensionPID_4_Frame16bitPtr = SuspensionPID_4_Frame16bit;

// used when we want to seek for the frame and have msgId
msgId_to_Frame_t FramesTable[] = {
    { C_suspensionUpdPID_1, CAN::SuspensionPID_1_Frame8bitPtr },
    { C_suspensionUpdPID_2, CAN::SuspensionPID_2_FrameMixedPtr },
    { C_suspensionUpdPID_3, CAN::SuspensionPID_3_FrameMixedPtr },
    { C_suspensionUpdPID_4, CAN::SuspensionPID_4_Frame16bitPtr },

    // this empty carrier signifies the end
    { C_NoUpdateFrame, nullptr }
};
// Used when we have a pid id and want to know the frame
PidID_to_Frame_t ID_to_FrameTable[] = {
    // suspension frame1
    { PIDs::IDs::leftFillPWM_8bit, CAN::SuspensionPID_1_Frame8bitPtr },
    { PIDs::IDs::leftDumpPWM_8bit, CAN::SuspensionPID_1_Frame8bitPtr },
    { PIDs::IDs::leftSuckPWM_8bit, CAN::SuspensionPID_1_Frame8bitPtr },
    { PIDs::IDs::rightFillPWM_8bit, CAN::SuspensionPID_1_Frame8bitPtr },
    { PIDs::IDs::rightDumpPWM_8bit, CAN::SuspensionPID_1_Frame8bitPtr },
    { PIDs::IDs::rightSuckPWM_8bit, CAN::SuspensionPID_1_Frame8bitPtr },
    { PIDs::IDs::airdryerPWM_8bit, CAN::SuspensionPID_1_Frame8bitPtr },
    { PIDs::IDs::compressorPWM_8bit, CAN::SuspensionPID_1_Frame8bitPtr },

    // suspension frame2
    { PIDs::IDs::systemPressure_12bit, CAN::SuspensionPID_2_FrameMixedPtr },
    { PIDs::IDs::leftPressure_12bit, CAN::SuspensionPID_2_FrameMixedPtr },
    { PIDs::IDs::rightPressure_12bit, CAN::SuspensionPID_2_FrameMixedPtr },
    { PIDs::IDs::compressorCurrent_8bit, CAN::SuspensionPID_2_FrameMixedPtr },
    { PIDs::IDs::suspensionSpare1PWM_8bit, CAN::SuspensionPID_2_FrameMixedPtr },

    // suspension frame3
    { PIDs::IDs::leftHeight_12bit, CAN::SuspensionPID_3_FrameMixedPtr },
    { PIDs::IDs::rightHeight_12bit, CAN::SuspensionPID_3_FrameMixedPtr },
    { PIDs::IDs::compressorTemp_12bit, CAN::SuspensionPID_3_FrameMixedPtr },
    { PIDs::IDs::suspensionSpareAnalog1_12bit, CAN::SuspensionPID_3_FrameMixedPtr },

    // suspension frame4
    { PIDs::IDs::airFeedState_16bit, CAN::SuspensionPID_4_Frame16bitPtr },
    { PIDs::IDs::heightState_16bit, CAN::SuspensionPID_4_Frame16bitPtr },

    // this empty carrier signifies the end
    { PIDs::IDs::Nothing, nullptr }
};
/**
 * Find the frame which holds this PID
 */
const Frame_t *frame_from_id(PIDs::IDs id){
  for (int i = 0; i < PIDs::MAX_PIDS; ++i) {
    if (ID_to_FrameTable[i].id == PIDs::IDs::Nothing)
      return nullptr;
    if (ID_to_FrameTable[i].id == id)
      return ID_to_FrameTable[i].frame;
  }
  return nullptr;
}

/**
 * Find the frame for this messageId
 */
const Frame_t *frame_from_msgId(can_msgIdsUpdate_e msgId) {
  for (int i = 0; i < MAX_FRAME_COUNT; ++i) {
    if (FramesTable[i].msgId == C_NoUpdateFrame)
      return nullptr;
    if (FramesTable[i].msgId == msgId)
      return FramesTable[i].frame;
  }
  return nullptr;
}

/**
 * Get the msgId for a given PID.id
 */
can_msgIdsUpdate_e msgId_from_ID(PIDs::IDs id) {
   for (int i = 0; i < MAX_FRAME_COUNT; ++i){
     if (FramesTable[i].msgId == C_NoUpdateFrame)
       return C_NoUpdateFrame;

     const Frame_t *frame = FramesTable[i].frame;
     for (int j = 0;i < frame->len; ++j) {
         PIDs::IDs tmpId = frame->ids[j];
         if (tmpId == PIDs::IDs::Nothing)
           break;

         if (tmpId == id)
           return frame->msgId;
     }
   }

   return C_NoUpdateFrame;
 }

} // namespace CAN

using namespace CAN;

ControllerBase::ControllerBase (can_senderIds_e nodeId) :
   m_senderId(nodeId & 0xFF)
{
    m_can.begin(baudRate);
}

ControllerBase::~ControllerBase ()
{
}

// as in request for PID value
void ControllerBase::_recievedUpdate(CAN_message_t *msg,
                                        can_senderIds_e /*senderId*/, can_msgIdsUpdate_e msgId)
{
  // find the correct frame
  const Frame_t *frame = frame_from_msgId(msgId);
  if (frame == nullptr)
    return;

  // read each pid from the msg bytes
  int byteCnt = 0;
  for (int i = 0; i < 8; ++i) {
      PIDs::IDs id = frame->ids[i];
      if (id == PIDs::IDs::Nothing) {
          break;
      }

      PID::Base *pid = PIDs::collection.find(id);
      if (pid != nullptr) {
          PID::byteSizes siz = pid->size();
          switch(siz) {
            case PID::byteSizes::oneByte: {
              uint16_t vlu = pid->rawValue();
              msg->buf[byteCnt++] = vlu & 0x00FF;
            } break;
            case PID::byteSizes::twoByte: {
              // little endian, LSB byte first
              uint16_t vlu = pid->rawValue();
              msg->buf[byteCnt++] = (vlu & 0x00FF) << 0;
              msg->buf[byteCnt++] = (vlu & 0xFF00) << 8;
            } break;
            case PID::byteSizes::fourByte:
              // little endian, LSB byte first
              uint32_t vlu32 = pid->rawData32();
              msg->buf[byteCnt++] = (vlu32 & 0x000000FF) << 0;
              msg->buf[byteCnt++] = (vlu32 & 0x0000FF00) << 8;
              msg->buf[byteCnt++] = (vlu32 & 0x00FF0000) << 16;
              msg->buf[byteCnt++] = (vlu32 & 0xFF000000) << 24;
              break;
          }
      }
  }

  _init_CAN_message_t(msg, 5);
  msg->id = CAN_MSG_TYPE_UPDATE | msgId | m_senderId;
  msg->len = byteCnt;
  send(*msg);
}

void ControllerBase::_recievedDiagnose(CAN_message_t *msg,
                                          can_senderIds_e senderId, can_msgIdsDiag_e msgId)
{
  // each node start at 0 or 6 to get the reciever node id
  uint16_t action, recieverNodeId;
  this->_parseID(msg->id, recieverNodeId, action);

  if (recieverNodeId == this->m_senderId) {
    // its intended for this node


    uint16_t id = 0;

    switch(action) {
      case C_suspensionDiagDTCLength: { // DTC Length
        _init_CAN_message_t(msg, 40); // 40ms timeout to make sure it reaches our destination
        msg->buf[msg->len++] = static_cast<uint8_t>(DTCs.store.length());
        ++action;
        msg->id = CAN_MSG_TYPE_DIAG | C_suspensionDiagDTCLength | m_senderId;
        this->send(*msg);
      }  break;

      case C_suspensionDiagGetDTC: { // get DTC with PID as argument
        // little endian
        id = msg->buf[1] << 8 | msg->buf[0];
        for (DTC *dtc = DTCs.store.first(); dtc == nullptr; dtc = DTCs.store.next()) {
          if (dtc->pidID() == id || id == 0){ // send every DTC if id is 0
              uint8_t idx = dtc->storedIndex();
              CAN_message_t msgErr = this->_buildDTC_Msg(dtc, idx);
              msgErr.timeout = 20; // wait 20ms if needed to make sure it reaches the line
              ++action;
              msgErr.id = CAN_MSG_TYPE_DIAG | C_suspensionDiagGetDTC | m_senderId;
              send(msgErr);
              return;
          }
        }
      } break;
      case C_suspensionDiagActuatorTest: { // Set actuatortest
        id = msg->buf[0];
        for (IOutput *outDrv = OutputsController.first();
             outDrv != nullptr;
             outDrv = OutputsController.next())
        {
          if (outDrv->pid()->id() == id) {
              // set actuation test on this pid
              outDrv->setActuatorTest(msg->buf[1]);
              // respond to requesting node
              ++action;
              msg->id = CAN_MSG_TYPE_DIAG | C_suspensionDiagActuatorTest | m_senderId;
              send(*msg);
              return;
          }
        }
      } break; case C_suspensionDiagClearActuatorTest: { // Set actuatortest
        id = msg->buf[0];
        for (IOutput *outDrv = OutputsController.first();
             outDrv != nullptr;
             outDrv = OutputsController.next())
        {
          if (outDrv->pid()->id() == id) {
              // set actuation test on this pid
              outDrv->clearActuatorTest();
              // respond to requesting node
              ++action;
              msg->id = CAN_MSG_TYPE_DIAG | C_suspensionDiagClearActuatorTest | m_senderId;
              send(*msg);
              return;
          }
        }
      } break;
      case C_suspensionDiagClearDTC: {
          uint8_t dtcCnt = msg->buf[0],
                  retCnt = 0;

          if (dtcCnt == DTCs.store.length())
              retCnt = DTCs.clear();
          _init_CAN_message_t(msg, 40);
          msg->id = CAN_MSG_TYPE_DIAG | C_suspensionDiagClearDTC | m_senderId;
          msg->buf[0] = retCnt;
          msg->len = 1;
          send(*msg);
      } break;
      default: ; //unsupported, subclass implement
    }
  }
}

void ControllerBase::_parseID(uint16_t id, uint16_t &targetNode, uint16_t &action) const
{
  // each node start at 0 or 6 to get the reciever node id
  uint16_t mID = (id & CAN_MSG_ID_MASK) >> 3; // to remove senderID bits
  targetNode = (mID & 0xF0) >> 4;

  action = mID & 0x0F;
}

CAN_message_t ControllerBase::_buildDTC_Msg(DTC *dtc, uint8_t idx) const
{
  CAN_message_t msg;
  _init_CAN_message_t(&msg, 10);

  msg.id = 0;
  // request [0:7] the DTC index in stored memory
  // response:
  //  [0:7]        [0:15]    [0:6] [:7]
  // stored nr     dtc code  occurrences & pending mask (1 on 7th bit = real code, 0=pending)

  msg.buf[0] = idx;
  uint16_t dtcCode = 0;
  uint8_t errNr = static_cast<uint8_t>(dtc->errType());
  while (errNr > 9)
      errNr -= 10;

  switch (dtc->pidID()) {
  // outputs
  case Adresses::leftFill:
      dtcCode = C_dtc_leftFill_openLoad + errNr;
      break;
  case Adresses::leftDump:
      dtcCode = C_dtc_leftDump_openLoad + errNr;
      break;
  case Adresses::leftSuck:
      dtcCode = C_dtc_leftSuck_openLoad + errNr;
      break;
  case Adresses::rightFill:
      dtcCode = C_dtc_rightFill_openLoad + errNr;
      break;
  case Adresses::rightDump:
      dtcCode = C_dtc_rightDump_openLoad + errNr;
      break;
  case Adresses::rightSuck:
      dtcCode = C_dtc_rightSuck_openLoad + errNr;
      break;
  case Adresses::airdryer:
      dtcCode = C_dtc_airDryer_openLoad + errNr;
      break;
  case Adresses::spare1:
      dtcCode = C_dtc_spare1_openLoad + errNr;
      break;
  case Adresses::compressor:
      dtcCode = C_dtc_compressor_openLoad + errNr;
      break;

  // sensors
  case Adresses::airPressure:
      dtcCode = C_dtc_airPressure_nonValidValue + errNr;
      break;
  case Adresses::leftPressure:
      dtcCode = C_dtc_leftPressure_nonValidValue + errNr;
      break;
  case Adresses::leftHeight:
      dtcCode = C_dtc_leftHeight_nonValidValue + errNr;
      break;
  case Adresses::rightPressure:
      dtcCode = C_dtc_rightPressure_nonValidValue + errNr;
      break;
  case Adresses::rightHeight:
      dtcCode = C_dtc_rightHeight_nonValidValue + errNr;
      break;
  case Adresses::compressorTemp:
      dtcCode = C_dtc_compressorTemp_nonValidValue + errNr;
      break;
  case Adresses::systemPressure:
      dtcCode = C_dtc_systemPressure_nonValidValue + errNr;
      break;
  case Adresses::spareAnalog1:
      dtcCode = C_dtc_spareAnalog1_nonValidValue + errNr;
      break;
  case Adresses::spareTemp1:
      dtcCode = C_dtc_spareTemp1_nonValidValue + errNr;
      break;

  // compressor control logic
  case Adresses::airFeed:
      dtcCode = C_dtc_airFeed_overCurrent + errNr;
      break;
  default:
      // unhandled ??
      dtcCode = 0;
      break;
  }

  // little endian
  msg.buf[1] = (dtcCode & 0x00FF) >> 0;
  msg.buf[2] = (dtcCode & 0xFF00) >> 8;

  // pending and occurrences
  msg.buf[3] = dtc->mask();
  msg.len = 4;

  return msg;
}


void ControllerBase::_init_CAN_message_t(CAN_message_t *msg, uint16_t timeout /*= 0*/) const
{
  msg->ext = msg->len = msg->id = 0;
  msg->timeout = timeout;
  for(int i = 0; i < 8; ++i)
    msg->buf[i] = 0;
}


void ControllerBase::init()
{
  m_can.begin();
}

void ControllerBase::loop()
{
  CAN_message_t msg;
  msg.timeout = 0; // no wait when receiving
  while(m_can.read(msg) != 0) {
    Serial.println("can msg");
    can_senderIds_e senderId = static_cast<can_senderIds_e>(msg.id & CAN_MSG_SENDER_ID_MASK);

    switch(msg.id & CAN_MSG_TYPE_MASK) {
      case CAN_MSG_TYPE_COMMAND: //msgTypeControlMask:
        can_msgIdsCommand_e msgIdCmd;
        msgIdCmd = static_cast<can_msgIdsCommand_e>(msg.id & CAN_MSG_ID_MASK);
        this->_recievedCommand(&msg, senderId, msgIdCmd);
        break;
      case CAN_MSG_TYPE_EXCEPTION: //msgTypeErrorMask:
        can_msgIdsException_e msgIdErr;
        msgIdErr = static_cast<can_msgIdsException_e>(msg.id & CAN_MSG_ID_MASK);
        this->_recievedException(&msg, senderId, msgIdErr);
        break;
      case CAN_MSG_TYPE_UPDATE: //msgTypeUpdateMask:
        can_msgIdsUpdate_e msgIdUpd;
        msgIdUpd = static_cast<can_msgIdsUpdate_e>(msg.id & CAN_MSG_ID_MASK);
        this->_recievedUpdate(&msg, senderId, msgIdUpd);
        break;
      case CAN_MSG_TYPE_DIAG:// msgTypeDiagMask:
        can_msgIdsDiag_e msgIdDiag;
        msgIdDiag = static_cast<can_msgIdsDiag_e>(msg.id & CAN_MSG_ID_MASK);
        this->_recievedDiagnose(&msg, senderId, msgIdDiag);
        break;
      default:
        // can never get here as msgTypeMask filter all bits but 2
        Serial.print("Can id err:");Serial.println(msg.id);
        return;
    }
  }

  // send all pids which has updates
  // sends in consecutive order, one at each loop time
  // eases out on our poor network
  PID::Base *pid = PIDs::collection.nextUpdated();
  if (pid) {
	  // find which frame this pid belongs to
	  const Frame_t *frame = frame_from_id(pid->id());
	  if (frame == nullptr)
		return;

	  // read the pids and send them
	  this->_init_CAN_message_t(&msg);
	  msg.id = static_cast<uint16_t>(frame->msgId);

	  // stuff the bytes into this frame
	  for(int i = 0; i < 8; ++i) {
		PIDs::IDs tID = frame->ids[i];
		pid = PIDs::collection.find(tID);
		if(pid == nullptr)
		  break;

		// mark as read
		pid->setUpdated(false);

		// output the PID value to the CAN frame
		switch(pid->size()){
		  case PID::byteSizes::oneByte:
			msg.buf[msg.len++] = static_cast<uint8_t>(pid->rawValue());
			break;
		  case PID::byteSizes::twoByte:
			// little endian, highest byte first
			uint16_t vlu16;
			vlu16 = pid->rawValue();
			msg.buf[msg.len++] = (vlu16 & 0x00FF) >> 0;
			msg.buf[msg.len++] = (vlu16 & 0xFF00) >> 8;
			break;
		  case PID::byteSizes::fourByte:
			// little endian, highest byte first
			uint32_t vlu32;
			vlu32 = pid->rawData32();
			msg.buf[msg.len++] = (vlu32 & 0x000000FF) >> 0;
			msg.buf[msg.len++] = (vlu32 & 0x0000FF00) >> 8;
			msg.buf[msg.len++] = (vlu32 & 0x00FF0000) >> 16;
			msg.buf[msg.len++] = (vlu32 & 0xFF000000) >> 24;
			break;
		}
	  }
	}
}

bool ControllerBase::send(CAN_message_t &msg)
{
  msg.id &= CAN_MSG_TYPE_MASK | CAN_MSG_ID_MASK; // clear possible erroneous nodeID bits
  msg.id |= m_senderId;  // this node sent this msg
  return m_can.write(msg);
}

bool ControllerBase::sendNewDTC(DTC *dtc, can_msgIdsException_e msgId)
{
  uint8_t idx = dtc->storedIndex();
  CAN_message_t msg = this->_buildDTC_Msg(dtc, idx);
  msg.id = msgId;
  return send(msg);
}


bool ControllerBase::sendDiagnoseCommand(can_msgIdsDiag_e msgId,
                                         PIDs::IDs id /*= PIDs::IDs::Nothing*/,
                                         uint8_t vlu /* = 0*/)
{
  uint16_t action = msgId & CAN_MSG_ID_MASK;

  CAN_message_t msg;
  this->_init_CAN_message_t(&msg, 1000);// don't abort if line is busy, 1sec timeout

  uint16_t mID = static_cast<uint16_t>(id);

  switch(action){
    case C_suspensionDiagDTCLength: // get DTC LENGTH
      msg.id = msgId;
      return send(msg);
    case C_suspensionDiagGetDTC: // get DTC
      msg.id = msgId;
      msg.buf[msg.len++] = (mID & 0xFF00) >> 8;
      msg.buf[msg.len++] = mID & 0x00FF;
      return send(msg);
    case C_suspensionDiagActuatorTest: // Set ActuatorTest
      msg.id = msgId;
      msg.buf[msg.len++] = (mID & 0xFF00) >> 8;
      msg.buf[msg.len++] = mID & 0x00FF;
      return send(msg);
    case 5: // Response ActuatorTest
      break;
    default: ; // ignore
  }
  return false;
}
