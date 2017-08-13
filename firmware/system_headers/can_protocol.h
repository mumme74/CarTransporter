/*
 * can_protocol.h
 *
 *  Created on: 31 jul 2017
 *      Author: fredrikjohansson
 */

#ifndef CAN_PROTOCOL_H_
#define CAN_PROTOCOL_H_

typedef enum  {
  // eight possible Nodes in network
  C_parkbrakeNode  = 0x00,
  C_suspensionNode = 0x01,
  C_displayNode    = 0x02,
  C_node4          = 0x03,
  C_node5          = 0x04,
  C_node6          = 0x05,
  C_node7          = 0x06,
  C_node8          = 0x07,
  //maxNodeId      = 0x07,
} can_senderIds_e;

#define CAN_MSG_SENDER_ID_MASK          0x0007  /* xxxxxxxx111 */
#define CAN_MSG_ID_MASK                 0x01F8  /* xx111111xxx */
#define CAN_MSG_TYPE_MASK               0x0600  /* 11xxxxxxxxx */
#define CAN_MSG_TYPE_COMMAND_MASK       0x0000  /* 00xxxxxxxxx */
#define CAN_MSG_TYPE_EXCEPTION_MASK     0x0200  /* 01xxxxxxxxx */
#define CAN_MSG_TYPE_UPDATE_MASK        0x0400  /* 10xxxxxxxxx */
#define CAN_MSG_TYPE_DIAG_MASK          0x0600  /* 11xxxxxxxxx */

#define CAN_MSG_TYPE_COMMAND     (0U << 9) /* 00xxxxxxxxx */
#define CAN_MSG_TYPE_EXCEPTION   (1U << 9) /* 01xxxxxxxxx */
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
typedef enum  {
    C_NoCommandFrame                 = 0,
    C_highestPrioCommand             = 1 << 3, // should not be used

    C_parkbrakeCmd_first             = 0x10 << 3,

    C_parkbrakeCmdSetConfig          = 0x10 << 3, // request: b0=configvalue in enum, b2:b3 value as uint16_t little endian
                                                  // response:
                                                  //  b0=configvalue in enum, b1=1 if ok, b1=0 on fail
    C_parkbrakeCmdGetConfig          = 0x11 << 3, // request b0=configValue in enum
                                                  // response:
                                                  // b0=configValue in enum, b2:b3 value as uint16_t little endian
    C_parkbrakeCmdServiceSet         = 0x12 << 3, // RTR, Look for update on PID to see if state changed, asyncronous
    C_parkbrakeCmdServiceUnset       = 0x13 << 3, // RTR, --" same as above "--
    C_parkbrakeCmdGetState           = 0x14 << 3, // RTR, get the state of each wheel,
                                                  // b0=LF,b1=RF,b2=LR,b3=RR, State = ctrl_states 0=tightened, 1Released, 8 error etc
    C_parkbrakeCmd_last              = 0x19 << 3,

    C_suspensionCmd_first            = 0x1A << 3,
    C_suspensionCmdSetLow            = 0x1A << 3, //0x0088,
    C_suspensionCmdSetNormal         = 0x1B << 3,
    C_suspensionCmdSetHigh           = 0x1C << 3,
    C_suspensionCmdSetRearWheelsUp   = 0x1E << 3,
    C_suspensionCmdSetRearWheelsDown = 0x1F << 3,

    C_suspensionCmdSetConfig         = 0x20 << 3,  // configure, byte0=command, byte 1-2 or 1-4 = value
    C_suspensionCmdGetConfig         = 0x21 << 3,  // get current config, byte 0=command
    C_suspensionCmdRepondConfig      = 0x22 << 3,  // response to above config, byte0=command, byte 1-2 or 1-4 = value
    C_suspensionCmdSaveConfig        = 0x23 << 3,  // Save config to suspensionECU EEPROM

    C_suspensionCmd_last             = 0x29 << 3,

    C_displayCmd_first               = 0x2A << 3,
    C_displayCmdBeep                 = 0x2A << 3,
    C_displayCmd_last                = 0x2F << 3,

    C_lowestPrioCommand              = 0x3F << 3 // should not be used
} can_msgIdsCommand_e;

