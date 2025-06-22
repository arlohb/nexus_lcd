#pragma once

#include <Adafruit_NeoPixel.h>
#include "Data.h"
#include "Ui.h"

class Program {
public:
    Program();

    void setup();
    void loop();
    
private:
    // Isn't the full size of the screen as it is drawn partially
    lv_color_t colourBuffer[512];
    
    Ui ui;
    Data data;

    static const int NP_PIN = 32;
    static const int NP_COUNT = 32;
    Adafruit_NeoPixel np = Adafruit_NeoPixel(NP_COUNT, NP_PIN, NEO_GRB + NEO_KHZ800);
};
