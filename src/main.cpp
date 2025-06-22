#include "Program.h"

#include <Arduino.h>

extern "C" void app_main() {
    initArduino();

    Program program;
    program.setup();

    while (true) {
        program.loop();
        delay(1);
    }
}
