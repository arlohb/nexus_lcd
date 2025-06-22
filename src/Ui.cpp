#include "Ui.h"

#include "Utils.h"

Ui::Ui() {}

void Ui::setup() {
    Serial.println("Setting up lvgl...");

    lv_init();
    
    lv_log_register_print_cb([] (lv_log_level_t level, const char* msg) {
        Serial.print(msg);
    });

    lv_tick_set_cb([] () { return (unsigned int) millis(); });

    lv_tft_espi_create(TFT_WIDTH, TFT_HEIGHT, colourBuffer, sizeof(colourBuffer));
    
    Serial.println("Creating UI elements...");

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
    
    cpuUsageLabel = lv_label_create(root);
    lv_label_set_text(cpuUsageLabel, "CPU 0%");
    lv_obj_align(cpuUsageLabel, LV_ALIGN_TOP_MID, 0, 40);
    lv_obj_set_style_text_color(cpuUsageLabel, lv_color_white(), LV_PART_MAIN);
    
    lv_obj_t* memUsageLabel = lv_label_create(memUsageBar);
    lv_label_set_text(memUsageLabel, "Mem");
    lv_obj_align(memUsageLabel, LV_ALIGN_LEFT_MID, 8, 0);
    lv_obj_set_style_text_color(memUsageLabel, lv_color_white(), LV_PART_MAIN);
    
    memUsageValue = lv_label_create(memUsageBar);
    lv_label_set_text(memUsageValue, "0%");
    lv_obj_align(memUsageValue, LV_ALIGN_RIGHT_MID, -8, 0);
    lv_obj_set_style_text_color(memUsageValue, lv_color_white(), LV_PART_MAIN);
    
    lv_obj_t* testBarLabel1 = lv_label_create(testBar1);
    lv_label_set_text(testBarLabel1, "T1");
    lv_obj_align(testBarLabel1, LV_ALIGN_LEFT_MID, 8, 0);
    lv_obj_set_style_text_color(testBarLabel1, lv_color_white(), LV_PART_MAIN);
    
    testBarValue1 = lv_label_create(testBar1);
    lv_label_set_text(testBarValue1, "0%");
    lv_obj_align(testBarValue1, LV_ALIGN_RIGHT_MID, -8, 0);
    lv_obj_set_style_text_color(testBarValue1, lv_color_white(), LV_PART_MAIN);

    lv_obj_t* testBarLabel2 = lv_label_create(testBar2);
    lv_label_set_text(testBarLabel2, "T2");
    lv_obj_align(testBarLabel2, LV_ALIGN_LEFT_MID, 8, 0);
    lv_obj_set_style_text_color(testBarLabel2, lv_color_white(), LV_PART_MAIN);
    
    testBarValue2 = lv_label_create(testBar2);
    lv_label_set_text(testBarValue2, "0%");
    lv_obj_align(testBarValue2, LV_ALIGN_RIGHT_MID, -8, 0);
    lv_obj_set_style_text_color(testBarValue2, lv_color_white(), LV_PART_MAIN);
    
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
}

void Ui::startTask(const Data* data) {
    struct Args {
        Ui* ui;
        const Data* data;
    };
    Args* args = new Args {this, data};

    xTaskCreate([] (void* arg) {
        Args* args = static_cast<Args*>(arg);

        while (true) {
            lv_timer_handler();
            args->ui->loop(*args->data);
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }, "lvgl_loop", 8 * 1024, args, 1, nullptr);
}

void Ui::loop(const Data& data) {
    if (lv_arc_get_value(cpuUsageArc) != data.cpuUsage) {
        lv_arc_set_value(cpuUsageArc, data.cpuUsage);
        lv_obj_set_style_arc_color(cpuUsageArc, utils::color_temp(lv_arc_get_value(cpuUsageArc)), LV_PART_INDICATOR);
        lv_obj_set_style_bg_color(cpuUsageArc, utils::color_temp(lv_arc_get_value(cpuUsageArc)), LV_PART_KNOB);
        lv_label_set_text_fmt(cpuUsageLabel, "CPU %d%%", lv_arc_get_value(cpuUsageArc));
    }

    if (lv_bar_get_value(memUsageBar) != data.memUsage) {
        lv_bar_set_value(memUsageBar, data.memUsage, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(memUsageBar, utils::color_temp(lv_bar_get_value(memUsageBar)), LV_PART_INDICATOR);
        lv_label_set_text_fmt(memUsageValue, "%d%%", lv_bar_get_value(memUsageBar));
    }

    if (lv_bar_get_value(testBar1) != data.testValue1) {
        lv_bar_set_value(testBar1, data.testValue1, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(testBar1, utils::color_temp(lv_bar_get_value(testBar1)), LV_PART_INDICATOR);
        lv_label_set_text_fmt(testBarValue1, "%d%%", lv_bar_get_value(testBar1));
    }

    if (lv_bar_get_value(testBar2) != data.testValue2) {
        lv_bar_set_value(testBar2, data.testValue2, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(testBar2, utils::color_temp(lv_bar_get_value(testBar2)), LV_PART_INDICATOR);
        lv_label_set_text_fmt(testBarValue2, "%d%%", lv_bar_get_value(testBar2));
    }
    
    int value = data.podCount;
    lv_label_set_text(podLabel, String(value).c_str());
    lv_obj_set_style_text_color(podLabel, utils::color_temp(value), LV_PART_MAIN);
    
    value = data.containerCount;
    lv_label_set_text(containerLabel, String(value).c_str());
    lv_obj_set_style_text_color(containerLabel, utils::color_temp(value), LV_PART_MAIN);
}
