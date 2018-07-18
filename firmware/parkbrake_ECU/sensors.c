/*
 * CarTransporter
 * Copyright (C) 2017  Fredrik Johansson mumme74@github.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ch.h"
#include "hal.h"
#include "board.h"

#include "sensors.h"
#include "control.h"
#include "eeprom_setup.h"
#include <math.h> // for M_PI symbol
#include "diag.h"
#include "debug.h"
#include "can_protocol.h"


// --------- start definitions ------------------------
// NOTE ! changing these values must be accompanied by changing
// in each respective callback
// a adc group for current sense in the bridge
#define ADC_BRIDGE_NUM_CHANNELS 2
#define ADC_BRIDGE_BUF_DEPTH 4

// another adc group for voltage sense and internal tempsensor
#define ADC_BACKGROUND_NUM_CHANNELS 3
#define ADC_BACKGROUND_BUF_DEPTH 6

#define BACKGROUND_TIMER_MS 250




// --------------------------------------------------------------------
// start function prototypes

// definition callback functions, bridge currents, voltages
static void bridgeFrontAdcCallback(ADCDriver *adcp, adcsample_t *buffer, size_t n);
static void bridgeRearAdcCallback(ADCDriver *adcp, adcsample_t *buffer, size_t n);
static void backgroundAdcCallback(ADCDriver *adcp, adcsample_t *buffer, size_t n);

// interupt callback for pad change interupts
static void extCallback(EXTDriver *extp, expchannel_t channel);

// helper function
static void bridgeCurrents(uint16_t *left_ma, uint16_t *right_ma);
static void backgroundTimerCallback(void);

static void clearFrontCurrents(void);
static void clearRearCurrents(void);
static void calculateWheelCircumference(void);


// ---------------------------------------------------------------
// start globally shared variables

const sen_motor_currents_t sen_motorCurrents = { 0, 0, 0, 0, 0, 0, 0, 0 };
const sen_voltages_t       sen_voltages = { 0, 0 };
const int8_t               sen_chipTemperature = 0;
const sen_wheelspeeds_t    sen_wheelSpeeds = { 0, 0, 0, 0 };


event_source_t sen_measuredEvts,
               sen_MsgHandlerThd;


// -----------------------------------------------------------------
// start private static objects declarations needed by this file

static virtual_timer_t backgroundTimer;
static thread_t *adcHandlerp,
                *pollInputsp,
                *settingsHandlerp;

// wheel circumference used to calculate speed. Can be set by settings from eeprom
static uint16_t wheelCircumference = 2000; // in mm

// a buffer for bridge current sense, one measurement for each axle so we can toggle current
// between front and rear axle. Makes overall current consumption less in each instant
static adcsample_t bridgeAdcBuf[ADC_BRIDGE_NUM_CHANNELS * ADC_BRIDGE_BUF_DEPTH];

// a buffer for bridge voltage sense
static adcsample_t backgroundAdcBuf[ADC_BACKGROUND_NUM_CHANNELS * ADC_BACKGROUND_BUF_DEPTH];


// ADC conversion group for bridge front motor currents
// mode: 		LINEAR buffer, 4 samples of 4 channels, SW triggered
// channels:
static const ADCConversionGroup adcBridgeFrontCfg = {
    FALSE, // circular
    ADC_BRIDGE_NUM_CHANNELS,
    bridgeFrontAdcCallback, // conversion callback
    NULL,              // error callback
    //---------- stm32 specific from here on ---------------------
    {{ // begin union struct adc
        // CR1 initialization data (look in ST documentation)
        0,
        // CR2 initialization data (look in ST documentation)
        ADC_CR2_JSWSTART | ADC_CR2_TSVREFE, // start immediately,
                                           // also start temperature check, more precisely don't turn it of
        // LTR register initialization data
        // LTR = LowThreshold watchdog level
        0,
        // HTR register initialization data
        // HTR = high threshold watchdog level
        0,
        // SMPRx register initialization data
        { // is [2] array SMPR1 and SMPR2
            0, // SMPR1 empty in bridge current conversion group

            // sample time, aka how long ADC capacitor is allowed
            // to charge in ADC clock cycles (Currently 8MHz, so around 3,5us load time)
            ADC_SMPR2_SMP_AN2(ADC_SAMPLE_28P5) | // LeftFront_CS
            ADC_SMPR2_SMP_AN3(ADC_SAMPLE_28P5)   // RightFront_CS
        },
        // SQRx register initialization data
        { // is a [3] array, SQR1, SQR2, SQR3
            ADC_SQR1_NUM_CH(ADC_BRIDGE_NUM_CHANNELS), // number of channels in this conversion
            0, // SQR2, ch 7-12 not used
            ADC_SQR3_SQ3_N(ADC_CHANNEL_IN2) | // LeftFront_CS
            ADC_SQR3_SQ4_N(ADC_CHANNEL_IN3)   // RightFront_CS
        }
    }} // end union struct adc
};

// ADC conversion group for bridge front motor currents
// mode:        LINEAR buffer, 4 samples of 4 channels, SW triggered
// channels:
static const ADCConversionGroup adcBridgeRearCfg = {
    FALSE, // circular
    ADC_BRIDGE_NUM_CHANNELS,
    bridgeRearAdcCallback, // conversion callback
    NULL,              // error callback
    //---------- stm32 specific from here on ---------------------
    {{ // begin union struct adc
        // CR1 initialization data (look in ST documentation)
        0,
        // CR2 initialization data (look in ST documentation)
        ADC_CR2_JSWSTART | ADC_CR2_TSVREFE, // start immediately,
                                           // also start temperature check, more precisely don't turn it of
        // LTR register initialization data
        // LTR = LowThreshold watchdog level
        0,
        // HTR register initialization data
        // HTR = high threshold watchdog level
        0,
        // SMPRx register initialization data
        { // is [2] array SMPR1 and SMPR2
            0, // SMPR1 empty in bridge current conversion group

            // sample time, aka how long ADC capacitor is allowed
            // to charge in ADC clock cycles (Currently 8MHz, so around 3,5us load time)
            ADC_SMPR2_SMP_AN0(ADC_SAMPLE_28P5) | // LeftRear_CS
            ADC_SMPR2_SMP_AN1(ADC_SAMPLE_28P5)  // RightRear_CS
        },
        // SQRx register initialization data
        { // is a [3] array, SQR1, SQR2, SQR3
            ADC_SQR1_NUM_CH(ADC_BRIDGE_NUM_CHANNELS), // number of channels in this conversion
            0, // SQR2, ch 7-12 not used
            ADC_SQR3_SQ1_N(ADC_CHANNEL_IN0) | // LeftRear_CS
            ADC_SQR3_SQ2_N(ADC_CHANNEL_IN1)   // RightRear_CS
        }
    }} // end union struct adc
};


// ADC conversion group for voltages and chip temp
// mode:        LINEAR buffer, 6 samples of 2 channels, SW triggered
// channels:
static const ADCConversionGroup adcBackgroundCfg = {
    FALSE, // not circular, but is repeated from callback
    ADC_BACKGROUND_NUM_CHANNELS, // total 3 both voltages and chip temperature
    backgroundAdcCallback, // conversion callback
    NULL,              // error callback
    //---------- stm32 specific from here on ---------------------
    {{ // begin union struct adc
        // CR1 initialization data (look in ST documentation)
        0,
        // CR2 initialization data (look in ST documentation)
        ADC_CR2_SWSTART | ADC_CR2_TSVREFE, // start immediately, also start temperature check
        // LTR register initialization data
        // LTR = LowThreshold watchdog level
        0,
        // HTR register initialization data
        // HTR = high threshold watchdog level
        0,
        // SMPRx register initialization data
        { // is [2] array SMPR1 and SMPR2

            // sample time, aka how long ADC capacitor is allowed to charge in ADC clock cycles
            ADC_SMPR1_SMP_SENSOR(ADC_SAMPLE_239P5) | // internal temperature sensor need 17,1 us charge time

            //  (ADC cycles currently at 8MHz, so around 3,5us load time)
            ADC_SMPR1_SMP_AN10(ADC_SAMPLE_28P5), // Feed_voltsense (IGN)
            ADC_SMPR2_SMP_AN8(ADC_SAMPLE_28P5) // PWR_voltsense (Bat)
        },
        // SQRx register initialization data
        { // is a [3] array, SQR1, SQR2, SQR3
            ADC_SQR1_NUM_CH(ADC_BACKGROUND_NUM_CHANNELS), // number of channels in this conversion
            0, // SQR2, conversion nr 7-12 not used, we only use 2
            ADC_SQR3_SQ1_N(ADC_CHANNEL_IN8) | // PWR_voltsense (Bat)
            ADC_SQR3_SQ2_N(ADC_CHANNEL_IN10)|  // Feed_voltsense (IGN)
            ADC_SQR3_SQ3_N(ADC_CHANNEL_SENSOR) // internal temperature sensor
        }
    }} // end union struct adc
};


// external input interupts
static const  EXTConfig extConfig = {
    {
        // This is somewhat confusing, stm32 interrupts on lines
        // so channel0 is for pins0 like PA0, PB0, PC0 ....
        //    channel1 is for pins1 like PA1, PB1, PC1 etc
        // IMPORTANT! you can only use one channel at a time as a EXT interrupt
        // so PA0 and PB0 can't both have a interrupt configured at the same time
        // also stm32F373 has 23 extlines, 0-15 for gpio, higher for internal peripherals
        // so this struct MUST be initialized with exactly 23 channels

        {EXT_CH_MODE_DISABLED, NULL}, // line0

        // PB1 -> LeftFront_DIAG
        {
            EXT_MODE_GPIOB | EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART,
            extCallback
        }, // line1

        // PB2 -> RightFront_DIAG
        {
            EXT_MODE_GPIOB | EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART,
            extCallback
        }, // line2

        {EXT_CH_MODE_DISABLED, NULL}, // line3
        {EXT_CH_MODE_DISABLED, NULL}, // line4
        {EXT_CH_MODE_DISABLED, NULL}, // line5

        // PF6 -> TIGHTEN_CMD_SIG
        {
            EXT_MODE_GPIOF | EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART,
            extCallback
        }, // line6

        // PF7 -> RELEASE_CMD_SIG
        {
            EXT_MODE_GPIOF | EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART,
            extCallback
        }, // line7

        // PA8 -> LeftRear_DIAG
        {
            EXT_MODE_GPIOA | EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART,
            extCallback
        }, // line8

        // PA9 -> RightRear_DIAG
        {
            EXT_MODE_GPIOA | EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART,
            extCallback
        }, // line9

        // PA10 -> PWR_ENABLED_SIG
        {
            EXT_MODE_GPIOA | EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART,
            extCallback
        }, // line10

        {EXT_CH_MODE_DISABLED, NULL}, // line11
        {EXT_CH_MODE_DISABLED, NULL}, // line12
        {EXT_CH_MODE_DISABLED, NULL}, // line13
        {EXT_CH_MODE_DISABLED, NULL}, // line14
        {EXT_CH_MODE_DISABLED, NULL}, // line15
        {EXT_CH_MODE_DISABLED, NULL}, // line16
        {EXT_CH_MODE_DISABLED, NULL}, // line17
        {EXT_CH_MODE_DISABLED, NULL}, // line18
        {EXT_CH_MODE_DISABLED, NULL}, // line19
        {EXT_CH_MODE_DISABLED, NULL}, // line20
        {EXT_CH_MODE_DISABLED, NULL}, // line21
        {EXT_CH_MODE_DISABLED, NULL}, // line22
        // 0-22 is 23 items
        // must be 23 items
    }
};

// end static objects declarations
// -----------------------------------------------------------------------------------------
// begin callbacks

// gets called when each conversion is made, front axle
static void bridgeFrontAdcCallback(ADCDriver *adcp, adcsample_t *buffer, size_t n)
{
    (void) buffer; (void) n; // squelsh compiler

    if (adcp->state == ADC_COMPLETE) {
        // calculate currents
        uint16_t left_ma, right_ma;
        bridgeCurrents(&left_ma, &right_ma);

        // go around the constness, we don't want any another code to write to this object
        sen_motor_currents_t *m;
        m = (sen_motor_currents_t*)&sen_motorCurrents;

        // enter exclusion zone to update variables
        chSysLockFromISR();
        m->leftFront  = left_ma;
        m->rightFront = right_ma;
        if (m->maxLeftFront < left_ma)
            m->maxLeftFront = left_ma;
        if (m->maxRightFront < right_ma)
            m->maxRightFront = right_ma;
        chSysUnlockFromISR();

        // notify our subscribers
        chEvtBroadcastFlagsI(&sen_measuredEvts, EVENT_FLAG_ADC_FRONTAXLE);
    }
}

// gets called when each conversion is made, rear axle
static void bridgeRearAdcCallback(ADCDriver *adcp, adcsample_t *buffer, size_t n)
{
    (void) buffer; (void) n; // squelsh compiler

    if (adcp->state == ADC_COMPLETE) {
        // calculate currents
        uint16_t left_ma, right_ma;
        bridgeCurrents(&left_ma, &right_ma);

        // go around the constness, we don't want any another code to write to this object
        sen_motor_currents_t *m;
        m = (sen_motor_currents_t*)&sen_motorCurrents;

        // enter exclusion zone to update variables
        chSysLockFromISR();
        m->leftRear  = left_ma;
        m->rightRear = right_ma;
        if (m->maxLeftRear < left_ma)
            m->maxLeftRear = left_ma;
        if (m->maxRightRear < right_ma)
            m->maxRightRear = right_ma;
        chSysUnlockFromISR();

        // notify our subscribers
        chEvtBroadcastFlagsI(&sen_measuredEvts, EVENT_FLAG_ADC_REARAXLE);
    }
}

// checks for voltages and chip temp
static void backgroundAdcCallback(ADCDriver *adcp, adcsample_t *buffer, size_t n)
{
    (void)buffer; (void)n;
    if (adcp->state == ADC_COMPLETE) {
        adcsample_t avgBatVolt, avgIgnVolt, tempVolt;
        avgBatVolt = (backgroundAdcBuf[0] + backgroundAdcBuf[3] + backgroundAdcBuf[6] +
                      backgroundAdcBuf[9] + backgroundAdcBuf[12] + backgroundAdcBuf[15]) / 6;
        avgIgnVolt = (backgroundAdcBuf[1] + backgroundAdcBuf[4] + backgroundAdcBuf[7] +
                      backgroundAdcBuf[10] + backgroundAdcBuf[13] + backgroundAdcBuf[16]) / 6;
        tempVolt = (backgroundAdcBuf[2] + backgroundAdcBuf[5] + backgroundAdcBuf[8] +
                    backgroundAdcBuf[11] + backgroundAdcBuf[14] + backgroundAdcBuf[17]) / 6;

        // convert to real voltages
        // 1V in = 10V on pwr line so 3,3V = 33 real volts
        // so 4095 steps * 8 = 32760mv, so factor 8 is close enough here too
        // that way we only need integer maths
        uint16_t batMilliVolt, ignMilliVolt;
        float tempF;
        batMilliVolt = avgBatVolt * 8;
        ignMilliVolt = avgIgnVolt * 8; // 12V (1489steps * 8) -> 11912mv
        /*
         * From programming manual page 207
         * Temperature (in °C) = {(V25 - VSENSE) / Avg_Slope} + 25.
         * Where,
         * V25 = VSENSE value for 25° C and
         * Avg_Slope = Average Slope for curve between Temperature vs. VSENSE (given in
         * mV/° C or µV/ °C).
         *
         * Page 105 chip datasheet:
         * V25 = 1.43V, AVG_SLOPE = 4.3mV / C
         *      1775steps = 1.43v
         */
        tempF = tempVolt - 1775;
        tempF /= 4.3;
        tempF += 25;
        int8_t temp = (uint8_t)tempF;

        // un-const
        sen_voltages_t *v = (sen_voltages_t*)&sen_voltages;
        uint8_t *t = (uint8_t*)&sen_chipTemperature;
        chSysLockFromISR();
        v->batVolt = batMilliVolt;
        v->ignVolt = ignMilliVolt;
        *t = temp;
        chSysUnlockFromISR();


        // notify our subscribers
        chEvtBroadcastFlagsI(&sen_measuredEvts, EVENT_FLAG_ADC_BACKGROUND);
    }
}


