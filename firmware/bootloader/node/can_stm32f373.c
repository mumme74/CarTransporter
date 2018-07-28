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
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

canframe_t rxbuf[BUFFER_SIZE];
canframe_t txbuf[BUFFER_SIZE];
volatile int rxpos, txpos;

static void init_filters(uint32_t id, uint32_t mask, bool enable)
{
  for (uint16_t fifo=0, filterId = 0; fifo < 2; fifo++) {
    // filter out all but 11bit ids and our specific ID
    can_filter_id_mask_16bit_init(
                filterId++,     /* Filter ID */
                (uint16_t)(id & 0x7FF), /* CAN ID */
                (uint16_t)(mask & 0x7FF),  /* CAN ID mask */
                0,
                0x7FF,
                fifo,     /* FIFO assignment (here: FIFO0) */
                enable); /* Enable the filter. */
    // squeslh all 29bit IDs
    can_filter_id_mask_32bit_init(filterId++, 0, mask, fifo, enable);
  }
}


static void receive(uint8_t fifo)
{
    // check if we are full
    if (rxpos >= BUFFER_SIZE)
      return;

    uint8_t fmi;
    uint16_t timestamp;
    canframe_t *msg = &rxbuf[++rxpos];
    can_receive(CAN1, fifo, true, &msg->EID, &msg->ext,
                &msg->rtr, &fmi, &msg->DLC, &msg->data8[0], &timestamp);

    can_fifo_release(CAN1, fifo);
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
  if (txpos < 0)
    return; // nothing to send
  if (!can_available_mailbox(CAN1))
    return; // output registers full

  canframe_t *msg = &txbuf[--txpos];
  can_transmit(CAN1, msg->EID, msg->ext, msg->rtr, msg->DLC, msg->data8);
}


// ------------------------------------------------------------------------
// public functions and variables

void canInit(uint32_t filterId)
{
    // init store variables
    rxpos = txpos = -1;

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
             8-1,               /* BRP+1: Baud rate prescaler */
             false,
             false))
    {

        /* Die because we failed to initialize. */
        while (1)
            __asm__("nop");
    }


    /* CAN filter init.on all fifos */
    init_filters(filterId, 0x7FF, true);

    /* NVIC setup. for both rx FIFOs  */
    nvic_enable_irq(NVIC_USB_LP_CAN1_RX0_IRQ); // fifo 0
    nvic_set_priority(NVIC_USB_LP_CAN1_RX0_IRQ, 1);
    nvic_enable_irq(NVIC_CAN1_RX1_IRQ);        // fifo 1
    nvic_set_priority(NVIC_CAN1_RX1_IRQ, 1);
    nvic_enable_irq(NVIC_USB_HP_CAN1_TX_IRQ);
    nvic_set_priority(NVIC_USB_HP_CAN1_TX_IRQ, 2);

    /* Enable CAN RX interrupt. */
    can_enable_irq(CAN1, CAN_IER_FMPIE0 | CAN_IER_FMPIE1 | CAN_IER_TMEIE);

}

void canShutdown(void)
{
  // clear filters
  init_filters(0, 0, false);
  can_disable_irq(CAN1, CAN_IER_FMPIE0 | CAN_IER_FMPIE1 | CAN_IER_TMEIE);
  can_reset(CAN1);
}


/**
 * @brief get the last recived msg form LIFO register
 * returns: null if empty
 *          last recieved frame
 */
canframe_t *canGet(void)
{
    if (rxpos < 0)
        return NULL;

    return &rxbuf[--rxpos];
}

/**
 * @brief post a frame to can
 * returns: -1 if buffer full
 *          0 if queued
 *          1 if send immediatly
 */
int8_t canPost(canframe_t *msg)
{
  // buffer full?
  if (txpos >= BUFFER_SIZE)
    return -1;

  canframe_t *bmsg = &txbuf[++txpos];
  memcpy(bmsg, msg, sizeof(canframe_t));

  if (can_available_mailbox(CAN1)) {
    nvic_generate_software_interrupt(CAN_IER_TMEIE);
    return 1;
  }
  return 0;
}

void canInitFrame(canframe_t *msg, uint32_t id)
{
  msg->EID = id;
  msg->data64 = 0;
  msg->DLC = 0;
  msg->ext = id > 0x7FF ? true : false;
  msg->rtr = false;
}
