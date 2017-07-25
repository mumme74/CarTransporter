/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/*
 * This file has been automatically generated using ChibiStudio board
 * generator plugin. Do not edit manually.
 */

#ifndef BOARD_H
#define BOARD_H

/*
 * Setup for ParkbrakeECU board.
 */

/*
 * Board identifier.
 */
#define BOARD_
#define BOARD_NAME                  "ParkbrakeECU"

/*
 * Board oscillators-related settings.
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                32768U
#endif

#define STM32_LSEDRV                (3U << 3U)

#if !defined(STM32_HSECLK)
#define STM32_HSECLK                24000000U
#endif

/*
 * MCU type as defined in the ST header.
 */
#define STM32F373xC

/*
 * IO pins assignments.
 */
#define GPIOA_LeftRear_CS           0U
#define GPIOA_RightRear_CS          1U
#define GPIOA_LeftFront_CS          2U
#define GPIOA_RightFront_CS         3U
#define GPIOA_LR_speed_in           4U
#define GPIOA_RR_speed_in           5U
#define GPIOA_LF_speed_in           6U
#define GPIOA_RF_speed_in           7U
#define GPIOA_LeftRear_DIAG         8U
#define GPIOA_RightRear_DIAG        9U
#define GPIOA_PWR_ENABLED_SIG       10U
#define GPIOA_CAN_RX                11U
#define GPIOA_CAN_TX                12U
#define GPIOA_JTMS                  13U
#define GPIOA_JTCLK                 14U
#define GPIOA_JTDI                  15U

#define GPIOB_PWR_voltsense         0U
#define GPIOB_LeftFront_DIAG        1U
#define GPIOB_RightFront_DIAG       2U
#define GPIOB_JTDO                  3U
#define GPIOB_JTNRST                4U
#define GPIOB_Bridge_Disable        5U
#define GPIOB_USART_TX              6U
#define GPIOB_USART_RX              7U
#define GPIOB_L_speed_out           8U
#define GPIOB_L_speed_out_inv       9U
#define GPIOB_PIN10                 10U
#define GPIOB_PIN11                 11U
#define GPIOB_PIN12                 12U
#define GPIOB_PIN13                 13U
#define GPIOB_R_speed_out           14U
#define GPIOB_R_speed_out_inv       15U

#define GPIOC_Feed_voltsense        0U
#define GPIOC_INDICATOR_ON          1U
#define GPIOC_LeftFront_Tighten     2U
#define GPIOC_LeftFront_Loosen      3U
#define GPIOC_RightFront_Tighten    4U
#define GPIOC_RightFront_Loosen     5U
#define GPIOC_SPI_CS                6U
#define GPIOC_SPI_SCK               7U
#define GPIOC_SPI_MISO              8U
#define GPIOC_SPI_MOSI              9U
#define GPIOC_LeftRear_Tighten      10U
#define GPIOC_LeftRear_Loosen       11U
#define GPIOC_RightRear_Tighten     12U
#define GPIOC_RightRear_Loosen      13U
#define GPIOC_uC_SET_POWER          14U
#define GPIOC_VR_diag_test          15U

#define GPIOD_PIN0                  0U
#define GPIOD_PIN1                  1U
#define GPIOD_IGN_ON_SIG            2U
#define GPIOD_PIN3                  3U
#define GPIOD_PIN4                  4U
#define GPIOD_PIN5                  5U
#define GPIOD_PIN6                  6U
#define GPIOD_PIN7                  7U
#define GPIOD_LIGHTS_ON_SIG         8U
#define GPIOD_PIN9                  9U
#define GPIOD_PIN10                 10U
#define GPIOD_PIN11                 11U
#define GPIOD_PIN12                 12U
#define GPIOD_PIN13                 13U
#define GPIOD_PIN14                 14U
#define GPIOD_PIN15                 15U

#define GPIOE_PIN0                  0U
#define GPIOE_PIN1                  1U
#define GPIOE_PIN2                  2U
#define GPIOE_PIN3                  3U
#define GPIOE_PIN4                  4U
#define GPIOE_PIN5                  5U
#define GPIOE_PIN6                  6U
#define GPIOE_PIN7                  7U
#define GPIOE_BUTTON_SIG            8U
#define GPIOE_BUTTON_INV_SIG        9U
#define GPIOE_PIN10                 10U
#define GPIOE_PIN11                 11U
#define GPIOE_PIN12                 12U
#define GPIOE_PIN13                 13U
#define GPIOE_PIN14                 14U
#define GPIOE_PIN15                 15U

#define GPIOF_OSC_IN                0U
#define GPIOF_OSC_OUT               1U
#define GPIOF_PIN2                  2U
#define GPIOF_PIN3                  3U
#define GPIOF_PIN4                  4U
#define GPIOF_PIN5                  5U
#define GPIOF_TIGHTEN_CMD_SIG       6U
#define GPIOF_RELEASE_CMD_SIG       7U
#define GPIOF_PIN8                  8U
#define GPIOF_PIN9                  9U
#define GPIOF_PIN10                 10U
#define GPIOF_PIN11                 11U
#define GPIOF_PIN12                 12U
#define GPIOF_PIN13                 13U
#define GPIOF_PIN14                 14U
#define GPIOF_PIN15                 15U

#define GPIOG_PIN0                  0U
#define GPIOG_PIN1                  1U
#define GPIOG_PIN2                  2U
#define GPIOG_PIN3                  3U
#define GPIOG_PIN4                  4U
#define GPIOG_PIN5                  5U
#define GPIOG_PIN6                  6U
#define GPIOG_PIN7                  7U
#define GPIOG_PIN8                  8U
#define GPIOG_PIN9                  9U
#define GPIOG_PIN10                 10U
#define GPIOG_PIN11                 11U
#define GPIOG_PIN12                 12U
#define GPIOG_PIN13                 13U
#define GPIOG_PIN14                 14U
#define GPIOG_PIN15                 15U

#define GPIOH_PIN0                  0U
#define GPIOH_PIN1                  1U
#define GPIOH_PIN2                  2U
#define GPIOH_PIN3                  3U
#define GPIOH_PIN4                  4U
#define GPIOH_PIN5                  5U
#define GPIOH_PIN6                  6U
#define GPIOH_PIN7                  7U
#define GPIOH_PIN8                  8U
#define GPIOH_PIN9                  9U
#define GPIOH_PIN10                 10U
#define GPIOH_PIN11                 11U
#define GPIOH_PIN12                 12U
#define GPIOH_PIN13                 13U
#define GPIOH_PIN14                 14U
#define GPIOH_PIN15                 15U

/*
 * IO lines assignments.
 */
#define LINE_LeftRear_CS            PAL_LINE(GPIOA, 0U)
#define LINE_RightRear_CS           PAL_LINE(GPIOA, 1U)
#define LINE_LeftFront_CS           PAL_LINE(GPIOA, 2U)
#define LINE_RightFront_CS          PAL_LINE(GPIOA, 3U)
#define LINE_LR_speed_in            PAL_LINE(GPIOA, 4U)
#define LINE_RR_speed_in            PAL_LINE(GPIOA, 5U)
#define LINE_LF_speed_in            PAL_LINE(GPIOA, 6U)
#define LINE_RF_speed_in            PAL_LINE(GPIOA, 7U)
#define LINE_LeftRear_DIAG          PAL_LINE(GPIOA, 8U)
#define LINE_RightRear_DIAG         PAL_LINE(GPIOA, 9U)
#define LINE_PWR_ENABLED_SIG        PAL_LINE(GPIOA, 10U)
#define LINE_CAN_RX                 PAL_LINE(GPIOA, 11U)
#define LINE_CAN_TX                 PAL_LINE(GPIOA, 12U)
#define LINE_JTMS                   PAL_LINE(GPIOA, 13U)
#define LINE_JTCLK                  PAL_LINE(GPIOA, 14U)
#define LINE_JTDI                   PAL_LINE(GPIOA, 15U)

