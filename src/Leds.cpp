#include "Leds.h"

#include <algorithm>
#include <lvgl.h>
#include "Utils.h"

Leds::Leds() : np(COUNT, PIN, NEO_GRB + NEO_KHZ800) {
    np.begin();
    np.setBrightness(1);
    np.fill(np.Color(255, 0, 0));
    np.show();
}

void Leds::start(Data* data) {
    struct Args {
        Leds* leds;
        Data* data;
    };
    
    Args* args = new Args {this, data};

    xTaskCreate([] (void* arg) {
        Args* args = static_cast<Args*>(arg);

        while (true) {
            args->leds->loop(*args->data);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }, "leds_loop", 2048, args, 1, nullptr);
}

void Leds::loop(Data& data) {
    std::vector<uint8_t> podData;
    {
        std::lock_guard<std::mutex> lock(data.podDataMutex);
        podData = data.podDataNormalised;
    }

    for (size_t i = 0; i < std::min(podData.size(), static_cast<size_t>(COUNT)); i++) {
        uint8_t value = podData[i];
        lv_color_t color = utils::color_temp_deep(static_cast<uint8_t>((float)value * 100.0f / 255.0f));
        np.setPixelColor(i, np.Color(color.red, color.green, color.blue));
    }

    np.show();
}
