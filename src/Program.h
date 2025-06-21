#pragma once

#include <TFT_eSPI.h>

class Program {
public:
    Program();

    void setup();
    void loop();
    
private:
    TFT_eSPI display;

    bool flip = false;
};
