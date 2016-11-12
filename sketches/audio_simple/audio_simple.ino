#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SerialFlash.h>

#include "MySound1.h"
#include "MySound2.h"

#define Pin1 23
#define Pin2 24

AudioPlayMemory sound0;
AudioPlayMemory sound1;

elapsedMicros current_time;

// play to both I2S audio board and on-chip DAC
AudioOutputAnalog dac; 

AudioConnection c1(sound0, 0, mix1, 0); 
AudioConnection c2(sound1, 0, mix1, 1);

// Create an object to control the audio shield.
// 
AudioControlSGTL5000 audioShield;

void setup() {

  AudioMemory(10);

  // turn on the output
  audioShield.enable();
  audioShield.volume(0.5);

  mix1.gain(0, 0.4);
  mix1.gain(1, 0.4);

  pinMode(Pin1, INPUT);
  pinMode(Pin2, INPUT);
  attachInterrupt(Pin1, callback0, RISING);
  attachInterrupt(Pin2, callback1, RISING);
}

void callback0() {
    print(current_time);
    sound0.play(AudioSample1);
    println("Sound 1");
}
void callback1() {
    print(current_time);
    sound1.play(AudioSample2);
    println("Sound 2");
}

void loop() {}