// periodic heartbeat of background ADC conversions, (re-)called by VT timer
static void backgroundTimerCallback(void)
{
    chSysLockFromISR();
    // restart the background ADC (voltage and temp) each 250ms
    systime_t nextStop;
    if (ADCD1.state != ADC_ACTIVE) {
        // ADC busy, retry in 250 us
        nextStop = US2ST(250);
    } else {
        adcStartConversionI(&ADCD1, &adcBackgroundCfg, backgroundAdcBuf,
                            ADC_BACKGROUND_BUF_DEPTH);
        nextStop = MS2ST(BACKGROUND_TIMER_MS);
    }
    chVTSetI(&backgroundTimer, nextStop, (void*)backgroundTimerCallback, NULL);
    chSysUnlockFromISR();
}

// external pin change status interupts
static void extCallback(EXTDriver *extp, expchannel_t channel)
{
    (void)extp; (void)channel;

    switch (channel) {
    case 1: // PB1 -> LeftFront_DIAG
        chEvtBroadcastFlagsI(&sen_measuredEvts, EVENT_FLAG_BRIDGE_LF_DIAG);
        break;
    case 2: // PB2 -> RightFront_DIAG
        chEvtBroadcastFlagsI(&sen_measuredEvts, EVENT_FLAG_BRIDGE_RF_DIAG);
        break;
    case 6: // PF6 -> TIGHTEN_CMD_SIG
        chEvtBroadcastFlagsI(&sen_measuredEvts, EVENT_FLAG_TIGHTEN_CMD_SIG);
        break;
    case 7: // PF7 -> RELEASE_CMD_SIG
        chEvtBroadcastFlagsI(&sen_measuredEvts, EVENT_FLAG_RELEASE_CMD_SIG);
        break;
    case 8: // PA8 -> LeftRear_DIAG
        chEvtBroadcastFlagsI(&sen_measuredEvts, EVENT_FLAG_BRIDGE_LR_DIAG);
        break;
    case 9: // PA9 -> RightRear_DIAG
        chEvtBroadcastFlagsI(&sen_measuredEvts, EVENT_FLAG_BRIDGE_RR_DIAG);
        break;
    case 10: // PA10 -> PWR_ENABLED_SIG
        chEvtBroadcastFlagsI(&sen_measuredEvts, EVENT_FLAG_PWR_ENABLED_SIG);
        break;
    default:
        break; // not handled?
    }
}



