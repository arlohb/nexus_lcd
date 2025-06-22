#pragma once

#include <lvgl.h>

namespace utils {

/**
 * @brief Mixes two colors in HSV space with a given ratio.
 *
 * This function interpolates between two colors, `c1` and `c2`, in the HSV color space.
 * The interpolation factor `t` determines the weight of each color in the mix:
 * - When `t` is 0, the result is entirely `c1`.
 * - When `t` is 255, the result is entirely `c2`.
 * - Values in between produce a linear blend.
 *
 * The function also handles hue wrapping to ensure smooth transitions across the 0/360 boundary.
 *
 * @param c1 The first color (as lv_color_t).
 * @param c2 The second color (as lv_color_t).
 * @param t  The interpolation factor (0-255).
 * @return The resulting mixed color (as lv_color_t).
 */
lv_color_t color_hsv_mix(lv_color_t c1, lv_color_t c2, uint8_t t);

/**
 * @brief Generates a color representing a temperature value.
 *
 * This function maps an integer temperature value (expected range: 0-100)
 * to a color by blending between a lightened blue and a red color.
 * The blending is performed in HSV color space, where 0 corresponds to blue
 * and 100 corresponds to red.
 *
 * @param value Integer value in the range [0, 100].
 * @return lv_color_t The resulting blended color.
 */
lv_color_t color_temp(int value);

/**
 * @brief Generates a color representing a temperature value.
 *
 * Very similar to color_temp, but with a deep blue.
 *
 * @param value Integer value in the range [0, 100].
 * @return lv_color_t The resulting blended color.
 */
lv_color_t color_temp_deep(int value);

}