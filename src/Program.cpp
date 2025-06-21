#include "Program.h"

Program::Program() {}

void Program::setup() {
    Serial.begin(115200);

    Serial.println("Setting up display...");
    display.begin();

    pinMode(TFT_BL, OUTPUT);
    analogWrite(TFT_BL, 255);

    Serial.println("Setup complete");
}

void Program::loop() {
    Serial.println("loop");

    if (flip) {
        display.fillScreen(TFT_RED);
    } else {
        display.fillScreen(TFT_BLUE);
    }
    flip = !flip;
    
    delay(1000);
}
