#include <fmt/core.h>
#include <Arduino.h>

#include "lcd/LCD_Driver.h"

void setup() {
    Serial.begin(115200);

    Serial.println("setup: Config_Init");
    Serial.flush();
    Config_Init();
    Serial.println("setup: LCD_Init");
    Serial.flush();
    LCD_Init();

    LCD_SetBacklight(255);

    Serial.println(fmt::format("Hello world! 🤖").c_str());
}

bool flip = false;

void loop() {
    Serial.println("loop");

    if (flip) {
        LCD_Clear(0x0000);
    } else {
        LCD_Clear(0xF800); // Red in RGB565
    }
    flip = !flip;
    
    delay(1000);
}

