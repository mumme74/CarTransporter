/*
    This example shows how to use the IntervalTimer library and the ADC library in the tensy 3.0/3.1

    The three important objects are: the ADC, the (one or more) IntervalTimer and the same number of RingBuffer.

    The ADC sets up the internal ADC, you can change the settings as usual. Enable interrupts.
    The IntervalTimer (timerX) executes a function every 'period' microseconds.
    This function, timerX_callback, starts the desired ADC measurement, for example:
        startSingleRead, startSingleDifferential, startSynchronizedSingleRead, startSynchronizedSingleDifferential
    you can use the ADC0 or ADC1 (only for Teensy 3.1).

    When the measurement is done, the adc_isr is executed:
        - If you have more than one timer per ADC module you need to know which pin was measured.
        - Then you store/process the data
        - Finally, if you have more than one timer you can check whether the last measurement interruped a
          a previous one (using adc->adcX->adcWasInUse), and restart it if so.
          The settings of the interrupted measurement are stored in the adc->adcX->adc_config struct.


*/



#include "ADC.h"
// and IntervalTimer
#include <IntervalTimer.h>

const int ledPin = LED_BUILTIN;

const int readPin0 = A9;
const int period0 = 100; // us

const int readPin1 = A8;
const int period1 = 100; // us

const int readPeriod = 100000; // us

ADC *adc = new ADC(); // adc object

IntervalTimer timer0, timer1; // timers

RingBuffer *buffer0 = new RingBuffer; // buffers to store the values
RingBuffer *buffer1 = new RingBuffer;

int startTimerValue0 = 0, startTimerValue1 = 0;

void setup() {

    pinMode(ledPin, OUTPUT); // led blinks every loop
    pinMode(ledPin+1, OUTPUT); // timer0 starts a measurement
    pinMode(ledPin+2, OUTPUT); // timer1 starts a measurement
    pinMode(ledPin+3, OUTPUT); // adc0_isr, measurement finished
    pinMode(readPin0, INPUT); pinMode(readPin1, INPUT); //pin 23 single ended

    Serial.begin(9600);

    delay(1000);

    ///// ADC0 ////
    //adc->setReference(ADC_REF_INTERNAL, ADC_0); change all 3.3 to 1.2 if you change the reference

    adc->setAveraging(16); // set number of averages
    adc->setResolution(12); // set bits of resolution

    // it can be ADC_VERY_LOW_SPEED, ADC_LOW_SPEED, ADC_MED_SPEED, ADC_HIGH_SPEED_16BITS, ADC_HIGH_SPEED or ADC_VERY_HIGH_SPEED
    // see the documentation for more information
    adc->setConversionSpeed(ADC_HIGH_SPEED); // change the conversion speed
    // it can be ADC_VERY_LOW_SPEED, ADC_LOW_SPEED, ADC_MED_SPEED, ADC_HIGH_SPEED or ADC_VERY_HIGH_SPEED
    adc->setSamplingSpeed(ADC_HIGH_SPEED); // change the sampling speed
    // with 16 averages, 12 bits resolution and ADC_HIGH_SPEED conversion and sampling it takes about 32.5 us for a conversion

    Serial.println("Starting Timers");

    // start the timers, if it's not possible, startTimerValuex will be false
    startTimerValue0 = timer0.begin(timer0_callback, period0);
    //delayMicroseconds(period0/2); // wait enough time for the first timer conversion to finish (depends on resolution and averaging)
    delayMicroseconds(20); // with the current settings, waiting so little time means the timer1 will interrupt the conversion
    // initiated by timer0. The adc_isr will restart the timer0's measurement.
    // You can check with an oscilloscope:
    // Pin 14 corresponds to the timer0 initiating a measurement
    // Pin 15 the same for the timer1
    // Pin 16 is the adc_isr.
    // Timer0 starts a comversion and 20 us later timer1 starts a new one, "pausing" the first, about 32 us later timer1's conversion
    // is done, and timer0's is restarted, 32 us later timer0's conversion finishes. About 5 us later timer0 starts a new conversion again.
    // (times don't add up to 100 us because the timer_callbacks and adc_isr take time to execute)
    // so in the worst case timer0 gets a new value in about twice as long as it would take alone.
    startTimerValue1 = timer1.begin(timer1_callback, period1);

    adc->enableInterrupts(ADC_0);

    Serial.println("Timers started");

    delay(500);
}

int value = 0;
char c=0;

void loop() {

    if(startTimerValue0==false) {
            Serial.println("Timer0 setup failed");
    }
    if(startTimerValue1==false) {
            Serial.println("Timer1 setup failed");
    }

    if(!buffer0->isEmpty()) { // read the values in the buffer
        Serial.print("Read pin 0: ");
        Serial.println(buffer0->read()*3.3/adc->getMaxValue());
        //Serial.println("New value!");
    }
    if(!buffer1->isEmpty()) { // read the values in the buffer
        Serial.print("Read pin 1: ");
        Serial.println(buffer1->read()*3.3/adc->getMaxValue());
        //Serial.println("New value!");
    }

    if (Serial.available()) {
        c = Serial.read();
        if(c=='s') { // stop timer
            Serial.println("Stop timer");
            timer0.end();
        } else if(c=='r') { // restart timer
            Serial.println("Restart timer");
            startTimerValue0 = timer0.begin(timer0_callback, period0);
        }
    }

    digitalWriteFast(LED_BUILTIN, !digitalReadFast(LED_BUILTIN) );

  delayMicroseconds(readPeriod);
}

// This function will be called with the desired frequency
// start the measurement
// in my low-res oscilloscope this function seems to take 1.5-2 us.
void timer0_callback(void) {

    digitalWriteFast(ledPin+1, HIGH);

    adc->startSingleRead(readPin0, ADC_0); // also: startSingleDifferential, analogSynchronizedRead, analogSynchronizedReadDifferential

    digitalWriteFast(ledPin+1, LOW);
    //digitalWriteFast(ledPin+1, !digitalReadFast(ledPin+1));

}

// This function will be called with the desired frequency
// start the measurement
void timer1_callback(void) {

    digitalWriteFast(ledPin+2, HIGH);

    adc->startSingleRead(readPin1, ADC_0);

    digitalWriteFast(ledPin+2, LOW);

}

// when the measurement finishes, this will be called
// first: see which pin finished and then save the measurement into the correct buffer
void adc0_isr() {

    digitalWriteFast(ledPin+3, HIGH);

    uint8_t pin = ADC::sc1a2channelADC0[ADC0_SC1A&ADC_SC1A_CHANNELS]; // the bits 0-4 of ADC0_SC1A have the channel

    // add value to correct buffer
    if(pin==readPin0) {
        buffer0->write(adc->readSingle());
    } else if(pin==readPin1) {
        buffer1->write(adc->readSingle());
    } else { // clear interrupt anyway
        ADC0_RA;
    }

    // restore ADC config if it was in use before being interrupted by the analog timer
    if (adc->adc0->adcWasInUse) {
        // restore ADC config, and restart conversion
        ADC0_CFG1 = adc->adc0->adc_config.savedCFG1;
        ADC0_CFG2 = adc->adc0->adc_config.savedCFG2;
        ADC0_SC2 = adc->adc0->adc_config.savedSC2;
        ADC0_SC3 = adc->adc0->adc_config.savedSC3;
        ADC0_SC1A = adc->adc0->adc_config.savedSC1A;
    }


    digitalWriteFast(ledPin+3, LOW);
    //digitalWriteFast(ledPin+2, !digitalReadFast(ledPin+2));

}