#define LINE_PWR_voltsense          PAL_LINE(GPIOB, 0U)
#define LINE_LeftFront_DIAG         PAL_LINE(GPIOB, 1U)
#define LINE_RightFront_DIAG        PAL_LINE(GPIOB, 2U)
#define LINE_JTDO                   PAL_LINE(GPIOB, 3U)
#define LINE_JTNRST                 PAL_LINE(GPIOB, 4U)
#define LINE_Bridge_Disable         PAL_LINE(GPIOB, 5U)
#define LINE_USART_TX               PAL_LINE(GPIOB, 6U)
#define LINE_USART_RX               PAL_LINE(GPIOB, 7U)
#define LINE_L_speed_out            PAL_LINE(GPIOB, 8U)
#define LINE_L_speed_out_inv        PAL_LINE(GPIOB, 9U)
#define LINE_R_speed_out            PAL_LINE(GPIOB, 14U)
#define LINE_R_speed_out_inv        PAL_LINE(GPIOB, 15U)

#define LINE_Feed_voltsense         PAL_LINE(GPIOC, 0U)
#define LINE_INDICATOR_ON           PAL_LINE(GPIOC, 1U)
#define LINE_LeftFront_Tighten      PAL_LINE(GPIOC, 2U)
#define LINE_LeftFront_Loosen       PAL_LINE(GPIOC, 3U)
#define LINE_RightFront_Tighten     PAL_LINE(GPIOC, 4U)
#define LINE_RightFront_Loosen      PAL_LINE(GPIOC, 5U)
#define LINE_SPI_CS                 PAL_LINE(GPIOC, 6U)
#define LINE_SPI_SCK                PAL_LINE(GPIOC, 7U)
#define LINE_SPI_MISO               PAL_LINE(GPIOC, 8U)
#define LINE_SPI_MOSI               PAL_LINE(GPIOC, 9U)
#define LINE_LeftRear_Tighten       PAL_LINE(GPIOC, 10U)
#define LINE_LeftRear_Loosen        PAL_LINE(GPIOC, 11U)
#define LINE_RightRear_Tighten      PAL_LINE(GPIOC, 12U)
#define LINE_RightRear_Loosen       PAL_LINE(GPIOC, 13U)
#define LINE_uC_SET_POWER           PAL_LINE(GPIOC, 14U)
#define LINE_VR_diag_test           PAL_LINE(GPIOC, 15U)

#define LINE_IGN_ON_SIG             PAL_LINE(GPIOD, 2U)
#define LINE_LIGHTS_ON_SIG          PAL_LINE(GPIOD, 8U)

#define LINE_BUTTON_SIG             PAL_LINE(GPIOE, 8U)
#define LINE_BUTTON_INV_SIG         PAL_LINE(GPIOE, 9U)

#define LINE_OSC_IN                 PAL_LINE(GPIOF, 0U)
#define LINE_OSC_OUT                PAL_LINE(GPIOF, 1U)
#define LINE_TIGHTEN_CMD_SIG        PAL_LINE(GPIOF, 6U)
#define LINE_RELEASE_CMD_SIG        PAL_LINE(GPIOF, 7U)



/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2U))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2U))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2U))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2U))
#define PIN_ODR_LOW(n)              (0U << (n))
#define PIN_ODR_HIGH(n)             (1U << (n))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_VERYLOW(n)       (0U << ((n) * 2U))
#define PIN_OSPEED_LOW(n)           (1U << ((n) * 2U))
#define PIN_OSPEED_MEDIUM(n)        (2U << ((n) * 2U))
#define PIN_OSPEED_HIGH(n)          (3U << ((n) * 2U))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2U))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2U))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2U))
#define PIN_AFIO_AF(n, v)           ((v) << (((n) % 8U) * 4U))

/*
 * GPIOA setup:
 *
 * PA0  - LeftRear_CS               (analog).
 * PA1  - RightRear_CS              (analog).
 * PA2  - LeftFront_CS              (analog).
 * PA3  - RightFront_CS             (analog).
 * PA4  - LR_speed_in               (input pullup).
 * PA5  - RR_speed_in               (input floating).
 * PA6  - LF_speed_in               (input pullup).
 * PA7  - RF_speed_in               (input pullup).
 * PA8  - LeftRear_DIAG             (input pullup).
 * PA9  - RightRear_DIAG            (input floating).
 * PA10 - PWR_ENABLED_SIG           (input floating).
 * PA11 - CAN_RX                    (alternate 9).
 * PA12 - CAN_TX                    (alternate 9).
 * PA13 - JTMS                      (alternate 0).
 * PA14 - JTCLK                     (alternate 0).
 * PA15 - JTDI                      (alternate 0).
 */
#define VAL_GPIOA_MODER             (PIN_MODE_ANALOG(GPIOA_LeftRear_CS) |   \
                                     PIN_MODE_ANALOG(GPIOA_RightRear_CS) |  \
                                     PIN_MODE_ANALOG(GPIOA_LeftFront_CS) |  \
                                     PIN_MODE_ANALOG(GPIOA_RightFront_CS) | \
                                     PIN_MODE_INPUT(GPIOA_LR_speed_in) |    \
                                     PIN_MODE_INPUT(GPIOA_RR_speed_in) |    \
                                     PIN_MODE_INPUT(GPIOA_LF_speed_in) |    \
                                     PIN_MODE_INPUT(GPIOA_RF_speed_in) |    \
                                     PIN_MODE_INPUT(GPIOA_LeftRear_DIAG) |  \
                                     PIN_MODE_INPUT(GPIOA_RightRear_DIAG) | \
                                     PIN_MODE_INPUT(GPIOA_PWR_ENABLED_SIG) |\
                                     PIN_MODE_ALTERNATE(GPIOA_CAN_RX) |     \
                                     PIN_MODE_ALTERNATE(GPIOA_CAN_TX) |     \
                                     PIN_MODE_ALTERNATE(GPIOA_JTMS) |       \
                                     PIN_MODE_ALTERNATE(GPIOA_JTCLK) |      \
                                     PIN_MODE_ALTERNATE(GPIOA_JTDI))
#define VAL_GPIOA_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOA_LeftRear_CS) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_RightRear_CS) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_LeftFront_CS) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_RightFront_CS) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_LR_speed_in) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_RR_speed_in) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_LF_speed_in) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_RF_speed_in) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_LeftRear_DIAG) |\
                                     PIN_OTYPE_OPENDRAIN(GPIOA_RightRear_DIAG) |\
                                     PIN_OTYPE_OPENDRAIN(GPIOA_PWR_ENABLED_SIG) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_CAN_RX) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOA_CAN_TX) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOA_JTMS) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_JTCLK) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOA_JTDI))
#define VAL_GPIOA_OSPEEDR           (PIN_OSPEED_VERYLOW(GPIOA_LeftRear_CS) |\
                                     PIN_OSPEED_VERYLOW(GPIOA_RightRear_CS) |\
                                     PIN_OSPEED_VERYLOW(GPIOA_LeftFront_CS) |\
                                     PIN_OSPEED_VERYLOW(GPIOA_RightFront_CS) |\
                                     PIN_OSPEED_HIGH(GPIOA_LR_speed_in) |   \
                                     PIN_OSPEED_HIGH(GPIOA_RR_speed_in) |   \
                                     PIN_OSPEED_HIGH(GPIOA_LF_speed_in) |   \
                                     PIN_OSPEED_HIGH(GPIOA_RF_speed_in) |   \
                                     PIN_OSPEED_HIGH(GPIOA_LeftRear_DIAG) | \
                                     PIN_OSPEED_HIGH(GPIOA_RightRear_DIAG) |\
                                     PIN_OSPEED_HIGH(GPIOA_PWR_ENABLED_SIG) |\
                                     PIN_OSPEED_HIGH(GPIOA_CAN_RX) |        \
                                     PIN_OSPEED_HIGH(GPIOA_CAN_TX) |        \
                                     PIN_OSPEED_HIGH(GPIOA_JTMS) |          \
                                     PIN_OSPEED_HIGH(GPIOA_JTCLK) |         \
                                     PIN_OSPEED_HIGH(GPIOA_JTDI))
