#include "Program.h"

#include <Arduino.h>

extern "C" void app_main() {
    initArduino();
    Serial.begin(115200);
    Serial.println("Starting program...");

    Program* program = new Program();
    program->start();
    
    vTaskDelete(nullptr);
}
