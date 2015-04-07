/*
 * CAN.cpp
 *
 *  Created on: 5 apr 2015
 *      Author: jof
 */

#include "CAN.h"
#include "Outputs.h"

namespace CAN {

struct Frame_t {
  const msgIdsUpdate msgId;
  const uint8_t len;
  const PIDs::IDs *ids;
};
struct PidID_to_Frame_t {
  const PIDs::IDs id;
  Frame_t *frame;
};
struct msgId_to_Frame_t {
  const msgIdsUpdate msgId;
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
  msgIdsUpdate::suspensionPID_1,
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
  msgIdsUpdate::suspensionPID_2,
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
  msgIdsUpdate::suspensionPID_3,
  4, // number of pids in this frame
  SuspensionPID3ids
};
Frame_t *SuspensionPID_3_FrameMixedPtr = &SuspensionPID_3_FrameMixed;

PIDs::IDs SuspensionPID4ids[] = {
    PIDs::IDs::airFeedState_32bit, // 4byte
    PIDs::IDs::heightState_32bit, // 8byte
};
Frame_t SuspensionPID_4_Frame32bit[] = {
  msgIdsUpdate::suspensionPID_4,
  2, // number of pids in this frame
  SuspensionPID4ids
};
Frame_t *SuspensionPID_4_Frame32bitPtr = SuspensionPID_4_Frame32bit;

// used when we want to seek for the frame and have msgId
msgId_to_Frame_t FramesTable[] = {
    { msgIdsUpdate::suspensionPID_1, CAN::SuspensionPID_1_Frame8bitPtr },
    { msgIdsUpdate::suspensionPID_2, CAN::SuspensionPID_2_FrameMixedPtr },
    { msgIdsUpdate::suspensionPID_3, CAN::SuspensionPID_3_FrameMixedPtr },
    { msgIdsUpdate::suspensionPID_4, CAN::SuspensionPID_4_Frame32bitPtr },

    // this empty carrier signifies the end
    { msgIdsUpdate::NoUpdateFrame, nullptr }
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
    { PIDs::IDs::airFeedState_32bit, CAN::SuspensionPID_4_Frame32bitPtr },
    { PIDs::IDs::heightState_32bit, CAN::SuspensionPID_4_Frame32bitPtr },

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
 * Find the frame for this maessageId
 */
const Frame_t *frame_from_msgId(msgIdsUpdate msgId) {
  for (int i = 0; i < MAX_FRAME_COUNT; ++i) {
    if (FramesTable[i].msgId == msgIdsUpdate::NoUpdateFrame)
      return nullptr;
    if (FramesTable[i].msgId == msgId)
      return FramesTable[i].frame;
  }
  return nullptr;
}

/**
 * Get the msgId for a given PID.id
 */
msgIdsUpdate msgId_from_ID(PIDs::IDs id) {
   for (int i = 0; i < MAX_FRAME_COUNT; ++i){
     if (FramesTable[i].msgId == msgIdsUpdate::NoUpdateFrame)
       return msgIdsUpdate::NoUpdateFrame;

     const Frame_t *frame = FramesTable[i].frame;
     for (int j = 0;i < frame->len; ++j) {
         PIDs::IDs tmpId = frame->ids[j];
         if (tmpId == PIDs::IDs::Nothing)
           break;

         if (tmpId == id)
           return frame->msgId;
     }
   }

   return msgIdsUpdate::NoUpdateFrame;
 }

CAN::ControllerBase::ControllerBase (senderIds nodeId) :
  m_can(baudRate), // Baud
  m_senderId(static_cast<uint8_t>(nodeId))
{
}

CAN::ControllerBase::~ControllerBase ()
{
}

void CAN::ControllerBase::_recievedUpdate(CAN_message_t *msg,
                                        senderIds /*senderId*/, msgIdsUpdate msgId)
{
  // find the correct frame
  const Frame_t *frame = frame_from_msgId(msgId);
  if (frame == nullptr)
    return;

  // read each pid from the msg bytes
  for (int i = 0, byteCnt = 0; i < 8; ++i) {
      PIDs::IDs id = frame->ids[i];
      if (id == PIDs::IDs::Nothing) {
          break;
      }

      PID::Base *pid = PIDs::collection.find(id);
      if (pid != nullptr) {
          PID::byteSizes siz = pid->size();
          switch(siz) {
            case PID::byteSizes::oneByte:
              pid->setRawValue(msg->buf[byteCnt++]);
              break;
            case PID::byteSizes::twoByte:
              // big endian, highest byte first
              uint16_t vlu16;
              vlu16 = msg->buf[byteCnt++] << 8;
              vlu16 |= msg->buf[byteCnt++];
              pid->setRawValue(vlu16);
              break;
            case PID::byteSizes::fourByte:
              // big endian, highest byte first
              uint32_t vlu32;
              vlu32 = msg->buf[byteCnt++] << 24;
              vlu32 |= msg->buf[byteCnt++] << 16;
              vlu32 |= msg->buf[byteCnt++] << 8;
              vlu32 |= msg->buf[byteCnt++];
              static_cast<PID::State*>(pid)->setRawData32(vlu32);
              break;
          }
      }
  }
}

void CAN::ControllerBase::_recievedDiagnose(CAN_message_t *msg,
                                          senderIds senderId, msgIdsDiag msgId)
{
  // each node start at 0 or 6 to get the reciever node id
  uint16_t action, recieverNodeId;
  this->_parseID(msg->id, recieverNodeId, action);

  if (recieverNodeId == this->m_senderId) {
    // its intended for this node


    uint16_t id = 0;

    switch(_normalizeAction(action)) {
      case 0: // DTC Length
        _init_CAN_message_t(msg, 40); // 40ms timeout to make sure it reaches our destination
        msg->buf[msg->len++] = static_cast<uint8_t>(DTCs.store.length());
        ++action;
        msg->id = msgTypeDiagMask & (recieverNodeId << 7) & (action << 3);
        this->send(*msg);
        break;
      case 1: // response DTC Length
        //uint8_t dtcCnt = msg->buf[0];
        //uint16_t id = msg->id & 0x0FF8; // note due to switch above only our own error get through
        break;//unsupported, subclass implement

      case 2: // get DTC with PID as argument
        id = msg->buf[0] << 8;
        id |= msg->buf[1];
        for (DTC *dtc = DTCs.store.first(); dtc == nullptr; dtc = DTCs.store.next()) {
          if (dtc->pidID() == id || id == 0){ // send every DTC if id is 0
              CAN_message_t msgErr = this->_buildDTC_Msg(dtc);
              msgErr.timeout = 20; // wait 20ms if needed to make sure it reaches the line
              ++action;
              msgErr.id = msgTypeDiagMask & (recieverNodeId << 7) & (action << 3);
              send(msgErr);
              return;
          }
        }
        break;
      case 3: // response to a get DTC request
        break;//unsupported, subclass implement

      case 4: // Set actuatortest
        id = msg->buf[0] << 8;
        id |= msg->buf[1];
        for (IOutput *outDrv = OutputsController.first();
             outDrv != nullptr;
             outDrv = OutputsController.next())
        {
          if (static_cast<uint16_t>(outDrv->pid()->id()) == id) {
              // set actuation test on this pid
              outDrv->setActuatorTest(msg->buf[2]);
              // respond to requesting node
              ++action;
              msg->id = msgTypeDiagMask & (recieverNodeId << 7) & (action << 3);
              send(*msg);
              return;
          }
        }
        break;

      case 5: // response actuatortest progress
        break;//unsupported, subclass implement

      default: ; //unsupported, subclass implement
    }
  }
}

void CAN::ControllerBase::_parseID(uint16_t id, uint16_t &targetNode, uint16_t &action) const
{
  // each node start at 0 or 6 to get the reciever node id
  uint16_t mID = (id & msgIdMask) >> 3; // to remove senderID bits
  targetNode = (mID & 0xF0) >> 4;

  action = mID & 0x0F;
}

uint16_t CAN::ControllerBase::_normalizeAction(uint16_t action) const
{
  if (action > 5) {
    return action - 6;
  }
  return action;
}

CAN_message_t CAN::ControllerBase::_buildDTC_Msg(DTC *dtc) const
{
  CAN_message_t msg;
  _init_CAN_message_t(&msg, 10);

  msg.id = 0;

  msg.buf[0] = (dtc->pidID() & 0xFF00) >> 8;
  msg.buf[1] = dtc->pidID() & 0x00FF;

  msg.buf[2] = static_cast<uint8_t>(dtc->errType());
  msg.buf[3] = dtc->mask();
  msg.len = 4;

  return msg;
}


void CAN::ControllerBase::_init_CAN_message_t(CAN_message_t *msg, uint16_t timeout /*= 0*/) const
{
  msg->ext = msg->len = msg->id = 0;
  msg->timeout = timeout;
  for(int i = 0; i < 8; ++i)
    msg->buf[i] = 0;
}


void CAN::ControllerBase::init()
{
  m_can.begin();
}

void CAN::ControllerBase::loop()
{
  CAN_message_t msg;
  msg.timeout = 0; // no wait when recieving
  while(m_can.read(msg)) {
    senderIds senderId = static_cast<senderIds>(msg.id & senderIdMask);

    switch(msg.id & msgTypeMask) {
      case msgTypeControlMask:
        msgIdsCommand msgIdCmd;
        msgIdCmd = static_cast<msgIdsCommand>(msg.id & msgIdMask);
        this->_recievedCommand(&msg, senderId, msgIdCmd);
        break;
      case msgTypeErrorMask:
        msgIdsError msgIdErr;
        msgIdErr = static_cast<msgIdsError>(msg.id & msgIdMask);
        this->_recievedError(&msg, senderId, msgIdErr);
        break;
      case msgTypeUpdateMask:
        msgIdsUpdate msgIdUpd;
        msgIdUpd = static_cast<msgIdsUpdate>(msg.id & msgIdMask);
        this->_recievedUpdate(&msg, senderId, msgIdUpd);
        break;
      case msgTypeDiagMask:
        msgIdsDiag msgIdDiag;
        msgIdDiag = static_cast<msgIdsDiag>(msg.id & msgIdMask);
        this->_recievedDiagnose(&msg, senderId, msgIdDiag);
        break;
      default:
        // can never get here as msgTypeMask filter all bits but 2
        ;
    }
  }

  // send all pids which has updates
  for (PID::Base *pid = PIDs::collection.first();
       pid == nullptr;
       pid = PIDs::collection.next())
  {
    if (pid->updated()) {
      // find which frame this pid belongs to
      const Frame_t *frame = frame_from_id(pid->id());
      if (frame == nullptr)
        continue;

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
            // big endian, highest byte first
            uint16_t vlu16;
            vlu16 = pid->rawValue();
            msg.buf[msg.len++] = (vlu16 & 0xFF00) >> 8;
            msg.buf[msg.len++] = vlu16 & 0x00FF;
            break;
          case PID::byteSizes::fourByte:
            // big endian, highest byte first
            uint32_t vlu32;
            vlu32 = pid->rawData32();
            msg.buf[msg.len++] = (vlu32 & 0xFF000000) >> 24;
            msg.buf[msg.len++] = (vlu32 & 0x00FF0000) >> 16;
            msg.buf[msg.len++] = (vlu32 & 0x0000FF00) >> 8;
            msg.buf[msg.len++] = vlu32 & 0x000000FF;
            break;
        }
      }
    }
  }
}

