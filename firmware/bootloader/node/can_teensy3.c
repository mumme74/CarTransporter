/*
 * can_teensy3.c
 *
 *  Created on: 2 jul 2019
 *      Author: jof
 */

#include <Arduino.h>
#include <kinetis.h>
#include <usb_serial.h>
//#include <core_cm4.h>
#include "system.h"
#include "can.h"
#include "fifo.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

typedef volatile uint32_t vuint32_t;
typedef volatile uint8_t  vuint8_t;

#define IRQ_PRIORITY    64 //0 = highest, 255 = lowest


#define FLEXCANb_CTRL1(b)                 (*(vuint32_t*)(b+4))
#define FLEXCANb_IMASK1(b)                (*(vuint32_t*)(b+0x28))
#define FLEXCANb_MBn_WORD0(b, n)          (*(vuint32_t*)(b+0x88+n*0x10))
#define FLEXCANb_MBn_WORD1(b, n)          (*(vuint32_t*)(b+0x8C+n*0x10))
#define FLEXCANb_MCR(b)                   (*(vuint32_t*)(b))
#define FLEXCANb_IFLAG1(b)                (*(vuint32_t*)(b+0x30))
#define FLEXCAN_MB_CS_LENGTH(x)       (((x)&0x0000000F)<<16)
#define FLEXCAN_MB_CS_CODE(x)         (((x)&0x0000000F)<<24)
#define FLEXCAN_MB_CS_CODE_MASK		(0x0F000000L)
#define FLEXCAN_MB_CS_DLC_MASK			(0x000F0000L)
#define FLEXCANb_MBn_CS(b, n)             (*(vuint32_t*)(b+0x80+n*0x10))
#define FLEXCANb_MBn_WORD0(b, n)          (*(vuint32_t*)(b+0x88+n*0x10))
#define FLEXCAN_get_code(cs)				(((cs) & FLEXCAN_MB_CS_CODE_MASK)>>24)
#define FLEXCAN_CTRL_RJW(x)            (((x)&0x00000003L)<<22)
#define FLEXCAN_CTRL_PSEG2(x)          (((x)&0x00000007L)<<16)
#define FLEXCAN_CTRL_PSEG1(x)          (((x)&0x00000007L)<<19)
#define FLEXCAN_CTRL_PRESDIV(x)        (((x)&0x000000FFL)<<24)
#define FLEXCANb_MB_MASK(b, n)            (*(vuint32_t*)(b+0x880+(n*4)))
#define FLEXCAN_MB_ID_IDEXT(x)        (((x)&0x0003FFFF)<<0)
#define FLEXCAN_MB_ID_IDSTD(x)        (((x)&0x000007FF)<<18)
#define FLEXCAN_MB_ID_PRIO(x)         (((x)&0x00000007)<<29)
#define FLEXCANb_MBn_ID(b, n)             (*(vuint32_t*)(b+0x84+n*0x10))
/*
#define FLEXCANb_RXMGMASK(b)              (*(vuint32_t*)(b+0x10))
#define FLEXCANb_RXFGMASK(b)              (*(vuint32_t*)(b+0x48))
#define FLEXCANb_MBn_WORD0(b, n)          (*(vuint32_t*)(b+0x88+n*0x10))
#define FLEXCANb_MBn_WORD1(b, n)          (*(vuint32_t*)(b+0x8C+n*0x10))
#define FLEXCANb_IDFLT_TAB(b, n)          (*(vuint32_t*)(b+0xE0+(n*4)))
*/

