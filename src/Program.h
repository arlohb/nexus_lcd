#pragma once

#include <lvgl.h>
#include <Adafruit_NeoPixel.h>
#include "Data.h"

class Program {
public:
    Program();

    void setup();
    void loop();
    
private:
    lv_display_t* display;
    // Isn't the full size of the screen as it is drawn partially
    lv_color_t colourBuffer[512];
    
    Data data;

    lv_obj_t* cpuUsageArc;
    lv_obj_t* cpuUsageLabel;
    lv_obj_t* memUsageBar;
    lv_obj_t* memUsageLabel;
    lv_obj_t* memUsageValue;
    lv_obj_t* testBar1;
    lv_obj_t* testBarLabel1;
    lv_obj_t* testBarValue1;
    lv_obj_t* testBar2;
    lv_obj_t* testBarLabel2;
    lv_obj_t* testBarValue2;
    lv_obj_t* podLabel;
    lv_obj_t* containerLabel;
    
    static const int NP_PIN = 32;
    static const int NP_COUNT = 32;
    Adafruit_NeoPixel np = Adafruit_NeoPixel(NP_COUNT, NP_PIN, NEO_GRB + NEO_KHZ800);
};
