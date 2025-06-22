#pragma once

#include <Adafruit_NeoPixel.h>
#include "Data.h"

class Leds {
public:
    static const int PIN = 32;
    static const int COUNT = 32;

    Leds();
    
    void setup();
    void loop(const Data& data);
    
private:
    Adafruit_NeoPixel np;
};
