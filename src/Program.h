#pragma once

#include "Data.h"
#include "Leds.h"
#include "Ui.h"

class Program {
public:
    Program();
    
    Program(const Program&) = delete;
    Program& operator=(const Program&) = delete;
    
    void start();

private:
    Data data;
    Ui ui;
    Leds leds;

    void startWifi();
};
