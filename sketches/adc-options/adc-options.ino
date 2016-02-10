/* 
This example is based off of the fantastic example at
https://github.com/pedvide/ADC/blob/master/examples/adc_pdb/adc_pdb.ino

This example allows us to programmatically set the sampling frequency, as well 
as pause the device. there *should* be a clean way to assign analog inputs via
the serial port as well...

I've set it to use the external ref for ADC conversion, so make sure 3v3 is 
plugged into AREF (the inner pin nearest to the USB)

pins 13-17 are reserved for ADC conversion (LED through A3), which leaves about
8(? I can't count) easily accessible analog pins.
*/
#include "ADC.h"
#include "RingBuffer.h"
#include <IntervalTimer.h>

const int ledPin = LED_BUILTIN;
const int readPin0 = A9;
const int period0 = 120; // microseconds
const int readPin1 = A6;
const int period1 = 120;
int readPeriod; // 1/readPeriod == sampling freq
char doSomething = 0; // get inputs from user (pause, etc.)

ADC *adc = new ADC();
IntervalTimer timer0, timer1;
RingBuffer *buffer0 = new RingBuffer;
RingBuffer *buffer1 = new RingBuffer;
int startTimerValue0 = 0, startTimerValue1 = 0;

void setup() {
    pinMode(ledPin, OUTPUT); // led blinks every loop
    pinMode(ledPin+1, OUTPUT); // timer0 starts a measurement
    pinMode(ledPin+2, OUTPUT); // timer1 starts a measurement
    pinMode(ledPin+3, OUTPUT); // adc0_isr, measurement finished for readPin0
    pinMode(ledPin+4, OUTPUT); // adc0_isr, measurement finished for readPin1

    pinMode(readPin0, INPUT); pinMode(readPin1, INPUT);
    Serial.begin(9600);
    delay(1000);

    adc->setReference(ADC_REF_EXT, ADC_1);
    adc->setAveraging(16);
    adc->setResolution(12);
    adc->setConversionSpeed(ADC_HIGH_SPEED_16BITS);
    adc->setSamplingSpeed(ADC_HIGH_SPEED_16BITS);

    Serial.println("What is the sampling period? 10000 == 100 Hz");
    while(Serial.available()==0) {}
    readPeriod = Serial.parseInt();
    Serial.println("On my mark... (type something).");
    while(Serial.available()==0) {}

    startTimerValue0 = timer0.begin(timer0_callback, period0);
    startTimerValue1 = timer1.begin(timer1_callback, period1);
    adc->enableInterrupts(ADC_0);

    delay(500);
}

void loop() {
    if(startTimerValue0==false) {
            Serial.println("Timer0 setup failed");
    }
    if(startTimerValue1==false) {
            Serial.println("Timer1 setup failed");
    }
    
    if(!buffer0->isEmpty()) { // read the values in the buffer
        Serial.println(buffer0->read()*3.3/adc->getMaxValue());
        Serial.print("\t");
    }
    if(!buffer1->isEmpty()) { // read the values in the buffer
        Serial.print(buffer1->read()*3.3/adc->getMaxValue());
        Serial.print("\t");
    }
    
    if (Serial.available()) {
        doSomething = Serial.read();
        if(doSomething=='x') { // pause
            Serial.println("Pausing...");
            while (Serial.available() == 0) {} // do nothing until new input
        }
    }
    delayMicroseconds(readPeriod);
} // end loop

//// Helpers
void timer0_callback(void) {
    digitalWriteFast(ledPin+1, HIGH);
    adc->startSingleRead(readPin0, ADC_0);
    digitalWriteFast(ledPin+1, LOW);
}

void timer1_callback(void) {
    digitalWriteFast(ledPin+2, HIGH);
    adc->startSingleRead(readPin1, ADC_0);
    digitalWriteFast(ledPin+2, LOW);
}

void adc0_isr() {
    uint8_t pin = ADC::sc1a2channelADC0[ADC0_SC1A&ADC_SC1A_CHANNELS];
    // add value to correct buffer
    if(pin==readPin0) {
        digitalWriteFast(ledPin+3, HIGH);
        buffer0->write(adc->readSingle());
        digitalWriteFast(ledPin+3, LOW);
    } else if(pin==readPin1) {
        digitalWriteFast(ledPin+4, HIGH);
        buffer1->write(adc->readSingle());
        if(adc->adc0->isConverting()) {
            digitalWriteFast(LED_BUILTIN, 1);
        }
        digitalWriteFast(ledPin+4, LOW);
    } else { // clear interrupt anyway
        adc->readSingle();
    }

    if (adc->adc0->adcWasInUse) {
        adc->adc0->loadConfig(&adc->adc0->adc_config);
        adc->adc0->adcWasInUse = false;
    }
}