// -------------------------------------------------------------------
// begin private functions to this file

static void bridgeCurrents(uint16_t *left_ma, uint16_t *right_ma)
{
    adcsample_t avgLeft, avgRight;
    // calculate average of these samples
    avgLeft = (bridgeAdcBuf[0] + bridgeAdcBuf[2] + bridgeAdcBuf[4] + bridgeAdcBuf[6]) / 4;
    avgRight = (bridgeAdcBuf[1] + bridgeAdcBuf[3] + bridgeAdcBuf[5] + bridgeAdcBuf[7]) / 4;

    // recalculate to Amperes (1A=0.1V)
    // 3.3V / 12bits = 0,000805664 volt / step
    // so 32760ma at 4095 gives us a factor of 8
    // close enough..
    *left_ma = avgLeft / 8;
    *right_ma = avgRight / 8;
}


static void clearFrontCurrents(void)
{
    sen_motor_currents_t *m;
    m = (sen_motor_currents_t *)&sen_motorCurrents;

    chSysLock();
    m->leftFront = 0;
    m->rightFront = 0;
    m->maxLeftFront = 0;
    m->maxRightFront = 0;
    chSysUnlock();
}

static void clearRearCurrents(void)
{
    sen_motor_currents_t *m;
    m = (sen_motor_currents_t *)&sen_motorCurrents;

    chSysLock();
    m->leftRear = 0;
    m->rightRear = 0;
    m->maxLeftRear = 0;
    m->maxRightRear = 0;
    chSysUnlock();
}

