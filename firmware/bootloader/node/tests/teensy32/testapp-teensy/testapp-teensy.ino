
#include <Arduino.h>

void startup_early_hook() {
    digitalWrite(8, !digitalRead(8));
}

void setup() {
    Serial.begin(115200);
    pinMode(9, OUTPUT);
}

void loop () {
    // toogle LED
    digitalWrite(9, !digitalRead(9));

    // wait 100ms
    delay(100);
    
    // output heartbeat
    Serial.print("Tick\r\n");
}
