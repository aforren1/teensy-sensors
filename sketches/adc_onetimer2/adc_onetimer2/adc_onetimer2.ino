#include "ADC.h"
#include "IntervalTimer.h"

const int analog_0 = A0;
const int analog_1 = A1;
const int analog_2 = A2;
const int analog_3 = A3;
const int analog_4 = A4;
const int analog_5 = A5;

const int period_0 = 5000;

IntervalTimer timer_0;
int value_0, value_1, value_2, value_3, value_4, value_5;

ADC *adc = new ADC();

void setup() {
  pinMode(analog_0, INPUT);
  pinMode(analog_1, INPUT);
  pinMode(analog_2, INPUT);
  pinMode(analog_3, INPUT);  
  pinMode(analog_4, INPUT);
  pinMode(analog_5, INPUT);
  
  Serial.begin(9600);
  delay(1000);

  adc->setReference(ADC_REF_3V3, ADC_0);
  adc->setAveraging(8);
  adc->setResolution(12);
  adc->setConversionSpeed(ADC_HIGH_SPEED);
  adc->setSamplingSpeed(ADC_HIGH_SPEED);
  timer_0.priority(10);
  timer_0.begin(timerCallback, period_0);
  delay(500);
}

volatile bool go_flag = false;

void timerCallback(void) {
  go_flag = true;
}

void loop() {
  bool go_flag_copy = false;
  go_flag = false;
  value_0 = adc->analogRead(analog_0);
  value_1 = adc->analogRead(analog_1);
  value_2 = adc->analogRead(analog_2);
  value_3 = adc->analogRead(analog_3);
  value_4 = adc->analogRead(analog_4);
  value_5 = adc->analogRead(analog_5);
  
  Serial.print(millis()/1000.0, 3);
  Serial.print("\t");
  Serial.print(value_0*3.3/adc->getMaxValue(ADC_0), 4);
  Serial.print("\t");
  
  Serial.print(value_1*3.3/adc->getMaxValue(ADC_0), 4);
  Serial.print("\t");
  
  Serial.print(value_2*3.3/adc->getMaxValue(ADC_0), 4);
  Serial.print("\t");
  
  Serial.print(value_3*3.3/adc->getMaxValue(ADC_0), 4);
  Serial.print("\t");
  
  Serial.print(value_3*3.3/adc->getMaxValue(ADC_0), 4);
  Serial.print("\t");
  
  Serial.println(value_5*3.3/adc->getMaxValue(ADC_0), 4);
  
  while(!go_flag_copy) { 
    noInterrupts();
    go_flag_copy = go_flag;
    interrupts();
  }
}