static void calculateWheelCircumference(void)
{
    uint32_t dia, profile;
    float circum;
    // go from inches to mm
    dia = (settings[S_RimDiameter] * 254) / 10;
    // get the profile height in mm
    profile = (settings[S_TireThread] * settings[S_TireProfile]) / 100;
    // add so both upper and lower part of tire height is added
    dia += profile * 2;

    // the circumference
    circum = dia * M_PI;
    wheelCircumference = (uint16_t)circum;
}

// ---------------------------------------------------------------------
// start thread

static THD_WORKING_AREA(waAdcHandlerThd, 128);
static THD_FUNCTION(adcHandlerThd, arg)
{
    (void) arg;
    chRegSetThreadName("sensor_adcHandlerThd");

    static event_listener_t evtMsg;
    static const uint8_t ADC_EVT = 0,
                         MSG_EVT = 1;

    // listen to ADC events for currents
    event_listener_t evtAdc;
    chEvtRegisterMaskWithFlags(&sen_measuredEvts, &evtAdc, EVENT_MASK(ADC_EVT),
                  EVENT_FLAG_ADC_REARAXLE | EVENT_FLAG_ADC_FRONTAXLE);
    // listen to msgs to this thread to change action
    chEvtRegisterMask(&sen_MsgHandlerThd, &evtMsg, EVENT_MASK(MSG_EVT));

    msg_t msg = 0;
    eventmask_t evt;
    sen_measure action = StopAll;
    systime_t timeout = TIME_INFINITE;

    while (TRUE) {
        // wait for a event to occur, either a new action or ADC finished
        // or don't wait at all if a new action needs to be done
        evt = chEvtWaitAnyTimeout(EVENT_MASK(ADC_EVT) | EVENT_MASK(MSG_EVT), timeout);
        if ((evt | EVENT_MASK(ADC_EVT)) || evt == 0){
            // we get here each time ADC has finished OR when a new action needs to be done (timeout==0)
            switch (action) {
            case StopAll:
                // no current to measure, restart background timer
                chVTSet(&backgroundTimer, MS2ST(BACKGROUND_TIMER_MS),
                        (void*)backgroundTimerCallback, NULL);
                break;
            case StartFront:
                adcStartConversion(&ADCD1, &adcBridgeFrontCfg, bridgeAdcBuf, ADC_BRIDGE_BUF_DEPTH);
                break;
            case StartRear:
                adcStartConversion(&ADCD1, &adcBridgeRearCfg, bridgeAdcBuf, ADC_BRIDGE_BUF_DEPTH);
                break;
            case StartAll: {
                // rationale here is to alternate between each axle
                eventflags_t wasFlg = chEvtGetAndClearFlags(&evtAdc);
                if (wasFlg & EVENT_FLAG_ADC_REARAXLE)
                    adcStartConversion(&ADCD1, &adcBridgeFrontCfg, bridgeAdcBuf, ADC_BRIDGE_BUF_DEPTH);
                else if (wasFlg & EVENT_FLAG_ADC_FRONTAXLE)
                    adcStartConversion(&ADCD1, &adcBridgeRearCfg, bridgeAdcBuf, ADC_BRIDGE_BUF_DEPTH);
                // else it was a background adc finished in which case we do nothing
            }   break;
            default:
                break; // do nothing as that stops the ADC measuring loop
            }

        } else if (evt | EVENT_MASK(MSG_EVT)) {
            sen_measure newAction = (sen_measure)msg;
            if (action == StartFront && newAction == StartRear) {
                clearRearCurrents();
                action = StartAll;
            } else if (action == StartRear && newAction == StartFront) {
                clearFrontCurrents();
                action = StartAll;
            } else if (action == StartAll && newAction == StopRear) {
                action = StartFront;
            } else if (action == StartAll && newAction == StopFront) {
                action = StartRear;
            } else if (newAction == StartAll) {
                clearRearCurrents();
                clearFrontCurrents();
                action = StartAll;
            } else {
                action = newAction;
            }

            // stop background timer if set
            if (action != StopAll && chVTIsArmed(&backgroundTimer))
                chVTReset(&backgroundTimer);

            // don't wait for a event to occur on next loop
            timeout = TIME_IMMEDIATE;
            continue;
        }

        // restore so we wait for next event to occur
        timeout = TIME_INFINITE;

    } // thread loop
}


