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
  C_nodeInValid    = 0x07,
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
    C_suspensionCmdSetRearWheelsUp   = 0x1E << 3, // request: [0:7] 1 = up, 0 = down
                                                  // responds with a userException frame if on error

    C_suspensionCmdSetConfig         = 0x20 << 3,  // configure, byte0=command, byte 1-2 or 1-4 = value
                                                   // request:
                                                   //     [0:7]       [0:16 or 0:31]
                                                   //   Configs id      data (as float or uint)
                                                   // response:
                                                   //     [0:7]         [0:7]
                                                   //   Configs id     result 1 if success, 0 on failure

    C_suspensionCmdGetConfig         = 0x21 << 3,  // get current config, byte 0=command
                                                   // request:
                                                   //   [0:7]
                                                   //  Configs id
                                                   //
                                                   // response:
                                                   //  [0:7]            [0:15 or 0:31]
                                                   // Configs id        data (as float or uint)

    //C_suspensionCmdSaveConfig        = 0x22 << 3,  // Save config to suspensionECU EEPROM

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
                                                  // userError id in enum
    C_parkbrakeExc_LAST            = 0x0F << 3,

    C_suspensionExc_FIRST          = 0x10 << 3,
    C_suspensionExcDTC             = 0x10 << 3,   // a error code has been set in suspension
                                                  //  [0:7]        [0:15]    [0:6] [:7]
                                                  // stored nr     dtc code  occurrences & pending mask (1 on 7th bit = real code, 0=pending)

    C_suspensionExcUserError       = 0x11 << 3,   // action wasn't allowed like when parkbrake during moving
                                                  // sent by parkbrake node when user does something wrong
                                                  // [0:15]
                                                  // userError id in enum

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
    C_suspensionDiagDTCLength       = 0x10 << 3, // request RTR, command to get DTC length
                                                 // response
                                                 //  [0:7] the length(number) of stored dtc's

    C_suspensionDiagGetDTC          = 0x11 << 3, // request [0:7] the DTC index in stored memory
                                                 // response:
                                                 //  [0:7]        [0:15]    [0:6] [:7]
                                                 // stored nr     dtc code  occurrences & pending mask

    C_suspensionDiagClearDTC        = 0x12 << 3, // clears all stored DTCs
                                                 // request [0:7] length of dtcs, must be equal to stored DTCs
                                                 // response  [0:7] number erased

    C_suspensionDiagActuatorTest    = 0x13 << 3, // request:
                                                 //  [0:7]         [0:7]
                                                 // pid adress     value
                                                 //
                                                 // response:
                                                 //  [0:7]         [0:7]
                                                 // pid adress     value
    C_suspensionDiagClearActuatorTest = 0x14 << 3, // request:
                                                   //  [0:7]
                                                   // pid adress
                                                   //
                                                   // response:
                                                   //  [0:7]


    //C_suspensionDiagActuatorStatus  = 0x14 << 3,
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
    C_suspensionUpdPID_1        = 0x20 << 3, // request might be RTR: no paylod on request
                                             // response:
                                             //    [0:7]          [0:7]         [0:7]          [0:7]            [0:7]           [0:7]           [0:7]           [0:7]
                                             // leftFillDuty | leftDumpDuty | leftSuckDuty | rightFillDuty | rightDumpDuty | rightSuckDuty | airDryerDuty | compressorDuty

    C_suspensionUpdPID_2        = 0x21 << 3, // request might be RTR, no payload on request
                                             // response:
                                             //   [0:15]              [0:15]        [0:15]          [0:7]             [0:7]
                                             // systemPressure | leftPressure | rightPressure | compressorCurrent | spare1Duty
                                             //    12bit              12bit          12bit

    C_suspensionUpdPID_3        = 0x22 << 3, // request might be RTR, no payload on request
                                             // response:
                                             //   [0:15]      [0:15]            [0:15]       [0:15]
                                             // leftHeight | rightHeight | compressorTemp | spareAnalog1
                                             //  12bit          12bit           12bit        12bit

    C_suspensionUpdPID_4        = 0x23 << 3, // request might be RTR, no ppayload on request
                                             // response:
                                             //     [0:15]          [0:15]        [0:15]
                                             //  airFeedState    heightState     loadedWeight
                                             //   Pid::States     Pid::States      in kg

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
    // outputs
    C_dtc_leftFill_openLoad         = 0x1101, // or shorted to plus
    C_dtc_leftFill_shorted          = 0x1102, // to ground
    C_dtc_leftFill_overtemp         = 0x1103,
    C_dtc_leftFill_overload         = 0x1104,
    C_dtc_leftFill_hardwarefault    = 0x1105,

    C_dtc_leftDump_openLoad         = 0x1111,
    C_dtc_leftDump_shorted          = 0x1112,
    C_dtc_leftDump_overtemp         = 0x1113,
    C_dtc_leftDump_overload         = 0x1114,
    C_dtc_leftDump_hardwarefault    = 0x1115,

    C_dtc_leftSuck_openLoad         = 0x1121,
    C_dtc_leftSuck_shorted          = 0x1122,
    C_dtc_leftSuck_overtemp         = 0x1123,
    C_dtc_leftSuck_overload         = 0x1124,
    C_dtc_leftSuck_hardwarefault    = 0x1125,

    C_dtc_rightFill_openLoad        = 0x1131,
    C_dtc_rightFill_shorted         = 0x1132,
    C_dtc_rightFill_overtemp        = 0x1133,
    C_dtc_rightFill_overload        = 0x1134,
    C_dtc_rightFill_hardwarefault   = 0x1135,

    C_dtc_rightDump_openLoad        = 0x1141,
    C_dtc_rightDump_shorted         = 0x1142,
    C_dtc_rightDump_overtemp        = 0x1143,
    C_dtc_rightDump_overload        = 0x1144,
    C_dtc_rightDump_hardwarefault   = 0x1145,

    C_dtc_rightSuck_openLoad        = 0x1151,
    C_dtc_rightSuck_shorted         = 0x1152,
    C_dtc_rightSuck_overtemp        = 0x1153,
    C_dtc_rightSuck_overload        = 0x1154,
    C_dtc_rightSuck_hardwarefault   = 0x1155,

    C_dtc_airDryer_openLoad         = 0x1161,
    C_dtc_airDryer_shorted          = 0x1162,
    C_dtc_airDryer_overtemp         = 0x1163,
    C_dtc_airDryer_overload         = 0x1164,
    C_dtc_airDryer_hardwarefault    = 0x1165,

    C_dtc_spare1_openLoad           = 0x1171,
    C_dtc_spare1_shorted            = 0x1172,
    C_dtc_spare1_overtemp           = 0x1173,
    C_dtc_spare1_overload           = 0x1174,
    C_dtc_spare1_hardwarefault      = 0x1175,

    C_dtc_compressor_openLoad       = 0x1181,
    C_dtc_compressor_shorted        = 0x1182,
    C_dtc_compressor_overtemp       = 0x1183,
    C_dtc_compressor_overload       = 0x1184,
    C_dtc_compressor_hardwarefault  = 0x1185,

    // suspension node inputs
    C_dtc_airPressure_nonValidValue   = 0x1200, // non sensical value from a sensor
    C_dtc_airPresssure_tooLowValue    = 0x1201, // value to low to be valid
    C_dtc_airPresssure_tooHighValue   = 0x1202, // value to high to be valid
    C_dtc_airPresssure_tooFastFalling = 0x1203, // when value changes to fast to lower
    C_dtc_airPressure_tooFastRising   = 0x1204, // when value changes to fast to higher value

    C_dtc_leftPressure_nonValidValue   = 0x1210,
    C_dtc_leftPresssure_tooLowValue    = 0x1211,
    C_dtc_leftPresssure_tooHighValue   = 0x1212,
    C_dtc_leftPresssure_tooFastFalling = 0x1213,
    C_dtc_leftPressure_tooFastRising   = 0x1214,

    C_dtc_leftHeight_nonValidValue     = 0x1220,
    C_dtc_leftHeight_tooLowValue       = 0x1221,
    C_dtc_leftHeight_tooHighValue      = 0x1222,
    C_dtc_leftHeight_tooFastFalling    = 0x1223,
    C_dtc_leftHeight_tooFastRising     = 0x1224,

    C_dtc_rightPressure_nonValidValue  = 0x1230,
    C_dtc_rightPresssure_tooLowValue   = 0x1231,
    C_dtc_rightPressure_tooHighValue   = 0x1232,
    C_dtc_rightPressure_tooFastFalling = 0x1233,
    C_dtc_rightPressure_tooFastRising  = 0x1234,

    C_dtc_rightHeight_nonValidValue    = 0x1240,
    C_dtc_rightHeight_tooLowValue      = 0x1241,
    C_dtc_rightHeight_tooHighValue     = 0x1242,
    C_dtc_rightHeight_tooFastFalling   = 0x1243,
    C_dtc_rightHeight_tooFastRising    = 0x1244,

    C_dtc_compressorTemp_nonValidValue   = 0x1250,
    C_dtc_compressorTemp_tooLowValue     = 0x1251,
    C_dtc_compressorTemp_tooHighValue    = 0x1252,
    C_dtc_compressorTemp_tooFastFalling  = 0x1253,
    C_dtc_compressorTemp_tooFastRising   = 0x1254,

    C_dtc_systemPressure_nonValidValue   = 0x1260,
    C_dtc_systemPressure_tooLowValue     = 0x1261,
    C_dtc_systemPressure_tooHighValue    = 0x1262,
    C_dtc_systemPressure_tooFastFalling  = 0x1263,
    C_dtc_systemPressure_tooFastRising   = 0x1264,

    C_dtc_spareAnalog1_nonValidValue     = 0x1270,
    C_dtc_spareAnalog1_tooLowValue       = 0x1271,
    C_dtc_spareAnalog1_tooHighValue      = 0x1272,
    C_dtc_spareAnalog1_tooFastFalling    = 0x1273,
    C_dtc_spareAnalog1_tooFastRising     = 0x1274,

    C_dtc_spareTemp1_nonValidValue       = 0x1280,
    C_dtc_spareTemp1_tooLowValue         = 0x1281,
    C_dtc_spareTemp1_tooHighValue        = 0x1282,
    C_dtc_spareTemp1_tooFastFalling      = 0x1283,
    C_dtc_spareTemp1_tooFastRising       = 0x1284,

    // suspension node airfeed control logic
    C_dtc_airFeed_overCurrent            = 0x1300,
    C_dtc_airFeed_overHeated             = 0x1301,


} can_DTCs_e;


// this enum is for all the user errors (Non DTC worthy exceptions)
typedef enum {
    C_userErrorNone = 0,
    C_userErrorIgnOff = 1,
    C_userErrorBrakeOff = 2,
    C_userErrorBtnInvOff = 3,

    // suspensionNode
    C_userErrorSuckedRearWheelBlocked = 10,
    C_userErrorHeightNonValidState = 11,

    C_userError_OFF
} can_userError_e;


// -----------------------------------------------------------------------------
// CAN pids


#endif /* CAN_PROTOCOL_H_ */