#define VAL_GPIOA_PUPDR             (PIN_PUPDR_FLOATING(GPIOA_LeftRear_CS) |\
                                     PIN_PUPDR_FLOATING(GPIOA_RightRear_CS) |\
                                     PIN_PUPDR_FLOATING(GPIOA_LeftFront_CS) |\
                                     PIN_PUPDR_FLOATING(GPIOA_RightFront_CS) |\
                                     PIN_PUPDR_PULLUP(GPIOA_LR_speed_in) |  \
                                     PIN_PUPDR_FLOATING(GPIOA_RR_speed_in) |\
                                     PIN_PUPDR_PULLUP(GPIOA_LF_speed_in) |  \
                                     PIN_PUPDR_PULLUP(GPIOA_RF_speed_in) |  \
                                     PIN_PUPDR_PULLUP(GPIOA_LeftRear_DIAG) |\
                                     PIN_PUPDR_FLOATING(GPIOA_RightRear_DIAG) |\
                                     PIN_PUPDR_FLOATING(GPIOA_PWR_ENABLED_SIG) |\
                                     PIN_PUPDR_PULLUP(GPIOA_CAN_RX) |       \
                                     PIN_PUPDR_PULLUP(GPIOA_CAN_TX) |       \
                                     PIN_PUPDR_PULLUP(GPIOA_JTMS) |         \
                                     PIN_PUPDR_PULLDOWN(GPIOA_JTCLK) |      \
                                     PIN_PUPDR_FLOATING(GPIOA_JTDI))
#define VAL_GPIOA_ODR               (PIN_ODR_HIGH(GPIOA_LeftRear_CS) |      \
                                     PIN_ODR_HIGH(GPIOA_RightRear_CS) |     \
                                     PIN_ODR_HIGH(GPIOA_LeftFront_CS) |     \
                                     PIN_ODR_HIGH(GPIOA_RightFront_CS) |    \
                                     PIN_ODR_HIGH(GPIOA_LR_speed_in) |      \
                                     PIN_ODR_HIGH(GPIOA_RR_speed_in) |      \
                                     PIN_ODR_HIGH(GPIOA_LF_speed_in) |      \
                                     PIN_ODR_LOW(GPIOA_RF_speed_in) |       \
                                     PIN_ODR_HIGH(GPIOA_LeftRear_DIAG) |    \
                                     PIN_ODR_HIGH(GPIOA_RightRear_DIAG) |   \
                                     PIN_ODR_HIGH(GPIOA_PWR_ENABLED_SIG) |  \
                                     PIN_ODR_HIGH(GPIOA_CAN_RX) |           \
                                     PIN_ODR_LOW(GPIOA_CAN_TX) |            \
                                     PIN_ODR_HIGH(GPIOA_JTMS) |             \
                                     PIN_ODR_LOW(GPIOA_JTCLK) |             \
                                     PIN_ODR_HIGH(GPIOA_JTDI))
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(GPIOA_LeftRear_CS, 0U) |   \
                                     PIN_AFIO_AF(GPIOA_RightRear_CS, 0U) |  \
                                     PIN_AFIO_AF(GPIOA_LeftFront_CS, 0U) |  \
                                     PIN_AFIO_AF(GPIOA_RightFront_CS, 0U) | \
                                     PIN_AFIO_AF(GPIOA_LR_speed_in, 0U) |   \
                                     PIN_AFIO_AF(GPIOA_RR_speed_in, 5U) |   \
                                     PIN_AFIO_AF(GPIOA_LF_speed_in, 0U) |   \
                                     PIN_AFIO_AF(GPIOA_RF_speed_in, 0U))
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(GPIOA_LeftRear_DIAG, 0U) | \
                                     PIN_AFIO_AF(GPIOA_RightRear_DIAG, 0U) |\
                                     PIN_AFIO_AF(GPIOA_PWR_ENABLED_SIG, 0U) |\
                                     PIN_AFIO_AF(GPIOA_CAN_RX, 9U) |        \
                                     PIN_AFIO_AF(GPIOA_CAN_TX, 9U) |        \
                                     PIN_AFIO_AF(GPIOA_JTMS, 0U) |          \
                                     PIN_AFIO_AF(GPIOA_JTCLK, 0U) |         \
                                     PIN_AFIO_AF(GPIOA_JTDI, 0U))

/*
 * GPIOB setup:
 *
 * PB0  - PWR_voltsense             (analog).
 * PB1  - LeftFront_DIAG            (input pullup).
 * PB2  - RightFront_DIAG           (input pullup).
 * PB3  - JTDO                      (alternate 0).
 * PB4  - JTNRST                    (alternate 0).
 * PB5  - Bridge_Disable            (output pushpull maximum).
 * PB6  - USART_TX                  (alternate 7).
 * PB7  - USART_RX                  (alternate 7).
 * PB8  - L_speed_out               (output pushpull maximum).
 * PB9  - L_speed_out_inv           (output pushpull maximum).
 * PB10 - PIN10                     (input pullup).
 * PB11 - PIN11                     (input pullup).
 * PB12 - PIN12                     (input pullup).
 * PB13 - PIN13                     (input pullup).
 * PB14 - R_speed_out               (output pushpull maximum).
 * PB15 - R_speed_out_inv           (output pushpull maximum).
 */
#define VAL_GPIOB_MODER             (PIN_MODE_ANALOG(GPIOB_PWR_voltsense) | \
                                     PIN_MODE_INPUT(GPIOB_LeftFront_DIAG) | \
                                     PIN_MODE_INPUT(GPIOB_RightFront_DIAG) |\
                                     PIN_MODE_ALTERNATE(GPIOB_JTDO) |       \
                                     PIN_MODE_ALTERNATE(GPIOB_JTNRST) |     \
                                     PIN_MODE_OUTPUT(GPIOB_Bridge_Disable) |\
                                     PIN_MODE_ALTERNATE(GPIOB_USART_TX) |   \
                                     PIN_MODE_ALTERNATE(GPIOB_USART_RX) |   \
                                     PIN_MODE_OUTPUT(GPIOB_L_speed_out) |   \
                                     PIN_MODE_OUTPUT(GPIOB_L_speed_out_inv) |\
                                     PIN_MODE_INPUT(GPIOB_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOB_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOB_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOB_PIN13) |          \
                                     PIN_MODE_OUTPUT(GPIOB_R_speed_out) |   \
                                     PIN_MODE_OUTPUT(GPIOB_R_speed_out_inv))
#define VAL_GPIOB_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOB_PWR_voltsense) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_LeftFront_DIAG) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_RightFront_DIAG) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_JTDO) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_JTNRST) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOB_Bridge_Disable) |\
                                     PIN_OTYPE_OPENDRAIN(GPIOB_USART_TX) |  \
                                     PIN_OTYPE_OPENDRAIN(GPIOB_USART_RX) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOB_L_speed_out) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_L_speed_out_inv) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOB_R_speed_out) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_R_speed_out_inv))