static THD_WORKING_AREA(waInputPoll, 128);
static THD_FUNCTION(inputPoll, arg)
{
    (void)arg;
    chRegSetThreadName("sensor_inputPoll");

    // poll these inputs (can't use interrupts due to clashes on the same interrupt channel)
    // PD2 -> IGN_ON_SIG
    // PD8 -> LIGHTS_ON_SIG
    // PE8 -> BUTTON_SIG
    // PE9 -> BUTTON_INV_SIG

    uint16_t debounceIgn = 0xFFFF,
             debounceLights = 0xFFFF,
             debounceBtn = 0xFFFF,
             debounceBtnInv = 0xFFFF,
             offBounceIgn = 0,
             offBounceLights = 0,
             offBounceBtn = 0,
             offBounceBtnInv = 0;

    while (TRUE) {
        // when inactive a stream of 1 circulate, on clears bit0 and it propagates eventually
        // to the 4 MSB bits, at that point, an event is triggered
        debounceIgn    = (debounceIgn << 1)    | !SEN_IGN_ON_SIG     | 0xE000;
        debounceLights = (debounceLights << 1) | !SEN_LIGHTS_ON_SIG  | 0xE000;
        debounceBtn    = (debounceBtn << 1)    | !SEN_BUTTON_SIG     | 0xE000;
        debounceBtnInv = (debounceBtnInv << 1) | !SEN_BUTTON_INV_SIG | 0xE000;
        if (debounceIgn == 0xF000)
            chEvtBroadcastFlags(&sen_measuredEvts, EVENT_FLAG_IGN_ON_SIG);
        if (debounceLights == 0xF000)
            chEvtBroadcastFlags(&sen_measuredEvts, EVENT_FLAG_LIGHTS_ON_SIG);
        if (debounceBtn == 0xF000)
            chEvtBroadcastFlags(&sen_measuredEvts, EVENT_FLAG_BUTTON_SIG);
        if (debounceBtnInv == 0xF000)
            chEvtBroadcastFlags(&sen_measuredEvts, EVENT_FLAG_BUTTON_INV_SIG);

        // same algorithm as above but tiggers on falling edge
        offBounceIgn    = (offBounceIgn << 1)    | SEN_IGN_ON_SIG      | 0x1FFF;
        offBounceLights = (offBounceLights << 1) | SEN_LIGHTS_ON_SIG   | 0x1FFF;
        offBounceBtn    = (offBounceBtn << 1)    | SEN_BUTTON_SIG      | 0x1FFF;
        offBounceBtnInv = (offBounceBtnInv << 1) | SEN_BUTTON_INV_SIG  | 0x1FFF;
        if (offBounceIgn == 0xF000)
            chEvtBroadcastFlags(&sen_measuredEvts, EVENT_FLAG_IGN_ON_SIG);
        if (offBounceLights == 0xF000)
            chEvtBroadcastFlags(&sen_measuredEvts, EVENT_FLAG_LIGHTS_ON_SIG);
        if (offBounceBtn)
            chEvtBroadcastFlags(&sen_measuredEvts, EVENT_FLAG_BUTTON_SIG);
        if (offBounceBtnInv == 0xF000)
            chEvtBroadcastFlags(&sen_measuredEvts, EVENT_FLAG_BUTTON_INV_SIG);

        chThdSleep(MS2ST(7));
    }
}

