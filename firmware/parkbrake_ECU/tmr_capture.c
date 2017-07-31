/*
 * tmr_compare.c
 *
 *  Created on: 29 jul 2017
 *      Author: fredrikjohansson
 */



#include "tmr_capture.h"
#include "hal.h"
#include "stm32_tim.h"
#include "stm32_dma.h"
#include "control.h"

// how many samples in each DMA loop
#define NR_SAMPLES 6
#define DMA_PRIORITY 2
#define DMA_IRQ_PRIORITY 1

// in the first version of pcb, the rear wheels weren't connected to a timer which have DMA
// so we just use front wheels there
//#define REARWHEELS_ACTIVE


// ---------------------------------------------------------------------------------
// begin function prototypes
static void dmaCallback(void *arg, uint32_t flags);

// ----------------------------------------------------------------------------------
// begin private functions and variables to this module


#if !defined(REARWHEELS_ACTIVE)
#define NR_DMA_STREAMS 2
#else
#define NR_DMA_STREAMS 4
#endif

static volatile uint16_t samplesBuf[NR_DMA_STREAMS][NR_SAMPLES];

uint32_t *tim3CcrForWheel(ctrl_wheels wheel)
{
    switch (wheel) {
    case LeftFront:  return &STM32_TIM3->CCR[0];
    case RightFront: return &STM32_TIM3->CCR[1];
    case LeftRear:   return &STM32_TIM3->CCR[2];
    case RightRear:  return &STM32_TIM3->CCR[3];
    }
    return NULL;
}
/*
const stm32_dmastream_t *dmaStreamForWheel(ctrl_wheels wheel)
{
    switch (wheel) {
    case LeftFront: return STM32_DMA1_STREAM6; // TIM3 ch1
    case RightFront: return ; //
    case LeftRear: return ;
    case RightRear: return;
    }
    return NULL;
}
*/

void init_TIM3(void)
{
    /*
     * Due to harware faults on rev 0 of the pcb we can't measure rear speed sensors
     * with a timer that has DMA...
     * Currently we just measure front sensors.
     * */


    /*
     * Select input prescaler
     * some info:
     * we must be able to measure down to 40Hz and up to 2000Hz
     * that equates to about 3,6km/h ->  263km/h assuming 60 pulses per
     * wheel revolution and 2.2m tire thread circumference
     *
     * A 16bit timer can go from 0->65535 and we should track 40Hz.
     * We must have less than 25% error margin at 7km/h (80Hz) so in reality we should
     * count at least 4 pulses within our timer overflow window. 1/80Hz = 0.0125s.
     * 12.5ms *4pulses gives 50ms so timer should overflow each 50ms.
     *
     * We then count the number of pulses each channel has on those 50ms to get the frequency
     * We copy registers on each timer overflow update event, via interrupt (not DMA as that
     * clashes with SPI TX).
     *
     * before timer overflows (laterCapture - previousCapture = time it took for 1 pulse)
     * so overflow must be slower than 50ms (20Hz) -> 65535 /  0,05 = 1310719Hz
     * 1.310719Mhz. We drive timer counter with internal clock (HSI) 8Mhz so we divide by 8
     * to get 1MHz clock increments. Is also a nice round number to calculate against
     */
    // turn on TIM3
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // disable to begin with, filter prescaler = 4
    STM32_TIM3->CR1 = STM32_TIM_CR1_CKD(2U);

    // input capture on both channels, input filter at least 8 counter cycles
    STM32_TIM3->CCMR1 = STM32_TIM_CCMR1_CC1S(1U) | STM32_TIM_CCMR1_IC1F(3U) |
                        STM32_TIM_CCMR1_CC2S(1U) | STM32_TIM_CCMR1_IC2F(3U);
    STM32_TIM3->CCMR2 = STM32_TIM_CCMR2_CC3S(1U) | STM32_TIM_CCMR2_IC3F(3U) |
                        STM32_TIM_CCMR2_CC4S(1U) | STM32_TIM_CCMR2_IC4F(3U);

    // rising edge, output compare
    STM32_TIM3->CCER = STM32_TIM_CCER_CC1E | STM32_TIM_CCER_CC2E;

    /*formula from reference manual:
     * The counter clock frequency CK_CNT is equal to fCK_PSC / (PSC[15:0] + 1).*/
    // we should get 1Mhz clock here
    STM32_TIM3->PSC = (SystemCoreClock / 1000000) -1; // 8 - 1; // might correct?

    // No slave mode
    STM32_TIM3->SMCR = RESET;

    // overflow at
    STM32_TIM3->ARR = 50000; // roll over after 50ms



    // DMA interrupt on capture
    //STM32_TIM3->DIER = STM32_TIM_DIER_CC1E | STM32_TIM_DIER_CC2DE;
    //STM32_TIM3->DIER = STM32_TIM_DIER_CC3DE | STM32_TIM_DIER_CC4DE;

/*
    // setup DMA
    int i;
    for (int i = LeftFront; i <= RightRear; ++i){
        if (!dmaStreamAllocate(&dmaStreams[i], DMA_PRIORITY, dmaCallback, (void*)i))
            chDbgAssert(false, "DMA stream used");

        dmaStreamSetPeripheral(&dmaStreams[i], tim3CcrForWheel(i));
        dmaStreamSetMemory0(&dmaStreams[i], samplesBuf[i]);
        dmaStreamSetTransactionSize(&dmaStreams[i], NR_SAMPLES);
        dmaStreamSetMode(&dmaStreams[i], STM32_DMA_CR_DIR_P2M | STM32_DMA_CR_TCIE
                                         STM32_DMA_CR_MSIZE_HWORD | STM32_DMA_CR_PSIZE_HWORD | );

        // TODO circular dma
        //      figure out how to avoid channel clashes with SPI(eeprom) and other peripherals
    }
*/

    // Start Timer
    STM32_TIM3->CR1 |= STM32_TIM_CR1_CEN;

}


// -----------------------------------------------------------------------------------
// begin callbacks


static void dmaCallback(void *arg, uint32_t flags)
{
    (void)flags;
    ctrl_wheels wheel = (ctrl_wheels)arg;
    uint16_t **buf;
    switch (wheel) {
    case LeftFront:
        break;
    case RightFront:
        break;
    case RightRear:
        break;
    case LeftRear:
        break;
    default:
        return;
    }

    int i;
    for (i = 0; i < NR_SAMPLES; ++i){

    }
}

// ------------------------------------------------------------------------------------
// begin public API

void tmr_init(void)
{
    ;
}