// each node start at 0 or 6 to get the receiver node id
// (msgId & msgIdMask) >> 3 to remove senderID bits
// recieverNodeId = msgId & 0xF0
// if (msgId & 0x0F > 5)
//    msgId -= 6
// action = msgId & 0x0F
typedef enum {
    C_NoExcFrame                   = 0,
    C_highestPrioExc               = 1 << 3,      // should not be used

    C_parkbrakeExc_FIRST           = 0x06 << 3,
    C_parkbrakeExcNewDTC           = 0x06 << 3,   // a new DTC has been set
                                                  // broadcasted when a dtc is set
                                                  // response
                                                  //  [0:7]        [0:15]    [0:7]
                                                  // stored nr     dtc code  occurrences

    C_parkbrakeExcUserError        = 0x07 << 3,   // action wasn't allowed like when parkbrake during moving
                                                  // sent by parkbrake node when user does something wrong
                                                  // [0:15]
                                                  // usererror id in enum
    C_parkbrakeExc_LAST            = 0x0F << 3,

    C_suspensionExc_FIRST          = 0x10 << 3,
    C_suspensionExcDTC             = 0x10 << 3,   // a error code has been set in suspension
    C_suspensionExcUserError       = 0x11 << 3,   // user did something stupid
    C_suspensionExcGotoLowFirst    = 0x12 << 3,   // when user wants sucked rear wheels
    C_suspensionExc_Last           = 0x19 << 3,

    C_displayExc_FIRST             = 0x1A << 3,
    C_displayExcDTC                = 0x1A << 3,
    C_displayExcUserEror           = 0x1B << 3,
    C_displayExcCustomError        = 0x1C << 3,
    C_displayExc_LAST              = 0x1F << 3,

    C_lowestPrioExc                = 0x3F << 3 // should not be used
} can_msgIdsException_e;

typedef enum {
    C_NoDiagFrame                   = 0,
    C_highestPrioDiag               = 1 << 3, // should not be used

    C_parkbrakeDiag_FIRST           = 0x06 << 3,
    C_parkbrakeDiagDTCLength        = 0x06 << 3,  // request RTR, command to get DTC length
                                                  // response
                                                  //  [0:7] the length(number) of stored dtc's
    C_parkbrakeDiagDTC              = 0x07 << 3, // request [0:7] the DTC index in stored memory
                                                 // response:
                                                 //  [0:7]        [0:15]    [0:7]        [0:15]
                                                 // stored nr     dtc code  occurrences  time since startup when set

    C_parkbrakeDiagDTCFreezeFrame   = 0x08 << 3, // request [0:7] the DTC index in stored memory
                                                 // response:
                                                 //  [0:7]        [0:7]      [0:48]
                                                 // stored nr    frame nr    data payload
                                                 //           /- frame0:     [0:7][0:7][0:7][0:7]
                                                 //          /    state brakes LF  RF   LR    RR
                                                 //     pid1:\   frame1:     [0:7][0:7][0:7][0:7]
                                                 //           \-  wheelspeed   LF   RF   LR   RR
                                                 //           /- frame2:     [0:7][0:7][0:7][0:7]
                                                 //          /    amps mom.    LF   RF  LR   RR
                                                 //     pid2:\   frame3:     [0:7][0:7][0:7][0:7]
                                                 //           \-  amps max.    LF   RF  LR   RR
                                                 //     pid3:    frame4:     [0:15][0:15][0:7] [0-7]
                                                 //                           batV  ignV  temp  input states
                                                 //     last byte frame 4:
                                                 //                [ 7 -   6   -  5  -  4  -   3  -   2  -  1  -   0  ]
                                                 //                IGN|B.LIGHTS|BTN|BTN_INV|LF_LIM|RF_LIM|LR_LIM|RR_LIM

    C_parkbrakeDiagClearDTCs        = 0x09 << 3, // clears all stored DTCs
                                                 // request [0:7] length of dtcs
                                                 // response  [0:7] number erased
    C_parkbrakeDiagActuatorTest     = 0x0A << 3, // request: t= tighten
                                                 //  [0  -  1  -  2  -  3  -  4  -  5  -  6  -  7]
                                                 //  t-LF t-RF   t-LR  t-RR  r-LF  r-RF  r-LR  r-RR
    C_parkbrakeDiag_LAST            = 0x0F << 3,

    C_suspensionDiag_FIRST          = 0x10 << 3,
    C_suspensionDiagGetDTCLength    = 0x10 << 3,
    C_suspensionDiagDTCLength       = 0x11 << 3,
    C_suspensionDiagGetDTC          = 0x12 << 3,
    C_suspensionDiagDTCResponse     = 0x13 << 3,
    C_suspensionDiagActuatorTest    = 0x14 << 3,
    C_suspensionDiagActuatorStatus  = 0x15 << 3,
    C_suspensionDiag_LAST           = 0x19 << 3,

    C_displayDiag_FIRST             = 0x1A << 3,
    C_displayDiagGetDTCLength       = 0x1A << 3,
    C_displayDiagDTCLength          = 0x1B << 3,
    C_displayDiagGetDTC             = 0x1C << 3,
    C_displayDiagDTCResponse        = 0x1D << 3,
    C_displayDiagActuatorTest       = 0x1E << 3,
    C_displayDiagActuatorStatus     = 0x1F << 3,
    C_displayDiag_LAST              = 0x24 << 3,

    C_lowestPrioDiag                = 0x3F << 3, // should not be used
} can_msgIdsDiag_e;

/**
 * PIDs documentation, databytes is documented within brackets
 * like |   first PID   ||   next PID    |
 *      [begin - end bit][begin - end bit]
 *
 * when no type is given its a unsigned int
 * else i = signed int, like i[0:8] = int8_t
 */