static THD_WORKING_AREA(waSettingsHandler, 128);
static THD_FUNCTION(settingsHandler, arg)
{
    (void)arg;
    chRegSetThreadName("sensor_settingsHandler");

    // initial statup calculate
    calculateWheelCircumference();

    event_listener_t evtListener;
    eventflags_t flags = S_RimDiameter | S_TireThread | S_TireProfile;

    chEvtRegisterMaskWithFlags(&ee_settingsChanged, &evtListener, EVENT_MASK(0), flags);

    while (TRUE) {
        settings_e setting = (settings_e)chEvtWaitOne(flags);
        switch (setting) {
        case S_RimDiameter: case S_TireThread: case S_TireProfile:
            calculateWheelCircumference();
            break;
        default:
            continue;
        }
    }
}

// -------------------------------------------------------------------
// begin API functions

// call to init adc
void sen_initSensors(void)
{
    clearFrontCurrents();
    clearRearCurrents();
    adcStart(&ADCD1, NULL); // warmup ADC...
    adcSTM32EnableTSVREFE(); // enable chip temp. measurements

    // start a background loop to read voltages, temp.
    chVTSet(&backgroundTimer, MS2ST(BACKGROUND_TIMER_MS),
             (void*)backgroundTimerCallback, NULL);

    // start external interrupts (from digital pins)
    extStart(&EXTD1, &extConfig);

    // initialize our event source
    chEvtObjectInit(&sen_measuredEvts);
    chEvtObjectInit(&sen_MsgHandlerThd);

    // initialize and start handler thread
    adcHandlerp = chThdCreateStatic(&waAdcHandlerThd, 128, NORMALPRIO+10, adcHandlerThd, NULL);
    chThdStart(adcHandlerp);

    // initialize and start input polling thread
    pollInputsp = chThdCreateStatic(&waInputPoll, 128, NORMALPRIO-5, inputPoll, NULL);

    // initialize factors from settings
    settingsHandlerp = chThdCreateStatic(&waSettingsHandler, 128, LOWPRIO,
                                         settingsHandler, NULL);
    chThdStart(settingsHandlerp);
}