#define FLEXCAN_MCR_HALT               (0x10000000)
#define FLEXCAN_MCR_FRZ_ACK            (0x01000000)
#define FLEXCAN_MCR_SOFT_RST           (0x02000000)
#define FLEXCAN_CTRL_CLK_SRC           (0x00002000)
#define FLEXCAN_CTRL_BOFF_MSK          (0x00008000)
#define FLEXCAN_MCR_FRZ                (0x40000000)
#define FLEXCAN_MCR_MDIS               (0x80000000)
#define FLEXCAN_MCR_LPM_ACK            (0x00100000)
#define FLEXCAN_MCR_SRX_DIS            (0x00020000)
#define FLEXCAN_MCR_IRMQ               (0x00010000)
#define FLEXCAN_MCR_NOT_RDY            (0x08000000)
#define FLEXCAN_MB_CS_IDE             (0x00200000)
#define FLEXCAN_MB_CS_SRR             (0x00400000)
#define FLEXCAN_MB_CS_RTR             (0x00100000)
#define FLEXCAN_MB_CODE_RX_FULL		(2)
#define FLEXCAN_MB_CODE_RX_OVERRUN		(6)
#define FLEXCAN_MB_CODE_TX_INACTIVE	(8)
#define FLEXCAN_MB_CODE_TX_ABORT		(9)
#define FLEXCAN_MB_CODE_RX_EMPTY		(4)
#define FLEXCAN_MB_CODE_TX_ONCE			(0x0C)
#define FLEXCAN_MB_ID_STD_MASK		   (0x1FFC0000L)
#define FLEXCAN_MB_ID_EXT_MASK		   (0x1FFFFFFFL)
#define FLEXCAN_MB_ID_STD_BIT_NO	(18)
#define FLEXCAN_CTRL_PROPSEG(x)        (((x)&0x00000007L)<<0)
#define NVIC_STIR_INTID_Pos                 0                                          /*!< STIR: INTLINESNUM Position */
#define NVIC_STIR_INTID_Msk                (0x1FFUL << NVIC_STIR_INTID_Pos)            /*!< STIR: INTLINESNUM Mask */
#define FLEXCAN_get_code(cs)				(((cs) & FLEXCAN_MB_CS_CODE_MASK)>>24)
#define FLEXCAN_get_length(cs)             (((cs) & FLEXCAN_MB_CS_DLC_MASK)>>16)

//Software Trigger Interrupt Register
//#define NVIC_BASE                         (0xE000E000UL +  0x0100UL)
//#define NVIC_STIR                         (*(vuint32_t*)(NVIC_BASE+0xE00))

#define FLEXCAN0_BASE			 (0x40024000L)

// interupt numbers
#if defined(__MK20DX256__)
# define CAN0_MSG_ISRn		 45 /* 45 CAN OR'ed Message buffer (0-15) */
# define CAN0_BUS_OFF_ISRn	 46 /* 46 CAN Bus Off */
# define CAN0_ERROR_ISRn         47 /* 47 CAN Error */
# define CAN0_TX_WARN_ISRn       48 /* 47 CAN Error */
# define CAN0_RX_WARN_ISRn       49 /* 47 CAN Error */
# define CAN0_WAKEUP_ISRn        50 /* 47 CAN Error */
#elif defined(__MKL26Z64__) || defined(__MK66FX1M0__)
# define CAN0_MSG_ISRn		 91 /* 45 CAN OR'ed Message buffer (0-15) */
# define CAN0_BUS_OFF_ISRn	 92 /* 46 CAN Bus Off */
# define CAN0_ERROR_ISRn         93 /* 47 CAN Error */
# define CAN0_TX_WARN_ISRn       94 /* 47 CAN Error */
# define CAN0_RX_WARN_ISRn       95 /* 47 CAN Error */
# define CAN0_WAKEUP_ISRn        96 /* 47 CAN Error */
#endif

// we do our own flexcan here as the lib seems to drop a frame occationally
// also we reduce bin size

extern fifo_t can_rxqueue, can_txqueue; // is visible to global from can.c


// prototypes
int8_t _canPost(canframe_t *msg);
void _canInit(void);
void _canLoop(void);
bool _canGet(void);

