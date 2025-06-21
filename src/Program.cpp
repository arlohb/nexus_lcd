#include "Program.h"

#include <Arduino.h>

Program::Program() {}

void Program::setup() {
    Serial.begin(115200);
    Serial.println();

    pinMode(TFT_BL, OUTPUT);
    analogWrite(TFT_BL, 255);
    
    Serial.println("Setting up lvgl...");

    lv_init();
    lv_tick_set_cb([] () { return (unsigned int) millis(); });

    lv_tft_espi_create(TFT_WIDTH, TFT_HEIGHT, colourBuffer, sizeof(colourBuffer));
    
    Serial.println("Creating button...");
    
    lv_obj_t* root = lv_scr_act();

    btn = lv_button_create(root);
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(btn, 200, 100);
    
    label = lv_label_create(btn);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    animatedBtn = lv_button_create(root);
    lv_obj_set_style_bg_color(animatedBtn, lv_color_hex(0xFF0000), LV_PART_MAIN);
    lv_obj_set_size(animatedBtn, 20, 20);

    Serial.println("Setup complete");
}

void Program::loop() {
    lv_timer_handler();

    unsigned int frameTime = millis() - lastMillis;
    lastMillis = millis();
    
    lv_label_set_text_fmt(label, "%d fps", 1000 / frameTime);
    
    // Animate in a circle around the display
    float theta = (millis() / 1000.0) * 1 * PI;
    lv_obj_set_pos(animatedBtn, 
        (TFT_WIDTH / 2) + (TFT_WIDTH / 2 - 50) * cos(theta),
        (TFT_HEIGHT / 2) + (TFT_HEIGHT / 2 - 50) * sin(theta)
    );
    
    yield();
}