#define VAL_GPIOB_OSPEEDR           (PIN_OSPEED_VERYLOW(GPIOB_PWR_voltsense) |\
                                     PIN_OSPEED_HIGH(GPIOB_LeftFront_DIAG) |\
                                     PIN_OSPEED_HIGH(GPIOB_RightFront_DIAG) |\
                                     PIN_OSPEED_HIGH(GPIOB_JTDO) |          \
                                     PIN_OSPEED_HIGH(GPIOB_JTNRST) |        \
                                     PIN_OSPEED_HIGH(GPIOB_Bridge_Disable) |\
                                     PIN_OSPEED_HIGH(GPIOB_USART_TX) |      \
                                     PIN_OSPEED_HIGH(GPIOB_USART_RX) |      \
                                     PIN_OSPEED_HIGH(GPIOB_L_speed_out) |   \
                                     PIN_OSPEED_HIGH(GPIOB_L_speed_out_inv) |\
                                     PIN_OSPEED_VERYLOW(GPIOB_PIN10) |      \
                                     PIN_OSPEED_VERYLOW(GPIOB_PIN11) |      \
                                     PIN_OSPEED_VERYLOW(GPIOB_PIN12) |      \
                                     PIN_OSPEED_VERYLOW(GPIOB_PIN13) |      \
                                     PIN_OSPEED_HIGH(GPIOB_R_speed_out) |   \
                                     PIN_OSPEED_HIGH(GPIOB_R_speed_out_inv))
#define VAL_GPIOB_PUPDR             (PIN_PUPDR_FLOATING(GPIOB_PWR_voltsense) |\
                                     PIN_PUPDR_PULLUP(GPIOB_LeftFront_DIAG) |\
                                     PIN_PUPDR_PULLUP(GPIOB_RightFront_DIAG) |\
                                     PIN_PUPDR_FLOATING(GPIOB_JTDO) |       \
                                     PIN_PUPDR_FLOATING(GPIOB_JTNRST) |     \
                                     PIN_PUPDR_PULLUP(GPIOB_Bridge_Disable) |\
                                     PIN_PUPDR_FLOATING(GPIOB_USART_TX) |   \
                                     PIN_PUPDR_PULLUP(GPIOB_USART_RX) |     \
                                     PIN_PUPDR_PULLUP(GPIOB_L_speed_out) |  \
                                     PIN_PUPDR_PULLUP(GPIOB_L_speed_out_inv) |\
                                     PIN_PUPDR_PULLUP(GPIOB_PIN10) |        \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN11) |        \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN12) |        \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN13) |        \
                                     PIN_PUPDR_PULLUP(GPIOB_R_speed_out) |  \
                                     PIN_PUPDR_PULLUP(GPIOB_R_speed_out_inv))
#define VAL_GPIOB_ODR               (PIN_ODR_HIGH(GPIOB_PWR_voltsense) |    \
                                     PIN_ODR_HIGH(GPIOB_LeftFront_DIAG) |   \
                                     PIN_ODR_HIGH(GPIOB_RightFront_DIAG) |  \
                                     PIN_ODR_HIGH(GPIOB_JTDO) |             \
                                     PIN_ODR_HIGH(GPIOB_JTNRST) |           \
                                     PIN_ODR_HIGH(GPIOB_Bridge_Disable) |   \
                                     PIN_ODR_HIGH(GPIOB_USART_TX) |         \
                                     PIN_ODR_HIGH(GPIOB_USART_RX) |         \
                                     PIN_ODR_HIGH(GPIOB_L_speed_out) |      \
                                     PIN_ODR_LOW(GPIOB_L_speed_out_inv) |   \
                                     PIN_ODR_HIGH(GPIOB_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOB_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOB_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOB_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOB_R_speed_out) |      \
                                     PIN_ODR_LOW(GPIOB_R_speed_out_inv))
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(GPIOB_PWR_voltsense, 0U) | \
                                     PIN_AFIO_AF(GPIOB_LeftFront_DIAG, 0U) |\
                                     PIN_AFIO_AF(GPIOB_RightFront_DIAG, 0U) |\
                                     PIN_AFIO_AF(GPIOB_JTDO, 0U) |          \
                                     PIN_AFIO_AF(GPIOB_JTNRST, 0U) |        \
                                     PIN_AFIO_AF(GPIOB_Bridge_Disable, 0U) |\
                                     PIN_AFIO_AF(GPIOB_USART_TX, 7U) |      \
                                     PIN_AFIO_AF(GPIOB_USART_RX, 7U))
#define VAL_GPIOB_AFRH              (PIN_AFIO_AF(GPIOB_L_speed_out, 0U) |   \
                                     PIN_AFIO_AF(GPIOB_L_speed_out_inv, 0U) |\
                                     PIN_AFIO_AF(GPIOB_PIN10, 0U) |         \
                                     PIN_AFIO_AF(GPIOB_PIN11, 0U) |         \
                                     PIN_AFIO_AF(GPIOB_PIN12, 0U) |         \
                                     PIN_AFIO_AF(GPIOB_PIN13, 0U) |         \
                                     PIN_AFIO_AF(GPIOB_R_speed_out, 0U) |   \
                                     PIN_AFIO_AF(GPIOB_R_speed_out_inv, 0U))

/*
 * GPIOC setup:
 *
 * PC0  - Feed_voltsense            (analog).
 * PC1  - INDICATOR_ON              (output opendrain minimum).
 * PC2  - LeftFront_Tighten         (output opendrain maximum).
 * PC3  - LeftFront_Loosen          (output opendrain maximum).
 * PC4  - RightFront_Tighten        (output pushpull maximum).
 * PC5  - RightFront_Loosen         (output pushpull maximum).
 * PC6  - SPI_CS                    (alternate 5).
 * PC7  - SPI_SCK                   (alternate 5).
 * PC8  - SPI_MISO                  (alternate 5).
 * PC9  - SPI_MOSI                  (alternate 5).
 * PC10 - LeftRear_Tighten          (output pushpull maximum).
 * PC11 - LeftRear_Loosen           (output pushpull maximum).
 * PC12 - RightRear_Tighten         (output pushpull maximum).
 * PC13 - RightRear_Loosen          (output pushpull maximum).
 * PC14 - uC_SET_POWER              (output pushpull maximum).
 * PC15 - VR_diag_test              (output pushpull minimum).
 */
#define VAL_GPIOC_MODER             (PIN_MODE_ANALOG(GPIOC_Feed_voltsense) |\
                                     PIN_MODE_OUTPUT(GPIOC_INDICATOR_ON) |  \
                                     PIN_MODE_OUTPUT(GPIOC_LeftFront_Tighten) |\
                                     PIN_MODE_OUTPUT(GPIOC_LeftFront_Loosen) |\
                                     PIN_MODE_OUTPUT(GPIOC_RightFront_Tighten) |\
                                     PIN_MODE_OUTPUT(GPIOC_RightFront_Loosen) |\
                                     PIN_MODE_ALTERNATE(GPIOC_SPI_CS) |     \
                                     PIN_MODE_ALTERNATE(GPIOC_SPI_SCK) |    \
                                     PIN_MODE_ALTERNATE(GPIOC_SPI_MISO) |   \
                                     PIN_MODE_ALTERNATE(GPIOC_SPI_MOSI) |   \
                                     PIN_MODE_OUTPUT(GPIOC_LeftRear_Tighten) |\
                                     PIN_MODE_OUTPUT(GPIOC_LeftRear_Loosen) |\
                                     PIN_MODE_OUTPUT(GPIOC_RightRear_Tighten) |\
                                     PIN_MODE_OUTPUT(GPIOC_RightRear_Loosen) |\
                                     PIN_MODE_OUTPUT(GPIOC_uC_SET_POWER) |  \
                                     PIN_MODE_OUTPUT(GPIOC_VR_diag_test))
