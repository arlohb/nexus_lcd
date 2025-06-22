#include "Program.h"

#include <Arduino.h>

extern "C" void app_main() {
    initArduino();

    Program* program = new Program();
    program->setup();
    
    vTaskDelete(nullptr);
}
