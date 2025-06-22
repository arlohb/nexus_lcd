#include "Program.h"

#include <Arduino.h>

Program program;

void setup() {
    Serial.begin(115200);
    Serial.println("Hello world!");
    return;
    program.setup();
}

void loop() {
    return;
    program.loop();
}