#define VAL_GPIOC_OTYPER            (PIN_OTYPE_OPENDRAIN(GPIOC_Feed_voltsense) |\
                                     PIN_OTYPE_OPENDRAIN(GPIOC_INDICATOR_ON) |\
                                     PIN_OTYPE_OPENDRAIN(GPIOC_LeftFront_Tighten) |\
                                     PIN_OTYPE_OPENDRAIN(GPIOC_LeftFront_Loosen) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_RightFront_Tighten) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_RightFront_Loosen) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_SPI_CS) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOC_SPI_SCK) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOC_SPI_MISO) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOC_SPI_MOSI) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOC_LeftRear_Tighten) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_LeftRear_Loosen) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_RightRear_Tighten) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_RightRear_Loosen) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_uC_SET_POWER) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_VR_diag_test))
#define VAL_GPIOC_OSPEEDR           (PIN_OSPEED_VERYLOW(GPIOC_Feed_voltsense) |\
                                     PIN_OSPEED_VERYLOW(GPIOC_INDICATOR_ON) |\
                                     PIN_OSPEED_HIGH(GPIOC_LeftFront_Tighten) |\
                                     PIN_OSPEED_HIGH(GPIOC_LeftFront_Loosen) |\
                                     PIN_OSPEED_HIGH(GPIOC_RightFront_Tighten) |\
                                     PIN_OSPEED_HIGH(GPIOC_RightFront_Loosen) |\
                                     PIN_OSPEED_HIGH(GPIOC_SPI_CS) |        \
                                     PIN_OSPEED_HIGH(GPIOC_SPI_SCK) |       \
                                     PIN_OSPEED_HIGH(GPIOC_SPI_MISO) |      \
                                     PIN_OSPEED_HIGH(GPIOC_SPI_MOSI) |      \
                                     PIN_OSPEED_HIGH(GPIOC_LeftRear_Tighten) |\
                                     PIN_OSPEED_HIGH(GPIOC_LeftRear_Loosen) |\
                                     PIN_OSPEED_HIGH(GPIOC_RightRear_Tighten) |\
                                     PIN_OSPEED_HIGH(GPIOC_RightRear_Loosen) |\
                                     PIN_OSPEED_HIGH(GPIOC_uC_SET_POWER) |  \
                                     PIN_OSPEED_VERYLOW(GPIOC_VR_diag_test))
#define VAL_GPIOC_PUPDR             (PIN_PUPDR_FLOATING(GPIOC_Feed_voltsense) |\
                                     PIN_PUPDR_FLOATING(GPIOC_INDICATOR_ON) |\
                                     PIN_PUPDR_FLOATING(GPIOC_LeftFront_Tighten) |\
                                     PIN_PUPDR_FLOATING(GPIOC_LeftFront_Loosen) |\
                                     PIN_PUPDR_PULLUP(GPIOC_RightFront_Tighten) |\
                                     PIN_PUPDR_FLOATING(GPIOC_RightFront_Loosen) |\
                                     PIN_PUPDR_FLOATING(GPIOC_SPI_CS) |     \
                                     PIN_PUPDR_FLOATING(GPIOC_SPI_SCK) |    \
                                     PIN_PUPDR_PULLUP(GPIOC_SPI_MISO) |     \
                                     PIN_PUPDR_PULLUP(GPIOC_SPI_MOSI) |     \
                                     PIN_PUPDR_FLOATING(GPIOC_LeftRear_Tighten) |\
                                     PIN_PUPDR_FLOATING(GPIOC_LeftRear_Loosen) |\
                                     PIN_PUPDR_FLOATING(GPIOC_RightRear_Tighten) |\
                                     PIN_PUPDR_FLOATING(GPIOC_RightRear_Loosen) |\
                                     PIN_PUPDR_FLOATING(GPIOC_uC_SET_POWER) |\
                                     PIN_PUPDR_FLOATING(GPIOC_VR_diag_test))
#define VAL_GPIOC_ODR               (PIN_ODR_HIGH(GPIOC_Feed_voltsense) |   \
                                     PIN_ODR_LOW(GPIOC_INDICATOR_ON) |      \
                                     PIN_ODR_LOW(GPIOC_LeftFront_Tighten) | \
                                     PIN_ODR_LOW(GPIOC_LeftFront_Loosen) |  \
                                     PIN_ODR_LOW(GPIOC_RightFront_Tighten) |\
                                     PIN_ODR_LOW(GPIOC_RightFront_Loosen) | \
                                     PIN_ODR_HIGH(GPIOC_SPI_CS) |           \
                                     PIN_ODR_HIGH(GPIOC_SPI_SCK) |          \
                                     PIN_ODR_HIGH(GPIOC_SPI_MISO) |         \
                                     PIN_ODR_HIGH(GPIOC_SPI_MOSI) |         \
                                     PIN_ODR_LOW(GPIOC_LeftRear_Tighten) |  \
                                     PIN_ODR_LOW(GPIOC_LeftRear_Loosen) |   \
                                     PIN_ODR_LOW(GPIOC_RightRear_Tighten) | \
                                     PIN_ODR_LOW(GPIOC_RightRear_Loosen) |  \
                                     PIN_ODR_LOW(GPIOC_uC_SET_POWER) |      \
                                     PIN_ODR_LOW(GPIOC_VR_diag_test))
#define VAL_GPIOC_AFRL              (PIN_AFIO_AF(GPIOC_Feed_voltsense, 0U) |\
                                     PIN_AFIO_AF(GPIOC_INDICATOR_ON, 0U) |  \
                                     PIN_AFIO_AF(GPIOC_LeftFront_Tighten, 0U) |\
                                     PIN_AFIO_AF(GPIOC_LeftFront_Loosen, 0U) |\
                                     PIN_AFIO_AF(GPIOC_RightFront_Tighten, 0U) |\
                                     PIN_AFIO_AF(GPIOC_RightFront_Loosen, 0U) |\
                                     PIN_AFIO_AF(GPIOC_SPI_CS, 5U) |        \
                                     PIN_AFIO_AF(GPIOC_SPI_SCK, 5U))
#define VAL_GPIOC_AFRH              (PIN_AFIO_AF(GPIOC_SPI_MISO, 5U) |      \
                                     PIN_AFIO_AF(GPIOC_SPI_MOSI, 5U) |      \
                                     PIN_AFIO_AF(GPIOC_LeftRear_Tighten, 0U) |\
                                     PIN_AFIO_AF(GPIOC_LeftRear_Loosen, 0U) |\
                                     PIN_AFIO_AF(GPIOC_RightRear_Tighten, 0U) |\
                                     PIN_AFIO_AF(GPIOC_RightRear_Loosen, 0U) |\
                                     PIN_AFIO_AF(GPIOC_uC_SET_POWER, 0U) |  \
                                     PIN_AFIO_AF(GPIOC_VR_diag_test, 0U))

/*
 * GPIOD setup:
 *
 * PD0  - PIN0                      (alternate 7).
 * PD1  - PIN1                      (alternate 7).
 * PD2  - IGN_ON_SIG                (input pulldown).
 * PD3  - PIN3                      (alternate 7).
 * PD4  - PIN4                      (alternate 7).
 * PD5  - PIN5                      (alternate 7).
 * PD6  - PIN6                      (alternate 7).
 * PD7  - PIN7                      (input pullup).
 * PD8  - LIGHTS_ON_SIG             (input pulldown).
 * PD9  - PIN9                      (input pullup).
 * PD10 - PIN10                     (input pullup).
 * PD11 - PIN11                     (output pushpull maximum).
 * PD12 - PIN12                     (input pullup).
 * PD13 - PIN13                     (input pullup).
 * PD14 - PIN14                     (input pullup).
 * PD15 - PIN15                     (input pullup).
 */