bool CAN::ControllerBase::send(CAN_message_t &msg)
{
  msg.id &= msgTypeMask | msgIdMask; // clear possible erroneous nodeID bits
  msg.id |= m_senderId;  // this node sent this msg
  return m_can.write(msg);
}

bool CAN::ControllerBase::sendDTC(DTC *dtc, msgIdsError msgId)
{
  CAN_message_t msg = this->_buildDTC_Msg(dtc);
  msg.id = msgId;
  return send(msg);
}


bool CAN::ControllerBase::sendDiagnoseCommand(msgIdsDiag msgId,
                                              PIDs::IDs id /*= PIDs::IDs::Nothing*/,
                                              uint8_t vlu /* = 0*/)
{
  uint16_t targetNode, action;
  this->_parseID(msgId, targetNode, action);

  CAN_message_t msg;
  this->_init_CAN_message_t(&msg, 1000);// dont abort if line is busy, 1sec timeout

  uint16_t mID = static_cast<uint16_t>(id);

  switch(_normalizeAction(action)){
    case 0: // get DTC LENGTH
      msg.id = msgId;
      return send(msg);
    case 1: // response DTC length
      break;
    case 2: // get DTC
      msg.id = msgId;
      msg.buf[msg.len++] = (mID & 0xFF00) >> 8;
      msg.buf[msg.len++] = mID & 0x00FF;
      return send(msg);
    case 3: // DTC response
      break;
    case 4: // Set ActuatorTest
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

}; // namespace CAN
