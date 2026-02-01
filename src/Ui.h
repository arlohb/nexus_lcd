#pragma once

#include <lvgl.h>
#include "Data.h"

class Ui {
public:
    Ui();
    
    Ui(const Ui&) = delete;
    Ui& operator=(const Ui&) = delete;

    void start(Data* data);

private:
    // Isn't the full size of the screen as it is drawn partially
    lv_color_t colourBuffer[256];

    lv_obj_t* nodeCpuUsageArcs[Data::NODE_COUNT];
    lv_obj_t* timeLabel;
    lv_obj_t* memUsageBar;
    lv_obj_t* memUsageValue;
    lv_obj_t* nasUsageBar;
    lv_obj_t* nasUsageBarValue;
    lv_obj_t* cpuUsageBar;
    lv_obj_t* cpuUsageBarValue;
    lv_obj_t* podLabel;
    lv_obj_t* containerLabel;

    void loop(Data& data);
};
