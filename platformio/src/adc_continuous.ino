#include "ADC.h"
#include "IntervalTimer.h"

const unsigned int channel_array[6] = {A0, A1, A2, A3, A4, A7};
const unsigned long period_0 = 1000;
const unsigned int array_size = sizeof(channel_array)/sizeof(int);
elapsedMicros current_time;
unsigned int ii = 0;

volatile unsigned int value_array[array_size];
volatile boolean delta = false;
volatile unsigned long current_time_copy = 0;
volatile unsigned long end_time = 0;

IntervalTimer timer_0;

ADC *adc = new ADC();

FASTRUN void adcCallback(void) {
    current_time_copy = current_time;
    for (ii = 0; ii < array_size; ii++) {
      value_array[ii] = adc->analogRead(channel_array[ii]);
    }
    end_time = current_time;
    delta = true;
}

void setup() {
    for(ii = 0; ii < array_size; ii++) {
        pinMode(channel_array[ii], INPUT);
    }

    adc->setReference(ADC_REF_3V3, ADC_0);
    adc->setAveraging(16);
    adc->setResolution(12);
    adc->setConversionSpeed(ADC_HIGH_SPEED);
    adc->setSamplingSpeed(ADC_HIGH_SPEED);
    timer_0.priority(10);
    timer_0.begin(adcCallback, period_0);

    delay(500);
    Serial.begin(9600);
    delay(1000);
}

void loop() {
    if (delta) {
        noInterrupts();
        delta = false;
        Serial.print(current_time_copy);
        for (ii = 0; ii < array_size; ii++) {
            Serial.print(" ");
            Serial.print(value_array[ii]);
        }
        Serial.print(" ");
        Serial.print(end_time);
        Serial.print("\n");
        interrupts();
    }
}
