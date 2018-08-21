/**
 * @file   ColorSpace.cpp
 * @brief  ColorSpace class implementation.
 * @author zer0
 * @date   2015-09-07
 */

#include <firebell/graphic/ColorSpace.hpp>
#include <cassert>

// --------------------
NAMESPACE_FIREBELL_OPEN
// --------------------

bool rgb2ycbcr601(const rgb_t * src, ycbcr_t * dest)
{
    // Kb = 0.114
    // Kr = 0.299

    // R'd, G'd, B'd    in {0, 1, 2, ..., 255}
    // ----------------------------------------
    // Y'               in {16, 17, ..., 235}
    //    with footroom in {1, 2, ..., 15}
    //         headroom in {236, 237, ..., 254}
    //         sync.    in {0, 255}
    // Cb, Cr           in {16, 17, ..., 240}

    dest->y = 16.0 + 1.0 / 256.0 * (65.738 * src->r + 129.057 * src->g + 25.064 * src->b);
    dest->cb = 128.0 + 1.0 / 256.0 * (-37.945 * src->r - 74.494 * src->g + 112.439 * src->b);
    dest->cr = 128.0 + 1.0 / 256.0 * (112.439 * src->r - 94.154 * src->g - 18.285 * src->b);

    return true;
}

bool rgb2ycbcr709(const rgb_t * src, ycbcr_t * dest)
{
    // Kb = 0.0722
    // Kr = 0.2126
    return false;
}

float get_hue(const rgb_t & rgb)
{
    float r = static_cast<float>(rgb.r) / static_cast<float>(MAX_RGB_CHANNEL);
    float g = static_cast<float>(rgb.g) / static_cast<float>(MAX_RGB_CHANNEL);
    float b = static_cast<float>(rgb.b) / static_cast<float>(MAX_RGB_CHANNEL);

    float theta = 0.0f;
    float h = 0.0f;

    float numerator = 0.5f * ((r - g) + (r - b));
    float denominator = std::sqrt(((r - g) * (r - g)) + ((r - b) * (g - b)));

    assert(denominator != 0);
    theta = std::acos(numerator / denominator);

    if (b <= g) {
        h = theta;
    } else { // b > g
        h = MAX_HUE_CHANNEL - theta;
    }

    // Result value to percentage.
    return h / MAX_HUE_CHANNEL;
}

float get_saturation(const rgb_t & rgb)
{
    float r = static_cast<float>(rgb.r) / static_cast<float>(MAX_RGB_CHANNEL);
    float g = static_cast<float>(rgb.g) / static_cast<float>(MAX_RGB_CHANNEL);
    float b = static_cast<float>(rgb.b) / static_cast<float>(MAX_RGB_CHANNEL);

    float min = r;
    min = min < g ? min : g;
    min = min < b ? min : b;

    if ((r + g + b) == 0) {
        return 1.0f;
    }
    return 1.0f - (3.0f / (r + g + b) * min);
}

float get_intensity(const rgb_t & rgb)
{
    return static_cast<float>(rgb.r + rgb.g + rgb.b) / static_cast<float>(MAX_RGB_CHANNEL) / 3.0f;
}

// ---------------------
NAMESPACE_FIREBELL_CLOSE
// ---------------------