typedef enum {
    C_NoUpdateFrame             = 0,
    C_highestPrioUpdate         = 1 << 3, // should not be used

    C_parkbrakeUpdPID_FIRST     = 0x10 << 3,

    C_parkbrakeUpdPID_1         = 0x10 << 3, // state wheels brakes | revs / sec on wheels
                                             // [0:7][0:7][0:7][0:7] [0:7][0:7][0:7][0:7]
                                             //  LF   RF   LR   RR    LF    RF  LR   RR
                                             //  each 500ms

    C_parkbrakeUpdPID_2         = 0x11 << 3, // amps momentary      |  amps maximum this seq.
                                             // [0:7][0:7][0:7][0:7] [0:7][0:7][0:7][0:7]
                                             //  LF   RF   LR   RR    LF   RF   LR   RR
                                             //  each 100ms during maneuvering

    C_parkbrakeUpdPID_3         = 0x12 << 3, // bat volt*1000|ign volt*1000|chip temp
                                             //   [0:15]         [0:15]    i[0:7] -> continue on next row

                                             // IGN|B.LIGHTS|BTN|BTN_INV|LF_LIM|RF_LIM|LR_LIM|RR_LIM
                                             // [7 -   6   -  5  -  4  -   3  -   2  -  1  -   0  ]
                                             //  bits so the whole data is 5bytes + 8bit = 6bytes
                                             //  each 500ms

    C_parkbrakeUpdPID_LAST      = 0x1F << 3,

    C_suspensionUpdPID_FIRST    = 0x20 << 3,
    C_suspensionUpdPID_1        = 0x20 << 3,
    C_suspensionUpdPID_2        = 0x21 << 3,
    C_suspensionUpdPID_3        = 0x22 << 3,
    C_suspensionUpdPID_4        = 0x23 << 3,
    C_suspensionUpdPID_LAST     = 0x2F << 3,

    C_displayUpdPID_FIRST       = 0x30 << 3,

    C_lowestPrioUpdate          = 0x3F << 3, // should not be used
} can_msgIdsUpdate_e;


// all trouble codes in this system
// Read more at: https://www.obd-codes.com/trouble_codes/obd-ii-c-chassis-codes.php
typedef enum {
    // parkbrake node ----------------------------------------------------------------------
    C_dtc_LF_wheelsensor            = 0x0035, // Left front wheel sensor circuit malfunction
    C_dtc_RF_wheelsensor            = 0x0040, // Right front wheel sensor circuit malfunction
    C_dtc_LR_wheelsensor            = 0x0045, // Left rear wheel sensor circuit malfunction
    C_dtc_RR_wheelsensor            = 0x0050, // Right rear wheel sensor circuit malfunction

    C_dtc_ADC_error_LF_release      = 0x1010, //
    C_dtc_ADC_error_RF_release      = 0x1011,
    C_dtc_ADC_error_LR_release      = 0x1012,
    C_dtc_ADC_error_RR_release      = 0x1013,

    C_dtc_OverCurrent_LF_release    = 0x1020,
    C_dtc_OverCurrent_RF_release    = 0x1021,
    C_dtc_OverCurrent_LR_release    = 0x1022,
    C_dtc_OverCurrent_RR_release    = 0x1023,

    C_dtc_ADC_error_LF_tighten      = 0x1030, //
    C_dtc_ADC_error_RF_tighten      = 0x1031,
    C_dtc_ADC_error_LR_tighten      = 0x1032,
    C_dtc_ADC_error_RR_tighten      = 0x1033,

    C_dtc_OverCurrent_LF_tighten    = 0x1040,
    C_dtc_OverCurrent_RF_tighten    = 0x1041,
    C_dtc_OverCurrent_LR_tighten    = 0x1042,
    C_dtc_OverCurrent_RR_tighten    = 0x1043,

    C_dtc_ADC_error_LF_service      = 0x1050, //
    C_dtc_ADC_error_RF_service      = 0x1051,
    C_dtc_ADC_error_LR_service      = 0x1052,
    C_dtc_ADC_error_RR_service      = 0x1053,

    C_dtc_OverCurrent_LF_service    = 0x1060,
    C_dtc_OverCurrent_RF_service    = 0x1061,
    C_dtc_OverCurrent_LR_service    = 0x1062,
    C_dtc_OverCurrent_RR_service    = 0x1063,


    // ------------------------------------------------------------------------------------
    // suspension node


} can_DTCs_e;


// this enum is for all the user errors (Non DTC worthy exceptions)
typedef enum {
    C_userErrorNone = 0,
    C_userErrorIgnOff = 1,
    C_userErrorBrakeOff = 2,
    C_userErrorBtnInvOff = 3,

    C_userError_OFF
} can_userError_e;


// -----------------------------------------------------------------------------
// CAN pids


#endif /* CAN_PROTOCOL_H_ */
