#include "Program.h"

#include <Arduino.h>
#include <WiFi.h>
#include "Utils.h"
#include "Secrets.h"

Program::Program() {}

void Program::setup() {
    Serial.begin(115200);
    Serial.println();

    pinMode(TFT_BL, OUTPUT);
    analogWrite(TFT_BL, 255);
    
    Serial.println("Setting up lvgl...");

    lv_init();
    
    lv_log_register_print_cb([] (lv_log_level_t level, const char* msg) {
        Serial.println(msg);
    });

    lv_tick_set_cb([] () { return (unsigned int) millis(); });

    lv_tft_espi_create(TFT_WIDTH, TFT_HEIGHT, colourBuffer, sizeof(colourBuffer));
    
    Serial.println("Creating button...");
    
    lv_obj_t* root = lv_scr_act();
    
    cpuUsageArc = lv_arc_create(root);
    lv_obj_set_size(cpuUsageArc, TFT_WIDTH - 20, TFT_HEIGHT - 20);
    lv_arc_set_rotation(cpuUsageArc, 135);
    lv_arc_set_bg_angles(cpuUsageArc, 0, 270);
    lv_obj_center(cpuUsageArc);
    
    memUsageBar = lv_bar_create(root);
    lv_obj_align(memUsageBar, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(memUsageBar, lv_pct(70), 32);
    
    testBar1 = lv_bar_create(root);
    lv_obj_align(testBar1, LV_ALIGN_CENTER, 0, 36);
    lv_obj_set_size(testBar1, lv_pct(60), 24);
    
    testBar2 = lv_bar_create(root);
    lv_obj_align(testBar2, LV_ALIGN_CENTER, 0, -36);
    lv_obj_set_size(testBar2, lv_pct(60), 24);
    
    memUsageLabel = lv_label_create(root);
    
    const int labelHorSpacing = 30;
    const int labelVerSpacing = 20;
    const int labelVerOffset = 20;
    
    lv_obj_t* podLabelTitle = lv_label_create(root);
    lv_obj_align(podLabelTitle, LV_ALIGN_BOTTOM_MID, -labelHorSpacing, -labelVerOffset - labelVerSpacing);
    lv_label_set_text(podLabelTitle, "Pods");
    podLabel = lv_label_create(root);
    lv_obj_align(podLabel, LV_ALIGN_BOTTOM_MID, -labelHorSpacing, -labelVerOffset);

    lv_obj_t* containerLabelTitle = lv_label_create(root);
    lv_obj_align(containerLabelTitle, LV_ALIGN_BOTTOM_MID, labelHorSpacing, -labelVerOffset - labelVerSpacing);
    lv_label_set_text(containerLabelTitle, "Cons");
    containerLabel = lv_label_create(root);
    lv_obj_align(containerLabel, LV_ALIGN_BOTTOM_MID, labelHorSpacing, -labelVerOffset);
    
    Serial.println("Connecting to WiFi...");
    WiFi.begin(secrets::ssid ,secrets::password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }
    Serial.println();

    Serial.println("Setup complete");
}

void Program::loop() {
    lv_timer_handler();

    lv_arc_set_value(cpuUsageArc, data.getCpuUsage());
    lv_obj_set_style_arc_color(cpuUsageArc, utils::color_temp(lv_arc_get_value(cpuUsageArc)), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(cpuUsageArc, utils::color_temp(lv_arc_get_value(cpuUsageArc)), LV_PART_KNOB);

    lv_bar_set_value(memUsageBar, data.getMemUsage(), LV_ANIM_OFF);
    lv_obj_set_style_bg_color(memUsageBar, utils::color_temp(lv_bar_get_value(memUsageBar)), LV_PART_INDICATOR);

    lv_bar_set_value(testBar1, data.getTestValue1(), LV_ANIM_OFF);
    lv_obj_set_style_bg_color(testBar1, utils::color_temp(lv_bar_get_value(testBar1)), LV_PART_INDICATOR);

    lv_bar_set_value(testBar2, data.getTestValue2(), LV_ANIM_OFF);
    lv_obj_set_style_bg_color(testBar2, utils::color_temp(lv_bar_get_value(testBar2)), LV_PART_INDICATOR);

    int value = data.getPodCount();
    lv_label_set_text(podLabel, String(value).c_str());
    lv_obj_set_style_text_color(podLabel, utils::color_temp(value), LV_PART_MAIN);
    value = data.getContainerCount();
    lv_label_set_text(containerLabel, String(value).c_str());
    lv_obj_set_style_text_color(containerLabel, utils::color_temp(value), LV_PART_MAIN);
    
    delay(3000);
}
