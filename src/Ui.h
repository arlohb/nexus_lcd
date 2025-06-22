#pragma once

#include <lvgl.h>
#include "Data.h"

class Ui {
public:
    Ui();
    
    Ui(const Ui&) = delete;
    Ui& operator=(const Ui&) = delete;

    void start(const Data* data);

private:
    // Isn't the full size of the screen as it is drawn partially
    lv_color_t colourBuffer[512];

    lv_obj_t* cpuUsageArc;
    lv_obj_t* cpuUsageLabel;
    lv_obj_t* memUsageBar;
    lv_obj_t* memUsageValue;
    lv_obj_t* testBar1;
    lv_obj_t* testBarValue1;
    lv_obj_t* testBar2;
    lv_obj_t* testBarValue2;
    lv_obj_t* podLabel;
    lv_obj_t* containerLabel;

    void loop(const Data& data);
};
