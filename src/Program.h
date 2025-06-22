#pragma once

#include "Data.h"
#include "Leds.h"
#include "Ui.h"

class Program {
public:
    Program();
    
    Program(const Program&) = delete;
    Program& operator=(const Program&) = delete;
    
    void setupLvgl();
    void startWifi();

    void setup();
    
    void startLvglTask();
    
private:
    // Isn't the full size of the screen as it is drawn partially
    lv_color_t colourBuffer[512];
    
    Data data;
    Ui ui;
    Leds leds;
};
