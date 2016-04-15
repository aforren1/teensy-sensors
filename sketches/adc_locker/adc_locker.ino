/* 
This is another thought wrt data acquisition, where only one of the 
IntervalTimers is used. This is much more flexible, but the timing is
somewhat less deterministic (ie. not everything is guaranteed to happen
on at the *exact* same time in the interval). However, it's less fussing
around with resuming previous conversions that were interrupted, and
probably easier for newer users to pick up.

This shouldn't need RingBuffers either.

This example will use the full 5 inputs; that's 3 for the accelerometer,
1 for the piezo disk, and 1 for the photosensor.
*/

#include "ADC.h"
#include <IntervalTimer.h>

const int led_pin = LED_BUILTIN; // blink after every loop around
const int analog_0 = A0;
const int analog_1 = A1;
const int analog_2 = A2;
const int analog_3 = A3;
const int analog_4 = A4;

const int period = 10000; // 100 hz? this is the period in microseconds
int start_timer_value = 0;
bool go_flag = false;
IntervalTimer timer0;
ADC *adc = new ADC();

void setup() {
    
    pinMode(led_pin, OUTPUT);
    pinMode(analog_0, OUTPUT);
    pinMode(analog_1, OUTPUT);
    pinMode(analog_2, OUTPUT);
    pinMode(analog_3, OUTPUT);
    pinMode(analog_4, OUTPUT);
    
    Serial.begin(9600);
    delay(1000); // This one is in ms (confusing!)
    
    adc->setReference(ADC_REF_3V3);
    adc->setAveraging(8);
    adc->setResolution(12);
    adc->setConversionSpeed(ADC_HIGH_SPEED);
    
    adc->startContinuous(analog_0, ADC_0);
    adc->startContinuous(analog_1, ADC_0);
    adc->startContinuous(analog_2, ADC_0);
    adc->startContinuous(analog_3, ADC_0);
    adc->startContinuous(analog_4, ADC_0);
  
    start_timer_value = timer0.begin(timer0_callback); //  
    delay(500);
}

void loop() {
    if(start_timer_value == false) {Serial.print("Timer setup failed");}
    
    while(!go_flag) { 
        // wait until go flag is reversed
    }
    
    // do all conversions after the go_flag is set
    Serial.print(analogHelper(analog_0), DEC);
    Serial.print(analogHelper(analog_0), DEC);
    Serial.print(analogHelper(analog_0), DEC);
    Serial.print(analogHelper(analog_0), DEC);
    Serial.print(analogHelper(analog_0), DEC);
    
    digitalWriteFast(led_pin, !digitalReadFast(led_pin));
    go_flag = !go_flag; // reset the flag
}

float analogHelper(int pin_num) {
    return adc->analogRead(pin_num)*3.3/adc->getMaxValue(ADC_0);
}
