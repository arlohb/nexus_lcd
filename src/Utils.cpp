#include "Utils.h"

template <typename T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
    return (v < lo) ? lo : (hi < v) ? hi : v;
}

namespace utils {
    
lv_color_t color_hsv_mix(lv_color_t c1, lv_color_t c2, uint8_t t) {
    lv_color_hsv_t hsv1 = lv_color_to_hsv(c1);
    lv_color_hsv_t hsv2 = lv_color_to_hsv(c2);
    
    if (hsv2.h - hsv1.h > 180) {
        hsv1.h += 360;
    } else if (hsv1.h - hsv2.h > 180) {
        hsv2.h += 360;
    }
    
    lv_color_hsv_t hsv;
    hsv.h = (hsv1.h * (255 - t) + hsv2.h * t) / 255;
    hsv.s = (hsv1.s * (255 - t) + hsv2.s * t) / 255;
    hsv.v = (hsv1.v * (255 - t) + hsv2.v * t) / 255;

    return lv_color_hsv_to_rgb(hsv.h, hsv.s, hsv.v);
}

lv_color_t color_temp(int value) {
    uint8_t t = static_cast<uint8_t>(clamp(value, 0, 100) / 100.0f * 255.0f);
    return color_hsv_mix(lv_palette_lighten(LV_PALETTE_BLUE, 2), lv_palette_main(LV_PALETTE_RED), t);
}

lv_color_t color_temp_deep(int value) {
    uint8_t t = static_cast<uint8_t>(clamp(value, 0, 100) / 100.0f * 255.0f);
    return color_hsv_mix(lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), t);
}
    
}
