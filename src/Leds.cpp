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
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }, "leds_loop", 1024, args, 1, nullptr);
}

void Leds::loop(Data& data) {
    if (!data.isArrayOk) {
        // Flash between red and white
        if (millis() % 1000 < 500) {
            np.fill(np.Color(255, 0, 0));
        } else {
            np.fill(np.Color(255, 255, 255));
        }
    } else if (data.isArrayRebuilding) {
        np.fill(np.Color(0, 0, 255));
    } else {
        np.clear();
    }

    np.show();
}