typedef enum {
  _RX_start,
  RX0 = _RX_start,
  RX1,
  RX2,
  RX3,
  _RX_end = RX3,
  _TX_start,
  TX0 = _TX_start,
  _TX_end = TX0 // only 1 tx box so we dont get race issues
  //TX1,
  //TX2,
  //TX3,
  //_TX_end = TX3
} mailboxes_t;

static uint32_t canFilter = 0,
	        canMask   = 0x7FF;

// -----------------------------------------------------------
// public functions and variables

void canShutdown(void)
{
  // enter freeze mode
  FLEXCANb_MCR(FLEXCAN0_BASE) |= (FLEXCAN_MCR_HALT);
  while(!(FLEXCANb_MCR(FLEXCAN0_BASE) & FLEXCAN_MCR_FRZ_ACK))
    ;
}

void canDisableIRQ(void)
{
  NVIC_DISABLE_IRQ(IRQ_CAN_MESSAGE);
}
void canEnableIRQ(void)
{
  NVIC_ENABLE_IRQ(IRQ_CAN_MESSAGE);
}



// -----------------------------------------------------------
// architecture specific from here on, treat as private
static void writeTx(const canframe_t *frm, uint8_t buffer);

void _canInit(void)
{
//  const uint8_t CAN_TX_BOX_CNT = 2;
//  Can0.begin(250000); // 250kHz
//  Can0.setNumTXBoxes(CAN_TX_BOX_CNT);
//  for (int i = 0; i < NUM_MAILBOXES - CAN_TX_BOX_CNT; ++i) {
//    Can0.setFilter(canFilter1, i);
//    Can0.setFilter(canFilter2, i);
//  }

  canFilter = (canId & (CAN_MSG_ID_MASK | CAN_MSG_TYPE_MASK))
					    | C_displayNode;
  canMask   = 0x7F8;

  // these are taken from FlexCAN.cpp
  CORE_PIN3_CONFIG = PORT_PCR_MUX(2);
  CORE_PIN4_CONFIG = PORT_PCR_MUX(2);
  // select clock source 16MHz xtal
  OSC0_CR |= OSC_ERCLKEN;
  SIM_SCGC6 |=  SIM_SCGC6_FLEXCAN0;
  FLEXCANb_CTRL1(FLEXCAN0_BASE) &= ~FLEXCAN_CTRL_CLK_SRC;

  // enable CAN
  FLEXCANb_MCR(FLEXCAN0_BASE) |=  FLEXCAN_MCR_FRZ;
  FLEXCANb_MCR(FLEXCAN0_BASE) &= ~FLEXCAN_MCR_MDIS;
  while(FLEXCANb_MCR(FLEXCAN0_BASE) & FLEXCAN_MCR_LPM_ACK)
    ;
  // soft reset
  FLEXCANb_MCR(FLEXCAN0_BASE) ^=  FLEXCAN_MCR_SOFT_RST;
  while(FLEXCANb_MCR(FLEXCAN0_BASE) & FLEXCAN_MCR_SOFT_RST)
    ;
  // wait for freeze ack
  while(!(FLEXCANb_MCR(FLEXCAN0_BASE) & FLEXCAN_MCR_FRZ_ACK))
    ;

  // disable self-reception
  FLEXCANb_MCR(FLEXCAN0_BASE) |= FLEXCAN_MCR_SRX_DIS;

  // we want 250_000baud
  // 16_000_000 hz / 250_000 / 4 = 16
  // {2,7,3}, //16 // from FlexCAN.cpp bittimingtable
  FLEXCANb_CTRL1(FLEXCAN0_BASE) = (FLEXCAN_CTRL_PROPSEG(2) |
                                FLEXCAN_CTRL_BOFF_MSK |
                                FLEXCAN_CTRL_RJW(1) |
                                FLEXCAN_CTRL_PSEG1(7) |
                                FLEXCAN_CTRL_PSEG2(3) |
                                FLEXCAN_CTRL_PRESDIV(3));

  // enable per-mailbox filtering
  FLEXCANb_MCR(FLEXCAN0_BASE) |= FLEXCAN_MCR_IRMQ;

  // set filter and mask
  // we must be in freeze mode, but we assured we are above
  for (int c = _RX_start; c < _TX_start; ++c) {
      // Set mask
      FLEXCANb_MB_MASK(FLEXCAN0_BASE, c) = canMask;

      // set Filter, should not listen to extended frames
      FLEXCANb_MBn_ID(FLEXCAN0_BASE, c) = FLEXCAN_MB_ID_IDSTD(canFilter);
      FLEXCANb_MBn_CS(FLEXCAN0_BASE, c) &= ~FLEXCAN_MB_CS_IDE;
  }

  // start the CAN
  FLEXCANb_MCR(FLEXCAN0_BASE) &= ~(FLEXCAN_MCR_HALT);
  // wait till exit of freeze mode
  while(FLEXCANb_MCR(FLEXCAN0_BASE) & FLEXCAN_MCR_FRZ_ACK);

  // wait till ready
  while(FLEXCANb_MCR(FLEXCAN0_BASE) & FLEXCAN_MCR_NOT_RDY);

  // set up mailboxes
  //Inialize RX
  for (mailboxes_t mb = _RX_start; mb <= _RX_end; ++mb)
      FLEXCANb_MBn_CS(FLEXCAN0_BASE, mb) = FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_RX_EMPTY);

  //Initialize TX
  for (mailboxes_t mb  = _TX_start; mb <= _TX_end; ++mb)
      FLEXCANb_MBn_CS(FLEXCAN0_BASE, mb) = FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_TX_INACTIVE);


