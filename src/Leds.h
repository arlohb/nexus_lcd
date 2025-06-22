#pragma once

#include <Adafruit_NeoPixel.h>
#include "Data.h"

class Leds {
public:
    static const int PIN = 32;
    static const int COUNT = 32;

    Leds();
    
    Leds(const Leds&) = delete;
    Leds& operator=(const Leds&) = delete;
    
    void start(const Data* data);
    
private:
    void loop(const Data& data);

    Adafruit_NeoPixel np;
};
