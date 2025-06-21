#pragma once

#include <lvgl.h>

class Program {
public:
    Program();

    void setup();
    void loop();
    
private:
    lv_display_t* display;
    // Isn't the full size of the screen as it is drawn partially
    lv_color_t colourBuffer[TFT_WIDTH * TFT_HEIGHT / 50];
    
    lv_obj_t* btn;
    lv_obj_t* label;
    lv_obj_t* animatedBtn;
    
    unsigned int lastMillis = 0;
};
