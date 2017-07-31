/*
 * can_protocol.h
 *
 *  Created on: 31 jul 2017
 *      Author: fredrikjohansson
 */

#ifndef CAN_PROTOCOL_H_
#define CAN_PROTOCOL_H_


enum  {
  // eight possible Nodes in network
  parkbrakeNode  = 0x00,
  suspensionNode = 0x01,
  displayNode    = 0x02,
  node4          = 0x03,
  node5          = 0x04,
  node6          = 0x05,
  node7          = 0x06,
  node8          = 0x07,
  //maxNodeId      = 0x07,
} can_senderIds_e;

#define CAN_MSG_SENDER_ID_MASK          0x0007  /* xxxxxxxx111 */
#define CAN_MSG_ID_MASK                 0x01F8  /* xx111111xxx */
#define CAN_MSG_TYPE_MASK               0x0600  /* 11xxxxxxxxx */
#define CAN_MSG_TYPE_COMMAND_MASK       0x0000  /* 00xxxxxxxxx */
#define CAN_MSG_TYPE_ERROR_MASK         0x0200  /* 01xxxxxxxxx */
#define CAN_MSG_TYPE_UPDATE_MASK        0x0400  /* 10xxxxxxxxx */
#define CAN_MSG_TYPE_DIAG_MASK          0x0600  /* 11xxxxxxxxx */

#define CAN_MSG_TYPE_COMMAND     (0U << 9) /* 00xxxxxxxxx */
#define CAN_MSG_TYPE_ERROR       (1U << 9) /* 01xxxxxxxxx */
#define CAN_MSG_TYPE_UPDATE      (2U << 9) /* 10xxxxxxxxx */
#define CAN_MSG_TYPE_DIAG        (3U << 9) /* 11xxxxxxxxx */

// use to set id of frame
#define canMsgSetId(msg, id) msg &= (id << 3)

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
#define CAN_FRAME_COUNT  64U;
enum  {
    C_NoCommandFrame              = 0,
    C_highestPrioCommand          = 1 << 3, // should not be used

    C_parkbrakeCmd_first          = 0x10 << 3,
    C_parkbrakeSetConfig          = 0x10 << 3, // b0=configvalue in enum, b2:b3 value as uint16_t little endian
                                               // response b0=1 if ok, b0=0 on fail
    C_parkbrakeGetConfig          = 0x11 << 3, // b0=configValue in enum, b2:b3 value as uint16_t little endian
    C_parkbrakeServiceSet         = 0x12 << 3, // responds with same PID and b0 = 1 if success other b0 = 0
    C_parkbrakeServiceUnset       = 0x13 << 3,
    C_parkbrakeCmd_last           = 0x19 << 3,

    C_suspensionCmd_first         = 0x1A << 3,
    C_suspensionSetLow            = 0x1A << 3, //0x0088,
    C_suspensionSetNormal         = 0x1B << 3,
    C_suspensionSetHigh           = 0x1C << 3,
    C_suspensionSetRearWheelsUp   = 0x1E << 3,
    C_suspensionSetRearWheelsDown = 0x1F << 3,

    C_suspensionSetConfig         = 0x20 << 3,  // configure, byte0=command, byte 1-2 or 1-4 = value
    C_suspensionGetConfig         = 0x21 << 3,  // get current config, byte 0=command
    C_suspensionRepondConfig      = 0x22 << 3,  // response to above config, byte0=command, byte 1-2 or 1-4 = value
    C_suspensionSaveConfig        = 0x23 << 3,  // Save config to suspensionECU EEPROM

    C_suspensionCmd_last          = 0x29 << 3,

    C_displayCmd_first            = 0x2A << 3,
    C_displayBeep                 = 0x2A << 3,
    C_displayCmd_last             = 0x2F << 3,

    C_lowestPrioCommand           = 0x3F << 3 // should not be used
} can_msgIdsCommand_e;

// each node start at 0 or 6 to get the receiver node id
// (msgId & msgIdMask) >> 3 to remove senderID bits
// recieverNodeId = msgId & 0xF0
// if (msgId & 0x0F > 5)
//    msgId -= 6
// action = msgId & 0x0F
enum {
    C_NoErrorFrame              = 0,
    C_highestPrioError          = 1 << 3, // should not be used

    C_parkbrakeDTC              = 0x06 << 3,
    C_parkbrakeUserError        = 0x07 << 3,   // action wasn't allowed like when parkbrake during moving

    C_suspensionDTC             = 0x08 << 3,   // a errorcode has been set in suspension
    C_suspensionUserError       = 0x09 << 3,   // user did something stupid
    C_suspensionGotoLowFirst    = 0x0A << 3,   // when user wants sucked rear wheels

    C_displayDTC                = 0x10 << 3,
    C_displayUserEror           = 0x11 << 3,
    C_displayCustomError        = 0x12 << 3,

    C_lowestPrioError           = 0x3F << 3 // should not be used
} can_msgIdsError_e;

enum {
    C_NoDiagFrame               = 0,
    C_highestPrioDiag           = 1 << 3, // should not be used

    C_parkbrakeGetDTCLength     = 0x06 << 3,  // command to get DTC length
    C_parkbrakeDTCLength        = 0x07 << 3,  // the response to above
    C_parkbrakeGetDTC           = 0x08 << 3,
    C_parkbrakeDTCResponse      = 0x09 << 3,
    C_parkbrakeActuatorTest     = 0x0A << 3,
    C_parkbrakeActuatorStatus   = 0x0B << 3,

    C_suspensionGetDTCLength    = 0x10 << 3,
    C_suspensionDTCLength       = 0x11 << 3,
    C_suspensionGetDTC          = 0x12 << 3,
    C_suspensionDTCResponse     = 0x13 << 3,
    C_suspensionActuatorTest    = 0x14 << 3,
    C_suspensionActuatorStatus  = 0x15 << 3,

    C_displayGetDTCLength       = 0x1A << 3,
    C_displayDTCLength          = 0x1B << 3,
    C_displayGetDTC             = 0x1C << 3,
    C_displayDTCResponse        = 0x1D << 3,
    C_displayActuatorTest       = 0x1E << 3,
    C_displayActuatorStatus     = 0x1F << 3,

    C_lowestPrioDiag            = 0x3F << 3, // should not be used
} can_msgIdsDiag_e;



enum {
    C_NoUpdateFrame             = 0,
    C_highestPrioUpdate         = 1 << 3, // should not be used

    C_parkbrakePID_FIRST        = 0x10 << 3,
    C_parkbrakePID_1            = 0x10 << 3,
    C_parkbrakePID_2            = 0x11 << 3,
    C_parkbrakePID_3            = 0x12 << 3,
    C_parkbrakePID_LAST         = 0x1F << 3,

    C_suspensionPID_FIRST       = 0x20 << 3,
    C_suspensionPID_1           = 0x20 << 3,
    C_suspensionPID_2           = 0x21 << 3,
    C_suspensionPID_3           = 0x22 << 3,
    C_suspensionPID_4           = 0x23 << 3,
    C_suspensionPID_LAST        = 0x2F << 3,

    C_displayPID_FIRST          = 0x30 << 3,

    C_lowestPrioUpdate          = 0x3F << 3, // should not be used
} can_msgIdsUpdate_e;

#endif /* CAN_PROTOCOL_H_ */
