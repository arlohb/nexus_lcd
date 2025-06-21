#pragma once

#include <lvgl.h>
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
    lv_obj_t* memUsageBar;
    lv_obj_t* memUsageLabel;
    lv_obj_t* testBar1;
    lv_obj_t* testBarLabel1;
    lv_obj_t* testBar2;
    lv_obj_t* testBarLabel2;
    lv_obj_t* podLabel;
    lv_obj_t* containerLabel;
};
