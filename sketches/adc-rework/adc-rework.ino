

/*
Now that I somewhat understand interrupts, here's a reworked example that
reads the two channels at proper intervals.

All of the "typical" pins we use feed into ADC_0, so we don't have to worry
about settings on ADC_1 yet...
*/

#include "ADC.h"
#include "RingBuffer.h"
#include <IntervalTimer.h>

const int ledPin = LED_BUILTIN;
const int readPin0 = A9;
const int period0 = 2000; // 500 hz sampling
const int readPin1 = A6;
const int period1 = 2000; // also 500 hz

int startTimerValue0 = 0, startTimerValue1 = 0;

elapsedMillis timestamp; //
IntervalTimer timer0, timer1;

ADC *adc = new ADC();
RingBuffer *buffer0 = new RingBuffer;
RingBuffer *buffer1 = new RingBuffer;

void setup() {

    // try to use digital pins for these functions to free up analog pins
    pinMode(ledPin, OUTPUT); // blink led on loop
    pinMode(0, OUTPUT); // timer0 starts measuring
    pinMode(1, OUTPUT); // timer1 starts measuring
    pinMode(2, OUTPUT); // adc0_isr, measurement finished for readPin0
    pinMode(3, OUTPUT); // adc0_isr, measurement finished for readPin1

    pinMode(readPin0, INPUT);
    pinMode(readPin1, INPUT);

    Serial.begin(9600);
    delay(1000);

    adc->setReference(ADC_REF_3V3, ADC_0);
    adc->setAveraging(20);
    adc->setResolution(16);
    adc->setConversionSpeed(ADC_MED_SPEED);

    startTimerValue0 = timer0.begin(timer0_callback, period0);
    delayMicroseconds(25);
    startTimerValue1 = timer1.begin(timer1_callback, period1);
    adc->enableInterrupts(ADC_0);

    delay(500);
}

// barebones reading
void loop() {
    if(startTimerValue0==false) {
            Serial.println("Timer0 setup failed");
    }
    if(startTimerValue1==false) {
            Serial.println("Timer1 setup failed");
    }
    Serial.println(timestamp);
    Serial.print("\t");
    if(!buffer0->isEmpty()) { // read the values in the buffer
        Serial.print(buffer0->read()*3.3/adc->getMaxValue());
        Serial.print("\t");
    }
    if(!buffer1->isEmpty()) { // read the values in the buffer
        //Serial.print("Read pin 1: ");
        Serial.print(buffer1->read()*3.3/adc->getMaxValue());
        Serial.print("\t");
    }
    delayMicroseconds(period0);
}

// additional functions
void timer0_callback(void) {
    digitalWriteFast(0, HIGH);
    adc->startSingleRead(readPin0, ADC_0);
    digitalWriteFast(0, LOW);
}

void timer1_callback(void) {
    digitalWriteFast(1, HIGH);
    adc->startSingleRead(readPin1, ADC_0);
    digitalWriteFast(1, LOW);
}

void adc0_isr() {
    uint8_t pin = ADC::sc1a2channelADC0[ADC0_SC1A&ADC_SC1A_CHANNELS];
    // add value to correct buffer
    if(pin==readPin0) {
        digitalWriteFast(2, HIGH);
        buffer0->write(adc->readSingle());
        digitalWriteFast(2, LOW);
    } else if(pin==readPin1) {
        digitalWriteFast(3, HIGH);
        buffer1->write(adc->readSingle());
        if(adc->adc0->isConverting()) {
            digitalWriteFast(LED_BUILTIN, 1);
        }
        digitalWriteFast(3, LOW);
    } else { // clear interrupt anyway
        adc->readSingle();
    }
    if (adc->adc0->adcWasInUse) {
        adc->adc0->loadConfig(&adc->adc0->adc_config);
        adc->adc0->adcWasInUse = false;
    }
}
