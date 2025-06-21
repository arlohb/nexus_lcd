#include <fmt/core.h>
#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI display;

void setup() {
    Serial.begin(115200);
    Serial.println("Setting up display...");
    display.begin();

    pinMode(TFT_BL, OUTPUT);
    analogWrite(TFT_BL, 255);
    
    Serial.println("Setup complete");
}

bool flip = false;

void loop() {
    Serial.println("loop");

    if (flip) {
        display.fillScreen(TFT_RED);
    } else {
        display.fillScreen(TFT_BLUE);
    }
    flip = !flip;
    
    delay(1000);
}

