// see
// https://github.com/pedvide/ADC/blob/
// cdfeb96d5a1135007d2a292bca3c01a6396bdffa/examples/
// analogReadIntervalTimer/analogReadIntervalTimer.ino
// for comments

#include

#define    MPU9250_ADDRESS            0x68
#define    MAG_ADDRESS                0x0C

#define    GYRO_FULL_SCALE_250_DPS    0x00
#define    GYRO_FULL_SCALE_500_DPS    0x08
#define    GYRO_FULL_SCALE_1000_DPS   0x10
#define    GYRO_FULL_SCALE_2000_DPS   0x18

#define    ACC_FULL_SCALE_2_G        0x00
#define    ACC_FULL_SCALE_4_G        0x08
#define    ACC_FULL_SCALE_8_G        0x10
#define    ACC_FULL_SCALE_16_G       0x18

#include "ADC.h"
#include "RingBuffer.h"
#include <IntervalTimer.h>

const int ledPin = LED_BUILTIN;
const int readPin0 = A10;
const int period0 = 120; // us
const int readPin1 = A11;
const int period1 = 120; // us
const int readPeriod = 1000; // 1khz sampling rate

ADC *adc = new ADC(); // adc object

IntervalTimer timer0, timer1; // timers

RingBuffer *buffer0 = new RingBuffer; // buffers to store the values
RingBuffer *buffer1 = new RingBuffer;

int startTimerValue0 = 0, startTimerValue1 = 0;

void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data)
{
 // Set register address
 Wire.beginTransmission(Address);
 Wire.write(Register);
 Wire.endTransmission();

 // Read Nbytes
 Wire.requestFrom(Address, Nbytes);
 uint8_t index=0;
 while (Wire.available())
   Data[index++]=Wire.read();
}
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data)
{
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.write(Data);
  Wire.endTransmission();
}

void setup() {

    pinMode(ledPin, OUTPUT); // led blinks every loop
    pinMode(ledPin+1, OUTPUT); // timer0 starts a measurement
    pinMode(ledPin+2, OUTPUT); // timer1 starts a measurement
    pinMode(ledPin+3, OUTPUT); // adc0_isr, measurement finished for readPin0
    pinMode(ledPin+4, OUTPUT); // adc0_isr, measurement finished for readPin1
    pinMode(readPin0, INPUT); pinMode(readPin1, INPUT);

    Wire.begin();
    Serial.begin(9600);
    delay(1000);

    adc->setAveraging(16);
    adc->setResolution(12);

    adc->setConversionSpeed(ADC_MED_SPEED);
    adc->setSamplingSpeed(ADC_MED_SPEED);

    Serial.println("Starting Timers");

    startTimerValue0 = timer0.begin(timer0_callback, period0);
    delayMicroseconds(25);
    startTimerValue1 = timer1.begin(timer1_callback, period1);

    adc->enableInterrupts(ADC_0);
    Serial.println("Timers started");

    I2CwriteByte(MPU9250_ADDRESS,27,GYRO_FULL_SCALE_2000_DPS);
    // Configure accelerometers range
    I2CwriteByte(MPU9250_ADDRESS,28,ACC_FULL_SCALE_16_G);
    // Set by pass mode for the magnetometers
    I2CwriteByte(MPU9250_ADDRESS,0x37,0x02);

    // Request first magnetometer single measurement
    I2CwriteByte(MAG_ADDRESS,0x0A,0x01);
    Serial.println("I2C started");

    delay(500);
}

int value = 0;
char c=0;
long int cpt=0;

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
    }
    if(!buffer1->isEmpty()) { // read the values in the buffer
        Serial.print("Read pin 1: ");
        Serial.println(buffer1->read()*3.3/adc->getMaxValue());
    }

    uint8_t Buf[14];
    I2Cread(MPU9250_ADDRESS,0x3B,14,Buf);
    // Accelerometer
    int16_t ax=-(Buf[0]<<8 | Buf[1]);
    int16_t ay=-(Buf[2]<<8 | Buf[3]);
    int16_t az=Buf[4]<<8 | Buf[5];
    Serial.print("X acc: ");
    Serial.println(ax);
    Serial.print("Y acc: ");
    Serial.print(ay);
    Serial.print("Z acc: ");
    Serial.print(az);

    delayMicroseconds(readPeriod);
}

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
        // restore ADC config, and restart conversion
        adc->adc0->loadConfig(&adc->adc0->adc_config);
        // avoid a conversion started by this isr to repeat itself
        adc->adc0->adcWasInUse = false;
    }
}
