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

volatile boolean play0 = false;
volatile boolean play1 = false;

elapsedMicros current_time;

// play to both I2S audio board and on-chip DAC
AudioOutputAnalog dac; 

AudioConnection c1(sound0, 0, mix1, 0); 
AudioConnection c2(sound1, 0, mix1, 1);

// Create an object to control the audio shield.
// 
AudioControlSGTL5000 audioShield;

void callback0() {
    play0 = true;
}
void callback1() {
    play1 = true;
}

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

void loop() {
  noInterrupts();
  if (play0) {
    play0 = false;
    print("Played sound at: ");
    println(current_time);
    sound0.play(AudioSample1);
  }
  interrupts();

  noInterrupts();
  if (play1) {
    play1 = false;
    print("Played sound 2 at: ")
    println(current_time);
    sound1.play(AudioSample2);
  }
  interrupts();
}
