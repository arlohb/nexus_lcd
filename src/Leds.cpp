#include "Leds.h"

#include <algorithm>
#include <lvgl.h>
#include "Utils.h"

Leds::Leds() : np(COUNT, PIN, NEO_GRB + NEO_KHZ800) {}

void Leds::setup() {
    np.begin();
    np.setBrightness(1);
    np.fill(np.Color(255, 0, 0));
    np.show();
}

void Leds::loop(const Data& data) {
    auto podData = data.getPodData();
    for (size_t i = 0; i < std::min(podData.size(), static_cast<size_t>(COUNT)); i++) {
        uint8_t value = podData[i];
        lv_color_t color = utils::color_temp_deep(static_cast<uint8_t>((float)value * 100.0f / 255.0f));
        np.setPixelColor(i, np.Color(color.red, color.green, color.blue));
    }
    np.show();
}