#ifdef __MK20DX256__
  // teensy3.2
  NVIC_SET_PRIORITY(IRQ_CAN_MESSAGE, IRQ_PRIORITY);
  NVIC_ENABLE_IRQ(IRQ_CAN_MESSAGE);
#else
  // teensy 3.5 - 3.6?
  NVIC_SET_PRIORITY(IRQ_CAN0_MESSAGE, IRQ_PRIORITY);
  NVIC_ENABLE_IRQ(IRQ_CAN0_MESSAGE);
#endif


  FLEXCANb_IMASK1(FLEXCAN0_BASE) = 0xFFFF; //enable interrupt masks for all m.boxes
}

bool _canGet(void)
{
  digitalWrite(6, !digitalRead(6));
  if (usb_serial_available())
    usb_serial_flush_output();
  return false;
}


// check if we have any available mailboxes already
int8_t _canPost(canframe_t *msg)
{
  canDisableIRQ();
  for (uint8_t i = _TX_start; i <= _TX_end; ++i) {
      uint32_t code = FLEXCAN_get_code(FLEXCANb_MBn_CS(FLEXCAN0_BASE, i));
      //print_uint(code);endl();
      switch(code) {
      //case FLEXCAN_MB_CODE_TX_ABORT:
      case FLEXCAN_MB_CODE_RX_EMPTY: // fallthrough
      case FLEXCAN_MB_CODE_TX_INACTIVE: {
	 writeTx(msg, i);
	 // generate a software interupt
	 //NVIC_STIR =  CAN0_MSG_ISRn | NVIC_STIR_INTID_Msk;
	 canEnableIRQ();
	 return 1;
      } break;
      default: ;
      }
  }
  //print_str("tx failed\n");
  //print_flush();
  canEnableIRQ();
  return 0;
}