// diagnose wheelsensor circuits
int sen_diagWheelSensors(void)
{
    // don't check when we have speed
    if (sen_vehicleSpeed())
        return 0;

    int checkedCnt = 0;
    uint8_t bit;

    // first set pins as normal inputs
    uint32_t oldMode = GPIOA->MODER; // so we can restore later

    palSetPadMode(GPIOA, GPIOA_LR_speed_in, PAL_MODE_INPUT);
    palSetPadMode(GPIOA, GPIOA_RR_speed_in, PAL_MODE_INPUT);
    palSetPadMode(GPIOA, GPIOA_LF_speed_in, PAL_MODE_INPUT);
    palSetPadMode(GPIOA, GPIOA_RF_speed_in, PAL_MODE_INPUT);

    for (bit = GPIOA_LR_speed_in; bit <= GPIOA_RF_speed_in; ++bit) {
        bool state = palReadPad(GPIOA, PAL_PORT_BIT(bit));
        palSetPad(GPIOC, GPIOC_VR_diag_test);
        chThdSleepMilliseconds(1); // let diagtest propagate in real life system
        if (state != palReadPad(GPIOA, PAL_PORT_BIT(bit))) {
#ifdef DEBUG_MODE
            char buf[30];
            char *name;
            if (bit == GPIOA_LR_speed_in)
                name = "RightFront";
            else if (bit == GPIOA_RR_speed_in)
                name = "RightRear";
            else if (bit == GPIOA_LR_speed_in)
                name = "LeftRear";
            else
                name = "LeftFront";
            chsnprintf(buf, 29, "open circuit at %s sensor", name);
            DEBUG_OUT(buf);
#endif
            // code for setting DTC open circuit here
            switch (bit) {
            case GPIOA_LF_speed_in:
                chMBPost(&dtc_MB, (msg_t)C_dtc_LF_wheelsensor, TIME_IMMEDIATE);
                break;
            case GPIOA_RF_speed_in:
                chMBPost(&dtc_MB, (msg_t)C_dtc_RF_wheelsensor, TIME_IMMEDIATE);
                break;
            case GPIOA_LR_speed_in:
                chMBPost(&dtc_MB, (msg_t)C_dtc_LR_wheelsensor, TIME_IMMEDIATE);
                break;
            case GPIOA_RR_speed_in:
                chMBPost(&dtc_MB, (msg_t)C_dtc_RR_wheelsensor, TIME_IMMEDIATE);
                break;
            default: break;
            }
        } else {
            ++checkedCnt; // sensor ok
        }
        palClearPad(GPIOC, GPIOC_VR_diag_test);
        chThdSleepMilliseconds(1); // let diagtest propagate in real life system
    }

    // restore pinMode status
    GPIOA->MODER = oldMode;

    return checkedCnt;
}