#define VAL_GPIOD_MODER             (PIN_MODE_ALTERNATE(GPIOD_PIN0) |       \
                                     PIN_MODE_ALTERNATE(GPIOD_PIN1) |       \
                                     PIN_MODE_INPUT(GPIOD_IGN_ON_SIG) |     \
                                     PIN_MODE_ALTERNATE(GPIOD_PIN3) |       \
                                     PIN_MODE_ALTERNATE(GPIOD_PIN4) |       \
                                     PIN_MODE_ALTERNATE(GPIOD_PIN5) |       \
                                     PIN_MODE_ALTERNATE(GPIOD_PIN6) |       \
                                     PIN_MODE_INPUT(GPIOD_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOD_LIGHTS_ON_SIG) |  \
                                     PIN_MODE_INPUT(GPIOD_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN10) |          \
                                     PIN_MODE_OUTPUT(GPIOD_PIN11) |         \
                                     PIN_MODE_INPUT(GPIOD_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOD_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOD_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOD_PIN15))
#define VAL_GPIOD_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOD_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_IGN_ON_SIG) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_LIGHTS_ON_SIG) |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN15))
#define VAL_GPIOD_OSPEEDR           (PIN_OSPEED_HIGH(GPIOD_PIN0) |          \
                                     PIN_OSPEED_HIGH(GPIOD_PIN1) |          \
                                     PIN_OSPEED_HIGH(GPIOD_IGN_ON_SIG) |    \
                                     PIN_OSPEED_HIGH(GPIOD_PIN3) |          \
                                     PIN_OSPEED_HIGH(GPIOD_PIN4) |          \
                                     PIN_OSPEED_HIGH(GPIOD_PIN5) |          \
                                     PIN_OSPEED_HIGH(GPIOD_PIN6) |          \
                                     PIN_OSPEED_VERYLOW(GPIOD_PIN7) |       \
                                     PIN_OSPEED_VERYLOW(GPIOD_LIGHTS_ON_SIG) |\
                                     PIN_OSPEED_VERYLOW(GPIOD_PIN9) |       \
                                     PIN_OSPEED_VERYLOW(GPIOD_PIN10) |      \
                                     PIN_OSPEED_HIGH(GPIOD_PIN11) |         \
                                     PIN_OSPEED_VERYLOW(GPIOD_PIN12) |      \
                                     PIN_OSPEED_VERYLOW(GPIOD_PIN13) |      \
                                     PIN_OSPEED_VERYLOW(GPIOD_PIN14) |      \
                                     PIN_OSPEED_VERYLOW(GPIOD_PIN15))
#define VAL_GPIOD_PUPDR             (PIN_PUPDR_FLOATING(GPIOD_PIN0) |       \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN1) |       \
                                     PIN_PUPDR_PULLDOWN(GPIOD_IGN_ON_SIG) | \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN6) |       \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN7) |         \
                                     PIN_PUPDR_PULLDOWN(GPIOD_LIGHTS_ON_SIG) |\
                                     PIN_PUPDR_PULLUP(GPIOD_PIN9) |         \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN10) |        \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN11) |      \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN12) |        \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN13) |        \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN14) |        \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN15))
#define VAL_GPIOD_ODR               (PIN_ODR_HIGH(GPIOD_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOD_PIN1) |             \
                                     PIN_ODR_LOW(GPIOD_IGN_ON_SIG) |        \
                                     PIN_ODR_HIGH(GPIOD_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOD_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOD_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOD_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOD_PIN7) |             \
                                     PIN_ODR_LOW(GPIOD_LIGHTS_ON_SIG) |     \
                                     PIN_ODR_HIGH(GPIOD_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOD_PIN10) |            \
                                     PIN_ODR_LOW(GPIOD_PIN11) |             \
                                     PIN_ODR_HIGH(GPIOD_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOD_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOD_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOD_PIN15))
#define VAL_GPIOD_AFRL              (PIN_AFIO_AF(GPIOD_PIN0, 7U) |          \
                                     PIN_AFIO_AF(GPIOD_PIN1, 7U) |          \
                                     PIN_AFIO_AF(GPIOD_IGN_ON_SIG, 0U) |    \
                                     PIN_AFIO_AF(GPIOD_PIN3, 7U) |          \
                                     PIN_AFIO_AF(GPIOD_PIN4, 7U) |          \
                                     PIN_AFIO_AF(GPIOD_PIN5, 7U) |          \
                                     PIN_AFIO_AF(GPIOD_PIN6, 7U) |          \
                                     PIN_AFIO_AF(GPIOD_PIN7, 0U))
#define VAL_GPIOD_AFRH              (PIN_AFIO_AF(GPIOD_LIGHTS_ON_SIG, 0U) | \
                                     PIN_AFIO_AF(GPIOD_PIN9, 0U) |          \
                                     PIN_AFIO_AF(GPIOD_PIN10, 0U) |         \
                                     PIN_AFIO_AF(GPIOD_PIN11, 0U) |         \
                                     PIN_AFIO_AF(GPIOD_PIN12, 0U) |         \
                                     PIN_AFIO_AF(GPIOD_PIN13, 0U) |         \
                                     PIN_AFIO_AF(GPIOD_PIN14, 0U) |         \
                                     PIN_AFIO_AF(GPIOD_PIN15, 0U))

/*
 * GPIOE setup:
 *
 * PE0  - PIN0                      (input pullup).
 * PE1  - PIN1                      (input pullup).
 * PE2  - PIN2                      (output opendrain maximum).
 * PE3  - PIN3                      (input pullup).
 * PE4  - PIN4                      (input pullup).
 * PE5  - PIN5                      (input pullup).
 * PE6  - PIN6                      (input pulldown).
 * PE7  - PIN7                      (analog).
 * PE8  - BUTTON_SIG                (input pulldown).
 * PE9  - BUTTON_INV_SIG            (input pullup).
 * PE10 - PIN10                     (input pullup).
 * PE11 - PIN11                     (input pullup).
 * PE12 - PIN12                     (input pullup).
 * PE13 - PIN13                     (input pullup).
 * PE14 - PIN14                     (input floating).
 * PE15 - PIN15                     (input pullup).
 */
#define VAL_GPIOE_MODER             (PIN_MODE_INPUT(GPIOE_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN1) |           \
                                     PIN_MODE_OUTPUT(GPIOE_PIN2) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN6) |           \
                                     PIN_MODE_ANALOG(GPIOE_PIN7) |          \
                                     PIN_MODE_INPUT(GPIOE_BUTTON_SIG) |     \
                                     PIN_MODE_INPUT(GPIOE_BUTTON_INV_SIG) | \
                                     PIN_MODE_INPUT(GPIOE_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN15))
#define VAL_GPIOE_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOE_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN1) |       \
                                     PIN_OTYPE_OPENDRAIN(GPIOE_PIN2) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_BUTTON_SIG) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_BUTTON_INV_SIG) |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN15))
