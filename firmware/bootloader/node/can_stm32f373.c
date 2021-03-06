/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2012 Piotr Esden-Tempski <piotr@esden.net>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "system.h"
#include "can.h"
#include "fifo.h"
#include <can_protocol.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/can.h>

extern fifo_t can_rxqueue, can_txqueue; // is visible to global from can.c


// prototypes
int8_t _canPost(can_frame_t *msg);
void _canInit(void);
bool _canGet(void);

// --------------------------------------------------------------
// private to this file

static void init_filters(uint32_t id, uint32_t mask, bool enable)
{
  for (uint16_t fifo=0, filterId = 0; fifo < 2; fifo++) {
    // filter out all but 11bit ids and our specific ID
    can_filter_id_mask_16bit_init(
                filterId++,     /* Filter ID */
                (uint16_t)id,   /* CAN ID */
                (uint16_t)mask,  /* CAN ID mask */
                (uint16_t)id,
                (uint16_t)mask,
                fifo,     /* FIFO assignment (here: FIFO0) */
                enable); /* Enable the filter. */
    // squeslh all 29bit IDs
    //can_filter_id_mask_32bit_init(filterId++, 0, 0x1FFFFFFF, fifo, enable);
  }
}

// called by receive interrupts
static void receive(uint8_t fifo)
{
    // check if we are full
    if (fifo_spaceleft(&can_rxqueue) < 2)// need 2 as 1 + 1 would reset (start over algorithm assumes empty)
      return;

    uint8_t fmi;
    uint16_t timestamp;
    can_frame_t msg;
    can_receive(CAN1, fifo, true, &msg.EID, &msg.ext,
                &msg.rtr, &fmi, &msg.DLC, &msg.data8[0], &timestamp);
    fifo_push(&can_rxqueue, &msg);
}


// ------------------------------------------------------------------------
// interupts

// for fifo0
void usb_lp_can1_rx0_isr(void)
{
  receive(0);
}

// for fifo 1
void can1_rx1_isr(void)
{
  receive(1);
}

// tx box avalable
void usb_hp_can1_tx_isr(void)
{
  // we clear this irq by writing a 1 to each transmit buffers RQCP
  if (CAN_TSR(CAN1) & CAN_TSR_RQCP0)
    CAN_TSR(CAN1) |= CAN_TSR_RQCP0;
  if (CAN_TSR(CAN1) & CAN_TSR_RQCP1)
    CAN_TSR(CAN1) |= CAN_TSR_RQCP1;
  if (CAN_TSR(CAN1) & CAN_TSR_RQCP2)
    CAN_TSR(CAN1) |= CAN_TSR_RQCP2;

  if (can_available_mailbox(CAN1)) {
    can_frame_t msg;
    if (!fifo_pop(&can_txqueue, &msg))
      return; // nothing to send

    can_transmit(CAN1, msg.EID, msg.ext, msg.rtr, msg.DLC, msg.data8);
  }
}


// ------------------------------------------------------------------------
// public functions and variables



void canShutdown(void)
{
  // clear filters
  init_filters(0, 0, false);
  can_disable_irq(CAN1, CAN_IER_FMPIE0 | CAN_IER_FFIE0 |
                        CAN_IER_FMPIE1 | CAN_IER_FFIE1 |
                        CAN_IER_TMEIE);
  can_reset(CAN1);

  rcc_periph_clock_disable(RCC_CAN1);
  rcc_periph_clock_disable(RCC_GPIOA);
}

void canDisableIRQ(void)
{
  nvic_disable_irq(NVIC_USB_HP_CAN1_TX_IRQ);
  nvic_disable_irq(NVIC_USB_LP_CAN1_RX0_IRQ);
  nvic_disable_irq(NVIC_CAN1_RX1_IRQ);
}
void canEnableIRQ(void)
{
  nvic_enable_irq(NVIC_USB_HP_CAN1_TX_IRQ);
  nvic_enable_irq(NVIC_USB_LP_CAN1_RX0_IRQ);
  nvic_enable_irq(NVIC_CAN1_RX1_IRQ);
}

// --------------------------------------------------------------------
// implementation specific here on, treat as private

bool _canGet(void)
{
  return false; // does nothing in this architecture
}

int8_t _canPost(can_frame_t *msg)
{
  // should we send immediately?
  if (can_available_mailbox(CAN1)) {
    uint16_t guard = 0;
    while (!fifo_push(&can_txqueue, msg) && ++guard)
      nvic_generate_software_interrupt(NVIC_USB_HP_CAN1_TX_IRQ);

    nvic_generate_software_interrupt(NVIC_USB_HP_CAN1_TX_IRQ);
    return 1;
  }
  return 0;
}