// get the average wheel revolution, discards wheel if it is 20% slower
uint8_t sen_wheelAverage(void)
{

    // get the fastest spinning wheel, and take out 20%
    uint32_t rev, check;
    rev = MAX(sen_wheelSpeeds.rightFront_rps, sen_wheelSpeeds.leftRear_rps);
    rev = MAX(rev, sen_wheelSpeeds.leftFront_rps);
    check = (uint32_t)(MAX(rev, sen_wheelSpeeds.rightRear_rps) * 0.8); // 20% slip is allowed

    rev = 0;
    int i, cnt = 0;
    uint8_t *rps = (uint8_t*)&sen_wheelSpeeds.leftFront_rps;
    for (i = LeftFront; i <= RightRear; ++i) {
        // only include wheels that has less than 20% slip in relation to other wheels
        if (rps[i] >= check) {
            rev += rps[i];
            ++cnt;
        }
    }

    return cnt > 0 ? rev / cnt : 0;
}

// calculates vehicle speed, returns in kph
uint8_t sen_vehicleSpeed(void)
{
    // first find the fastest spinning wheel
    uint8_t rps = MAX(sen_wheelSpeeds.leftFront_rps, sen_wheelSpeeds.rightFront_rps);
    rps = MAX(rps, sen_wheelSpeeds.rightRear_rps);
    rps = MAX(rps, sen_wheelSpeeds.leftRear_rps);

    // first distance in 1s
    uint32_t speed = rps * wheelCircumference;
    // distance in 1h
    speed *= 3600;
    // mm/h -> km/h
    speed /= 1000000;

    return speed < 256 ? speed : 255;
}