#define VAL_GPIOE_OSPEEDR           (PIN_OSPEED_VERYLOW(GPIOE_PIN0) |       \
                                     PIN_OSPEED_VERYLOW(GPIOE_PIN1) |       \
                                     PIN_OSPEED_HIGH(GPIOE_PIN2) |          \
                                     PIN_OSPEED_HIGH(GPIOE_PIN3) |          \
                                     PIN_OSPEED_VERYLOW(GPIOE_PIN4) |       \
                                     PIN_OSPEED_VERYLOW(GPIOE_PIN5) |       \
                                     PIN_OSPEED_HIGH(GPIOE_PIN6) |          \
                                     PIN_OSPEED_VERYLOW(GPIOE_PIN7) |       \
                                     PIN_OSPEED_HIGH(GPIOE_BUTTON_SIG) |    \
                                     PIN_OSPEED_HIGH(GPIOE_BUTTON_INV_SIG) |\
                                     PIN_OSPEED_VERYLOW(GPIOE_PIN10) |      \
                                     PIN_OSPEED_VERYLOW(GPIOE_PIN11) |      \
                                     PIN_OSPEED_VERYLOW(GPIOE_PIN12) |      \
                                     PIN_OSPEED_VERYLOW(GPIOE_PIN13) |      \
                                     PIN_OSPEED_VERYLOW(GPIOE_PIN14) |      \
                                     PIN_OSPEED_VERYLOW(GPIOE_PIN15))
#define VAL_GPIOE_PUPDR             (PIN_PUPDR_PULLUP(GPIOE_PIN0) |         \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN1) |         \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN2) |       \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN3) |         \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN4) |         \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN5) |         \
                                     PIN_PUPDR_PULLDOWN(GPIOE_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN7) |       \
                                     PIN_PUPDR_PULLDOWN(GPIOE_BUTTON_SIG) | \
                                     PIN_PUPDR_PULLUP(GPIOE_BUTTON_INV_SIG) |\
                                     PIN_PUPDR_PULLUP(GPIOE_PIN10) |        \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN11) |        \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN12) |        \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN13) |        \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN14) |      \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN15))
#define VAL_GPIOE_ODR               (PIN_ODR_HIGH(GPIOE_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN7) |             \
                                     PIN_ODR_LOW(GPIOE_BUTTON_SIG) |        \
                                     PIN_ODR_HIGH(GPIOE_BUTTON_INV_SIG) |   \
                                     PIN_ODR_HIGH(GPIOE_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOE_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOE_PIN12) |            \
                                     PIN_ODR_LOW(GPIOE_PIN13) |             \
                                     PIN_ODR_LOW(GPIOE_PIN14) |             \
                                     PIN_ODR_LOW(GPIOE_PIN15))
#define VAL_GPIOE_AFRL              (PIN_AFIO_AF(GPIOE_PIN0, 0U) |          \
                                     PIN_AFIO_AF(GPIOE_PIN1, 0U) |          \
                                     PIN_AFIO_AF(GPIOE_PIN2, 0U) |          \
                                     PIN_AFIO_AF(GPIOE_PIN3, 0U) |          \
                                     PIN_AFIO_AF(GPIOE_PIN4, 0U) |          \
                                     PIN_AFIO_AF(GPIOE_PIN5, 0U) |          \
                                     PIN_AFIO_AF(GPIOE_PIN6, 0U) |          \
                                     PIN_AFIO_AF(GPIOE_PIN7, 0U))
#define VAL_GPIOE_AFRH              (PIN_AFIO_AF(GPIOE_BUTTON_SIG, 0U) |    \
                                     PIN_AFIO_AF(GPIOE_BUTTON_INV_SIG, 0U) |\
                                     PIN_AFIO_AF(GPIOE_PIN10, 0U) |         \
                                     PIN_AFIO_AF(GPIOE_PIN11, 0U) |         \
                                     PIN_AFIO_AF(GPIOE_PIN12, 0U) |         \
                                     PIN_AFIO_AF(GPIOE_PIN13, 0U) |         \
                                     PIN_AFIO_AF(GPIOE_PIN14, 0U) |         \
                                     PIN_AFIO_AF(GPIOE_PIN15, 0U))

/*
 * GPIOF setup:
 *
 * PF0  - OSC_IN                    (input floating).
 * PF1  - OSC_OUT                   (input floating).
 * PF2  - PIN2                      (input pulldown).
 * PF3  - PIN3                      (input pullup).
 * PF4  - PIN4                      (input pulldown).
 * PF5  - PIN5                      (input pullup).
 * PF6  - TIGHTEN_CMD_SIG           (input pullup).
 * PF7  - RELEASE_CMD_SIG           (input pulldown).
 * PF8  - PIN8                      (input pullup).
 * PF9  - PIN9                      (input pulldown).
 * PF10 - PIN10                     (input pulldown).
 * PF11 - PIN11                     (input pullup).
 * PF12 - PIN12                     (input pullup).
 * PF13 - PIN13                     (input pullup).
 * PF14 - PIN14                     (input pullup).
 * PF15 - PIN15                     (input pullup).
 */
#define VAL_GPIOF_MODER             (PIN_MODE_INPUT(GPIOF_OSC_IN) |         \
                                     PIN_MODE_INPUT(GPIOF_OSC_OUT) |        \
                                     PIN_MODE_INPUT(GPIOF_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOF_TIGHTEN_CMD_SIG) |\
                                     PIN_MODE_INPUT(GPIOF_RELEASE_CMD_SIG) |\
                                     PIN_MODE_INPUT(GPIOF_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN15))
#define VAL_GPIOF_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOF_OSC_IN) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOF_OSC_OUT) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_TIGHTEN_CMD_SIG) |\
                                     PIN_OTYPE_PUSHPULL(GPIOF_RELEASE_CMD_SIG) |\
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN15))
#define VAL_GPIOF_OSPEEDR           (PIN_OSPEED_HIGH(GPIOF_OSC_IN) |        \
                                     PIN_OSPEED_HIGH(GPIOF_OSC_OUT) |       \
                                     PIN_OSPEED_HIGH(GPIOF_PIN2) |          \
                                     PIN_OSPEED_VERYLOW(GPIOF_PIN3) |       \
                                     PIN_OSPEED_HIGH(GPIOF_PIN4) |          \
                                     PIN_OSPEED_VERYLOW(GPIOF_PIN5) |       \
                                     PIN_OSPEED_VERYLOW(GPIOF_TIGHTEN_CMD_SIG) |\
                                     PIN_OSPEED_VERYLOW(GPIOF_RELEASE_CMD_SIG) |\
                                     PIN_OSPEED_VERYLOW(GPIOF_PIN8) |       \
                                     PIN_OSPEED_HIGH(GPIOF_PIN9) |          \
                                     PIN_OSPEED_HIGH(GPIOF_PIN10) |         \
                                     PIN_OSPEED_VERYLOW(GPIOF_PIN11) |      \
                                     PIN_OSPEED_VERYLOW(GPIOF_PIN12) |      \
                                     PIN_OSPEED_VERYLOW(GPIOF_PIN13) |      \
                                     PIN_OSPEED_VERYLOW(GPIOF_PIN14) |      \
                                     PIN_OSPEED_VERYLOW(GPIOF_PIN15))
#define VAL_GPIOF_PUPDR             (PIN_PUPDR_FLOATING(GPIOF_OSC_IN) |     \
                                     PIN_PUPDR_FLOATING(GPIOF_OSC_OUT) |    \
                                     PIN_PUPDR_PULLDOWN(GPIOF_PIN2) |       \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN3) |         \
                                     PIN_PUPDR_PULLDOWN(GPIOF_PIN4) |       \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN5) |         \
                                     PIN_PUPDR_PULLUP(GPIOF_TIGHTEN_CMD_SIG) |\
                                     PIN_PUPDR_PULLDOWN(GPIOF_RELEASE_CMD_SIG) |\
                                     PIN_PUPDR_PULLUP(GPIOF_PIN8) |         \
                                     PIN_PUPDR_PULLDOWN(GPIOF_PIN9) |       \
                                     PIN_PUPDR_PULLDOWN(GPIOF_PIN10) |      \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN11) |        \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN12) |        \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN13) |        \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN14) |        \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN15))