// NOTE !! this is called from isr, not used by normal code
static void readMB(canframe_t *frm, uint8_t buffer, fifo_t *queue)
{
  // get identifier and dlc
  frm->DLC = FLEXCAN_get_length(FLEXCANb_MBn_CS(FLEXCAN0_BASE, buffer));
  frm->ext = (FLEXCANb_MBn_CS(FLEXCAN0_BASE, buffer) & FLEXCAN_MB_CS_IDE)? 1:0;
  frm->rtr = (FLEXCANb_MBn_CS(FLEXCAN0_BASE, buffer) & FLEXCAN_MB_CS_RTR)? 1:0;
  frm->EID = (FLEXCANb_MBn_ID(FLEXCAN0_BASE, buffer) & FLEXCAN_MB_ID_EXT_MASK);
  if(!frm->ext)
    frm->EID >>= FLEXCAN_MB_ID_STD_BIT_NO;

  // get the message
  // these are big endian in register, byte trickery to reverse them
  u32to8_t w0 = { FLEXCANb_MBn_WORD0(FLEXCAN0_BASE, buffer) },
	   w1 = { FLEXCANb_MBn_WORD1(FLEXCAN0_BASE, buffer) };
  for (int s = 0, e = 3; s < 4 && frm->DLC > s; ++s, --e)
      frm->data8[s] = w0.v8[e];
  for (int s = 4, e = 3; s < 8 && frm->DLC > s; ++s, --e)
      frm->data8[s] = w1.v8[e];
  FLEXCANb_MBn_WORD0(FLEXCAN0_BASE, buffer) = w0.v32;
  FLEXCANb_MBn_WORD1(FLEXCAN0_BASE, buffer) = w1.v32;

  // check if we are full
  if (fifo_spaceleft(queue) < 2)// need 2 as 1 + 1 would reset (start over algorithm assumes empty)
    return;
  /*
  print_str("rcv isr dlc=");
  print_uint(frm->DLC);print_str(" ");
  for(int i = 0; i < frm->DLC; ++i)
    print_uint(frm->data8[i]);
  endl();
  print_flush();
  */

  fifo_push(queue, frm);
}

// NOTE! this is called from is, do not use from normal code
static void writeTx(const canframe_t *frm, uint8_t buffer)
{
  // transmit the frame

  // clear interupt flag, must done first
  // 44.4.1 Transmit process
  FLEXCANb_IFLAG1(FLEXCAN0_BASE) |= (1 << buffer);

  FLEXCANb_MBn_CS(FLEXCAN0_BASE, buffer) = FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_TX_INACTIVE);
  if(frm->ext) {
    FLEXCANb_MBn_ID(FLEXCAN0_BASE, buffer) = (frm->EID & FLEXCAN_MB_ID_EXT_MASK);
  } else {
    FLEXCANb_MBn_ID(FLEXCAN0_BASE, buffer) = FLEXCAN_MB_ID_IDSTD(frm->IDE);
  }

  // insert data to transmit Mailbox, its big endian
  // these are big endian in register, byte trickery to reverse them
  u32to8_t w0, w1;
  for (int s = 0, e = 3; s < 4; ++s, --e)
    w0.v8[e] = frm->data8[s];
  for (int s = 4, e = 3; s < 8; ++s, --e)
    w1.v8[e] = frm->data8[s];
  FLEXCANb_MBn_WORD0(FLEXCAN0_BASE, buffer) = w0.v32;
  FLEXCANb_MBn_WORD1(FLEXCAN0_BASE, buffer) = w1.v32;
/*
  print_str("post isr");
  print_uint(frm->data8[0]);endl();
  digitalWrite(8, !digitalRead(8));
  */

  uint32_t flags = FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_TX_ONCE) |
                                         FLEXCAN_MB_CS_LENGTH(frm->DLC);
  if (frm->ext)
    flags |= FLEXCAN_MB_CS_IDE | FLEXCAN_MB_CS_SRR;
  if (frm->rtr)
    flags |= FLEXCAN_MB_CS_RTR;
  // set the flags and be done
  FLEXCANb_MBn_CS(FLEXCAN0_BASE, buffer) = flags;
}


