/*
This is a (very) minimal example to test the serial port reading.
It'll blink every ~100 ms and report both the time in milliseconds and a single sensor value.
Modified from https://www.arduino.cc/en/Tutorial/KnockSensor
*/

const int ledPin = LED_BUILTIN;   
const int sensor= A10; // read from A10
const long refTime = millis();

// these variables will change:
int ledState = LOW;         // variable used to store the last LED status, to toggle the light

void setup() {
    pinMode(ledPin, OUTPUT); // declare the ledPin as an output
    Serial.begin(9600);       // use the serial port
}

void loop() {
    Serial.print(millis() - refTime);
    Serial.print('\t');
    Serial.print(analogRead(sensor));
    Serial.println('\n');
    ledState = !ledState; // toggle led every loop
    delay(100);  // delay to avoid overloading the serial port buffer
}