#define VAL_GPIOF_ODR               (PIN_ODR_HIGH(GPIOF_OSC_IN) |           \
                                     PIN_ODR_HIGH(GPIOF_OSC_OUT) |          \
                                     PIN_ODR_HIGH(GPIOF_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOF_TIGHTEN_CMD_SIG) |  \
                                     PIN_ODR_LOW(GPIOF_RELEASE_CMD_SIG) |   \
                                     PIN_ODR_HIGH(GPIOF_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN15))
#define VAL_GPIOF_AFRL              (PIN_AFIO_AF(GPIOF_OSC_IN, 0U) |        \
                                     PIN_AFIO_AF(GPIOF_OSC_OUT, 0U) |       \
                                     PIN_AFIO_AF(GPIOF_PIN2, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN3, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN4, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN5, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_TIGHTEN_CMD_SIG, 0U) |\
                                     PIN_AFIO_AF(GPIOF_RELEASE_CMD_SIG, 0U))
#define VAL_GPIOF_AFRH              (PIN_AFIO_AF(GPIOF_PIN8, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN9, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN10, 0U) |         \
                                     PIN_AFIO_AF(GPIOF_PIN11, 0U) |         \
                                     PIN_AFIO_AF(GPIOF_PIN12, 0U) |         \
                                     PIN_AFIO_AF(GPIOF_PIN13, 0U) |         \
                                     PIN_AFIO_AF(GPIOF_PIN14, 0U) |         \
                                     PIN_AFIO_AF(GPIOF_PIN15, 0U))

/*
 * GPIOG setup:
 *
 * PG0  - PIN0                      (input pullup).
 * PG1  - PIN1                      (input pullup).
 * PG2  - PIN2                      (input pullup).
 * PG3  - PIN3                      (input pullup).
 * PG4  - PIN4                      (input pullup).
 * PG5  - PIN5                      (input pullup).
 * PG6  - PIN6                      (input pullup).
 * PG7  - PIN7                      (input pullup).
 * PG8  - PIN8                      (input pullup).
 * PG9  - PIN9                      (input pullup).
 * PG10 - PIN10                     (input pullup).
 * PG11 - PIN11                     (input pullup).
 * PG12 - PIN12                     (input pullup).
 * PG13 - PIN13                     (input pullup).
 * PG14 - PIN14                     (input pullup).
 * PG15 - PIN15                     (input pullup).
 */
#define VAL_GPIOG_MODER             (PIN_MODE_INPUT(GPIOG_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN15))
#define VAL_GPIOG_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOG_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN15))
#define VAL_GPIOG_OSPEEDR           (PIN_OSPEED_VERYLOW(GPIOG_PIN0) |       \
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN1) |       \
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN2) |       \
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN3) |       \
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN4) |       \
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN5) |       \
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN6) |       \
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN7) |       \
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN8) |       \
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN9) |       \
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN10) |      \
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN11) |      \
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN12) |      \
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN13) |      \
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN14) |      \
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN15))
#define VAL_GPIOG_PUPDR             (PIN_PUPDR_PULLUP(GPIOG_PIN0) |         \
                                     PIN_PUPDR_PULLUP(GPIOG_PIN1) |         \
                                     PIN_PUPDR_PULLUP(GPIOG_PIN2) |         \
                                     PIN_PUPDR_PULLUP(GPIOG_PIN3) |         \
                                     PIN_PUPDR_PULLUP(GPIOG_PIN4) |         \
                                     PIN_PUPDR_PULLUP(GPIOG_PIN5) |         \
                                     PIN_PUPDR_PULLUP(GPIOG_PIN6) |         \
                                     PIN_PUPDR_PULLUP(GPIOG_PIN7) |         \
                                     PIN_PUPDR_PULLUP(GPIOG_PIN8) |         \
                                     PIN_PUPDR_PULLUP(GPIOG_PIN9) |         \
                                     PIN_PUPDR_PULLUP(GPIOG_PIN10) |        \
                                     PIN_PUPDR_PULLUP(GPIOG_PIN11) |        \
                                     PIN_PUPDR_PULLUP(GPIOG_PIN12) |        \
                                     PIN_PUPDR_PULLUP(GPIOG_PIN13) |        \
                                     PIN_PUPDR_PULLUP(GPIOG_PIN14) |        \
                                     PIN_PUPDR_PULLUP(GPIOG_PIN15))
#define VAL_GPIOG_ODR               (PIN_ODR_HIGH(GPIOG_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN15))
#define VAL_GPIOG_AFRL              (PIN_AFIO_AF(GPIOG_PIN0, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN1, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN2, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN3, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN4, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN5, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN6, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN7, 0U))
#define VAL_GPIOG_AFRH              (PIN_AFIO_AF(GPIOG_PIN8, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN9, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN10, 0U) |         \
                                     PIN_AFIO_AF(GPIOG_PIN11, 0U) |         \
                                     PIN_AFIO_AF(GPIOG_PIN12, 0U) |         \
                                     PIN_AFIO_AF(GPIOG_PIN13, 0U) |         \
                                     PIN_AFIO_AF(GPIOG_PIN14, 0U) |         \
                                     PIN_AFIO_AF(GPIOG_PIN15, 0U))

/*
 * GPIOH setup:
 *
 * PH0  - PIN0                      (input pullup).
 * PH1  - PIN1                      (input pullup).
 * PH2  - PIN2                      (input pullup).
 * PH3  - PIN3                      (input pullup).
 * PH4  - PIN4                      (input pullup).
 * PH5  - PIN5                      (input pullup).
 * PH6  - PIN6                      (input pullup).
 * PH7  - PIN7                      (input pullup).
 * PH8  - PIN8                      (input pullup).
 * PH9  - PIN9                      (input pullup).
 * PH10 - PIN10                     (input pullup).
 * PH11 - PIN11                     (input pullup).
 * PH12 - PIN12                     (input pullup).
 * PH13 - PIN13                     (input pullup).
 * PH14 - PIN14                     (input pullup).
 * PH15 - PIN15                     (input pullup).
 */
#define VAL_GPIOH_MODER             (PIN_MODE_INPUT(GPIOH_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN15))
#define VAL_GPIOH_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOH_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN15))
#define VAL_GPIOH_OSPEEDR           (PIN_OSPEED_VERYLOW(GPIOH_PIN0) |       \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN1) |       \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN2) |       \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN3) |       \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN4) |       \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN5) |       \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN6) |       \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN7) |       \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN8) |       \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN9) |       \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN10) |      \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN11) |      \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN12) |      \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN13) |      \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN14) |      \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN15))
#define VAL_GPIOH_PUPDR             (PIN_PUPDR_PULLUP(GPIOH_PIN0) |         \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN1) |         \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN2) |         \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN3) |         \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN4) |         \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN5) |         \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN6) |         \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN7) |         \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN8) |         \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN9) |         \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN10) |        \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN11) |        \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN12) |        \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN13) |        \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN14) |        \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN15))
#define VAL_GPIOH_ODR               (PIN_ODR_HIGH(GPIOH_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN15))
#define VAL_GPIOH_AFRL              (PIN_AFIO_AF(GPIOH_PIN0, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN1, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN2, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN3, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN4, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN5, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN6, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN7, 0U))
#define VAL_GPIOH_AFRH              (PIN_AFIO_AF(GPIOH_PIN8, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN9, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN10, 0U) |         \
                                     PIN_AFIO_AF(GPIOH_PIN11, 0U) |         \
                                     PIN_AFIO_AF(GPIOH_PIN12, 0U) |         \
                                     PIN_AFIO_AF(GPIOH_PIN13, 0U) |         \
                                     PIN_AFIO_AF(GPIOH_PIN14, 0U) |         \
                                     PIN_AFIO_AF(GPIOH_PIN15, 0U))


#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* BOARD_H */
