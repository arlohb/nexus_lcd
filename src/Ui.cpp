#include "Ui.h"

#include "Utils.h"

Ui::Ui() {
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
    lv_arc_set_value(cpuUsageArc, 0);
    lv_obj_center(cpuUsageArc);
    
    memUsageBar = lv_bar_create(root);
    lv_obj_align(memUsageBar, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(memUsageBar, lv_pct(70), 32);
    lv_bar_set_value(memUsageBar, 0, LV_ANIM_OFF);
    
    nasUsageBar = lv_bar_create(root);
    lv_obj_align(nasUsageBar, LV_ALIGN_CENTER, 0, 36);
    lv_obj_set_size(nasUsageBar, lv_pct(60), 24);
    lv_bar_set_value(nasUsageBar, 0, LV_ANIM_OFF);
    
    testBar = lv_bar_create(root);
    lv_obj_align(testBar, LV_ALIGN_CENTER, 0, -36);
    lv_obj_set_size(testBar, lv_pct(60), 24);
    lv_bar_set_value(testBar, 0, LV_ANIM_OFF);
    
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
    
    lv_obj_t* testBarLabel1 = lv_label_create(nasUsageBar);
    lv_label_set_text(testBarLabel1, "Nas");
    lv_obj_align(testBarLabel1, LV_ALIGN_LEFT_MID, 8, 0);
    lv_obj_set_style_text_color(testBarLabel1, lv_color_white(), LV_PART_MAIN);
    
    nasUsageBarValue = lv_label_create(nasUsageBar);
    lv_label_set_text(nasUsageBarValue, "0%");
    lv_obj_align(nasUsageBarValue, LV_ALIGN_RIGHT_MID, -8, 0);
    lv_obj_set_style_text_color(nasUsageBarValue, lv_color_white(), LV_PART_MAIN);

    lv_obj_t* testBarLabel2 = lv_label_create(testBar);
    lv_label_set_text(testBarLabel2, "Test");
    lv_obj_align(testBarLabel2, LV_ALIGN_LEFT_MID, 8, 0);
    lv_obj_set_style_text_color(testBarLabel2, lv_color_white(), LV_PART_MAIN);
    
    testBarValue = lv_label_create(testBar);
    lv_label_set_text(testBarValue, "0%");
    lv_obj_align(testBarValue, LV_ALIGN_RIGHT_MID, -8, 0);
    lv_obj_set_style_text_color(testBarValue, lv_color_white(), LV_PART_MAIN);
    
    const int labelHorSpacing = 30;
    const int labelVerSpacing = 20;
    const int labelVerOffset = 20;
    
    lv_obj_t* podLabelTitle = lv_label_create(root);
    lv_obj_align(podLabelTitle, LV_ALIGN_BOTTOM_MID, -labelHorSpacing, -labelVerOffset - labelVerSpacing);
    lv_label_set_text(podLabelTitle, "Pods");
    podLabel = lv_label_create(root);
    lv_obj_align(podLabel, LV_ALIGN_BOTTOM_MID, -labelHorSpacing, -labelVerOffset);
    lv_label_set_text(podLabel, "0");

    lv_obj_t* containerLabelTitle = lv_label_create(root);
    lv_obj_align(containerLabelTitle, LV_ALIGN_BOTTOM_MID, labelHorSpacing, -labelVerOffset - labelVerSpacing);
    lv_label_set_text(containerLabelTitle, "Cons");
    containerLabel = lv_label_create(root);
    lv_obj_align(containerLabel, LV_ALIGN_BOTTOM_MID, labelHorSpacing, -labelVerOffset);
    lv_label_set_text(containerLabel, "0");
}

void Ui::start(const Data* data) {
    struct Args {
        Ui* ui;
        const Data* data;
    };
    Args* args = new Args {this, data};

    xTaskCreate([] (void* arg) {
        Args* args = static_cast<Args*>(arg);

        while (true) {
            args->ui->loop(*args->data);

            // This includes rendering
            lv_timer_handler();

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

    if (lv_bar_get_value(nasUsageBar) != data.nasUsage) {
        lv_bar_set_value(nasUsageBar, data.nasUsage, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(nasUsageBar, utils::color_temp(lv_bar_get_value(nasUsageBar)), LV_PART_INDICATOR);
        lv_label_set_text_fmt(nasUsageBarValue, "%d%%", lv_bar_get_value(nasUsageBar));
    }

    if (lv_bar_get_value(testBar) != data.testValue) {
        lv_bar_set_value(testBar, data.testValue, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(testBar, utils::color_temp(lv_bar_get_value(testBar)), LV_PART_INDICATOR);
        lv_label_set_text_fmt(testBarValue, "%d%%", lv_bar_get_value(testBar));
    }
    
    int value = data.podCount;
    lv_label_set_text(podLabel, String(value).c_str());
    lv_obj_set_style_text_color(podLabel, utils::color_temp(value), LV_PART_MAIN);
    
    value = data.containerCount;
    lv_label_set_text(containerLabel, String(value).c_str());
    lv_obj_set_style_text_color(containerLabel, utils::color_temp(value), LV_PART_MAIN);
}
