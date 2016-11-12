/* 
Digital interrupts example?
*/

#define PinInt 23
elapsedMicros current_time;

void setup() {
    Serial.begin(9600);
    pinMode(PinInt, INPUT);
    attachInterrupt(PinInt, interruptRoutine, RISING); // interrupt 1 is data ready
}

void loop() {}

void interruptRoutine()
{
    Serial.println(current_time);
} 