void can0_message_isr(void) {
  //a message either came in or was freshly sent. Figure out which and act accordingly.
    uint32_t status = FLEXCANb_IFLAG1(FLEXCAN0_BASE);
    canframe_t frm;
    //bool caughtFrame;

    for (int i = 0; i < 16; i++) if (status & (1 << i)) //has this mailbox triggered an interrupt?
    {
        uint32_t code = FLEXCAN_get_code(FLEXCANb_MBn_CS(FLEXCAN0_BASE, i));
        switch (code)
        {
        /* //these codes exist but aren't useful here as far as I know. Just kept for reference and in case they're needed some day.
        case 0: //inactive Receive box. Must be a false alarm!?
            break;
        case 1: //mailbox is busy. Don't touch it.
            break;
        case 4: //rx empty already. Why did it interrupt then?
            break;
        case 9: //TX being aborted. if its a RX then it has an answer to a rtr
	    break;
        case 0xA: //remote request response. Remote request is deprecated and I don't care about it. Be gone!
            break;
        case 0xC: //TX mailbox is full and will be sent as soon as possible, then goes to inactive
            break;
        case 0xE: //remote request junk again. Go away.
            break;
        */
        case FLEXCAN_MB_CODE_RX_FULL: //rx full, that's more like it. Copy the frame to RX buffer
        case FLEXCAN_MB_CODE_RX_OVERRUN: //rx overrun. We didn't get there in time and a second frame tried to enter the MB. Whoops... Can probably still grab the frame though.
            // insert into rx queue
            readMB(&frm, i, &can_rxqueue);


            //it seems filtering works by matching against the ID stored in the mailbox
            //so after a frame comes in we've got to refresh the ID field to be the filter ID and not the ID
            //that just came in.
            FLEXCANb_MBn_ID(FLEXCAN0_BASE, i) = FLEXCAN_MB_ID_IDSTD(canFilter);

            break;
        case FLEXCAN_MB_CODE_TX_INACTIVE:
            //TX inactive. Just chillin' waiting for a message to send. Let's see if we've got one.
            if (fifo_pop(&can_txqueue, &frm))
                writeTx(&frm, i);
            break;
        }
    }

    FLEXCANb_IFLAG1(FLEXCAN0_BASE) = status; //writing its value back to itself clears all flags
}

void can0_bus_off_isr(void) {
  print_str("Bus is off");endl();
}

void can0_error_isr(void) {
}

void can0_tx_warn_isr(void) {
}

void can0_rx_warn_isr(void) {
}

void can0_wakeup_isr(void) {
}













//int8_t _canPost(canframe_t *msg)
//{
//  if (fifo_peek(&can_txqueue, msg)) {
//    CAN_message_t frm;
//    frm.id = msg->EID;
//    frm.ext = msg->ext;
//    frm.rtr = msg->rtr;
//    frm.len = msg->DLC;
//    for(uint8_t i = 0; i < msg->DLC; ++i)
//      frm.buf[i] = msg->data8[i];
//
//    if (Can0.write(frm)) {
//      fifo_pop(&can_txqueue, msg);
//      print_str("post:");print_uint(frm.buf[0]);endl();
//      return 1;
//    }
//  }
//  return 0;
//}


//// should be called from teensy_main
//void _canLoop(void)
//{
//  if (!fifo_empty(&can_txqueue)) {
//    canframe_t msg;
//    _canPost(&msg);
//  }
//
//  CAN_message_t frm;
//
//  if (Can0.read(frm)) {
//    // check if we are full
//    if (fifo_spaceleft(&can_rxqueue) < 2)// need 2 as 1 + 1 would reset (start over algorithm assumes empty)
//       return;
//
//    canframe_t msg;
//    msg.DLC = frm.len;
//    msg.EID = frm.id;
//    msg.ext = frm.ext != 0;
//    msg.rtr = frm.rtr != 0;
//    for (uint8_t i = 0; i< msg.DLC; ++i)
//      msg.data8[i] = frm.buf[i];
//
//    fifo_push(&can_rxqueue, &msg);
//  }
//
//
//  digitalWrite(6, !digitalRead(6));
//  //delay(20);
//}