void _canInit(void)
{
    /* Enable peripheral clocks. */
    rcc_periph_clock_enable(RCC_CAN1);
    rcc_periph_clock_enable(RCC_GPIOA);

    // init pins
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO11 | GPIO12);
    gpio_set_af(GPIOA, GPIO_AF9, GPIO11 | GPIO12);
    gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, GPIO12);

    /* Reset CAN. */
    can_reset(CAN1);

    /* CAN cell init.
     * Setting the bitrate to 1MBit. APB1 = 32MHz,
     * prescaler = 2 -> 16MHz time quanta frequency.
     * Bus driven at 36MHz / brp
     * 1tq sync + 9tq bit segment1 (TS1) + 6tq bit segment2 (TS2) =
     * 16time quanto per bit period, therefor 32MHz/8=4MHz -> 4MHz/16tq = 250kHz
     */
    if (can_init(CAN1,
             false,           /* TTCM: Time triggered comm mode? */
             true,            /* ABOM: Automatic bus-off management? */
             false,           /* AWUM: Automatic wakeup mode? */
             false,           /* NART: No automatic retransmission? */
             false,           /* RFLM: Receive FIFO locked mode? */
             false,           /* TXFP: Transmit FIFO priority? */
             CAN_BTR_SJW_1TQ,
             CAN_BTR_TS1_9TQ,
             CAN_BTR_TS2_6TQ,
             8+1,               /* BRP+1: Baud rate prescaler */
             false,
             false))
    {

        /* Die because we failed to initialize. */
        while (1)
            __asm__("nop");
    }


//    for(uint32_t nr = 0; nr < 2; ++nr) {
//      uint16_t id1 = (CAN_MY_ID << 5), msk1 = (0x7FF<<5), id2 = ((CAN_HOST_ID) << 5), msk2 = (0x7FF << 5);
//      uint32_t fifo = 0;
//      uint32_t filter_select_bit = 1 << nr;
//
//      /* Request initialization "enter". */
//      CAN_FMR(CAN1) |= CAN_FMR_FINIT; // enter filter setup mode
//      /* Deactivate the filter. */
//      CAN_FA1R(CAN1) &= ~filter_select_bit; // turn off filter
//      /* Set 16-bit scale for the filter. */
//      CAN_FS1R(CAN1) &= ~filter_select_bit; // 0 = 2*16bits filter, 1= 1*32bit filter
//      /* Set filter mode to id/mask mode. */
//      CAN_FM1R(CAN1) &= ~filter_select_bit; // list = 1/mask = 0 mode
//      /* Set the first filter register. */
//      CAN_FiR1(CAN1, nr) = (msk1 << 16) | (id1);
//      /* Set the second filter register. */
//      CAN_FiR2(CAN1, nr) = (msk2 << 16) | (id2);
//      if (fifo) {
//        CAN_FFA1R(CAN1) |= filter_select_bit;  /* FIFO1 */ // 1=fifo1
//      } else {
//        CAN_FFA1R(CAN1) &= ~filter_select_bit; /* FIFO0 */ // 0=fifo0
//      }
//      CAN_FA1R(CAN1) |= filter_select_bit; /* Activate filter. */
//
//      /* Request initialization "leave". */
//      CAN_FMR(CAN1) &= ~CAN_FMR_FINIT; // exit filter setup mode
//
//      uint32_t regF1 = CAN_FiR1(CAN1, nr);
//      uint32_t regF2 = CAN_FiR2(CAN1, nr);
//    }


    //can_filter_id_mask_16bit_init(0, id1, msk1,id2, msk2, 0, true);
    /* CAN filter init.on all fifos we have 14 filter banks */
    //                       filternr  id1              mask1       id2               mask2   fifo  enabled
    //can_filter_id_mask_16bit_init(0,   (CAN_MY_ID<<5), (0x7FF<<5), (CAN_HOST_ID<<5), (0x7FF<<5), 0, true);
    //can_filter_id_mask_16bit_init(1,   (CAN_MY_ID<<5), (0x7FF<<5), (CAN_HOST_ID<<5), (0x7FF<<5), 1, true);

    // these are right aligned, took me long time to figure out...
    //           filtenr 32bit?  list,no msk    id1          id2        fifo  enable
    can_filter_init(0,   false,  true, (((0x7FF<<(5+16))| (CAN_MY_ID<<5))), ((0x7FF<<(5+16))|(CAN_HOST_ID<<5)), 0, true);
    can_filter_init(1,   false,  true, (((0x7FF<<(5+16))| (CAN_MY_ID<<5))), ((0x7FF<<(5+16))|(CAN_HOST_ID<<5)), 1, true);

    //init_filters(canId, 0x07F8, true);
    //init_filters(0, 0, true); // for debug, all filters open

    /* NVIC setup. for both rx FIFOs  */
    nvic_enable_irq(NVIC_USB_LP_CAN1_RX0_IRQ); // fifo 0
 //   nvic_set_priority(NVIC_USB_LP_CAN1_RX0_IRQ, 1);
    nvic_enable_irq(NVIC_CAN1_RX1_IRQ);        // fifo 1
 //   nvic_set_priority(NVIC_CAN1_RX1_IRQ, 2);
    nvic_enable_irq(NVIC_USB_HP_CAN1_TX_IRQ);
 //   nvic_set_priority(NVIC_USB_HP_CAN1_TX_IRQ, 3);

    /* Enable CAN RX interrupt. */
    can_enable_irq(CAN1, CAN_IER_FMPIE0 | CAN_IER_FFIE0 |
                         CAN_IER_FMPIE1 | CAN_IER_FFIE1 |
                         CAN_IER_TMEIE);

}
