/*
 * system.c
 *
 *  Created on: 27 jul 2018
 *      Author: jof
 */


#include "system.h"
#include <stddef.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/vector.h>
#include <libopencm3/stm32/common/rcc_common_all.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/cm3/nvic.h>

const struct rcc_clock_scale clock_scale  = {
    .pllsrc = RCC_CFGR_PLLSRC_HSE_PREDIV,
    .pllmul = RCC_CFGR_PLLMUL_MUL9,
    .plldiv = RCC_CFGR2_PREDIV_DIV3, // a 24MHz chrystal
    .usbdiv1 = false,
    .flash_waitstates = 2,
    .hpre = RCC_CFGR_HPRE_DIV_NONE,
    .ppre1 = RCC_CFGR_PPRE1_DIV_2,
    .ppre2 = RCC_CFGR_PPRE2_DIV_NONE,
    .ahb_frequency = 72e6,
    .apb1_frequency = 32e6,
    .apb2_frequency = 72e6,
};

#if NODE_ID==CAN_PARKBRAKE_NODE
const uint16_t canId = CAN_MSG_TYPE_COMMAND | C_parkbrakeCmdReboot | C_parkbrakeNode;
#elif NODE_ID==C_suspensionNode
const uint16_t canId = CAN_MSG_TYPE_COMMAND | C_suspensionCmdReboot | C_suspensionNode;
#endif

const uint16_t pageSize = 2048; // 2kb


volatile uint32_t globalMsSinceStartup = 0;

void systemInit(void)
{
  rcc_clock_setup_pll(&clock_scale);
}

void systemDeinit(void)
{
  // Can't figure out this one , continue on ext oscillator and hope for the best..
  // rcc_clock_setup_pll(&rcc_configs[RCC_CLOCK_HSI_64MHZ]);
}

void systickInit(void)
{
    /* 72MHz / 8 => 9000000 counts per second */
    //systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);

    /* 9000000/9000 = 1000 overflows per second - every 1ms one interrupt */
    /* SysTick interrupt every N clock pulses: set reload to N-1 */
    //systick_set_reload(8999);

    systick_set_frequency(1000, clock_scale.ahb_frequency);

    systick_interrupt_enable();

    /* Start counting. */
    systick_clear();
    systick_counter_enable();
}

void systickShutdown(void)
{
  systick_interrupt_disable();
  systick_counter_disable();
  systick_clear();
  systick_set_clocksource(0);
  systick_set_reload(0);
}



void sys_tick_handler(void)
{
  ++globalMsSinceStartup;
}


void systemToApplication(void)
{
  extern const size_t _appRomStart;
  typedef void (*funcptr_t)(void);

  /* variable that will be loaded with the start address of the application */
  size_t  *jumpAddress;
  const size_t* applicationAddress = &_appRomStart;

  /* get jump address from application vector table */
  jumpAddress = (size_t*) applicationAddress[1];

  /* load this address into function pointer */
  funcptr_t start_func = (funcptr_t)jumpAddress;

  /* Disable all interrupts */
  for(int i = 0; i < NVIC_IRQ_COUNT; ++i){
    nvic_clear_pending_irq(i);
    nvic_disable_irq(i);
  }

  /* set stack pointer as in application's vector table */
  // change both Mainstackpointer and PriorityStackpointer
  asm("msr msp, %[address]" : : [address] "r" (applicationAddress));
  asm("msr psp, %[address]" : : [address] "r" (applicationAddress));
  //register size_t sp asm ("msp");
//  register uint32_t __sp     __asm("msp");
//  __sp = (applicationAddress[0]);

  /* call our application startup */
  start_func();
}

void systemReset(void)
{
  scb_reset_system();
}

can_bootloaderErrs_e systemFlashErase(uint8_t *startAddr, uint8_t *endAddr)
{
  can_bootloaderErrs_e err = C_bootloaderErrOK;
  flash_unlock();
  flash_wait_for_last_operation();

  do {
    // Start delete page.
    FLASH_CR |= FLASH_CR_PER;
    FLASH_AR = (uint32_t)startAddr;
    FLASH_CR |= FLASH_CR_STRT;

    // Wait until it's finished.
    flash_wait_for_last_operation();

    if (!(FLASH_SR & FLASH_SR_EOP)) {
      err = C_bootloaderErrPageEraseFailed;
      break; // write failed
    }

    // Page erase flag does not clear automatically.
    FLASH_CR &= !FLASH_CR_PER;

    startAddr += pageSize; // set next page
  } while (startAddr < endAddr);

  flash_lock();
  flash_wait_for_last_operation();

  return err;

}

can_bootloaderErrs_e systemFlashWritePage(uint16_t *memPageBuf,
                                          volatile uint16_t *addr)
{
  can_bootloaderErrs_e err = C_bootloaderErrOK;
  flash_unlock();
  flash_wait_for_last_operation();

  // must only write 2 bytes at a time, as per programming manual
  // else bus error interrupt occurs
  uint16_t *end = memPageBuf + (pageSize / 2); // pagesize is uint8 not 16
  do {
    FLASH_CR |= FLASH_CR_PG;
    *addr = *memPageBuf; // write to rom
    flash_wait_for_last_operation();

    // checks for write errors (addr is volatile)
    if (*addr != *memPageBuf) {
      err = C_bootloaderErrPageWriteFailed;
      // quit writing sequence and lock flash
      break;
    }

    ++addr;
  } while (memPageBuf++ < end);

  flash_lock();
  flash_wait_for_last_operation();

  return err;
}