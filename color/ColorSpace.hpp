/**
 * @file   ColorSpace.hpp
 * @brief  ColorSpace class prototype.
 * @author zer0
 * @date   2015-09-07
 */

#ifndef __INCLUDE_FIREBELL__FIREBELL_GRAPHIC_COLORSPACE_HPP__
#define __INCLUDE_FIREBELL__FIREBELL_GRAPHIC_COLORSPACE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <firebell/config.hpp>
#include <cstdint>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846  /* pi */
#endif

// --------------------
NAMESPACE_FIREBELL_OPEN
// --------------------

/** Maximum RGB color channel. */
const uint8_t MAX_RGB_CHANNEL = 255;

/** Maximum Hue (from HSI) color channel. */
const float MAX_HUE_CHANNEL = M_PI * 2;

/**
 * RGB color system structure.
 *
 * @remarks
 *  R, G, B, A channel assigned 0 ~ 255 value.
 */
struct rgb_t
{
    uint8_t r, g, b;

    rgb_t()
    {
        set(0, 0, 0);
    }

    rgb_t(const uint8_t & r, const uint8_t & g, const uint8_t & b)
    {
        set(r, g, b);
    }

    inline void set(const uint8_t & r, const uint8_t & g, const uint8_t & b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }
};

/**
 * HSI color system structure.
 *
 * @remarks
 *  HSI channel assigned 0.0 ~ 1.0 value: @n
 *  hue is 0 ~ 360 to percentage. @n
 *  saturation is 0 ~ 100 to percentage. @n
 *  intencity is 0 ~ 100 to percentage.
 */
struct hsi_t
{
    float h, s, i;

    hsi_t()
    {
        set(0, 0, 0);
    }

    hsi_t(const float & h, const float & s, const float & i)
    {
        set(h, s, i);
    }

    inline void set(const float & h, const float & s, const float & i)
    {
        this->h = h;
        this->s = s;
        this->i = i;
    }
};

/**
 * YCbCr color system structure.
 *
 * @remarks
 *  Y(Luminance). @n
 *  Cb(Chrominance Blue). @n
 *  Cr(Chrominance Red).
 */
struct ycbcr_t
{
    float y, cb, cr;

    ycbcr_t()
    {
        set(0, 0, 0);
    }

    ycbcr_t(const float & y, const float & cb, const float & cr)
    {
        set(y, cb, cr);
    }

    inline void set(const float & y, const float & cb, const float & cr)
    {
        this->y = y;
        this->cb = cb;
        this->cr = cr;
    }
};

// -------------------
// Conversion methods.
// -------------------

/**
 * Convert RGB to YCbCr(ITU-R BT.601).
 *
 * @remarks
 *  YCbCr(ITU-R BT.601) is used in the SDTV(Standard-Definition Television).
 *
 * @return
 *  Convert successful a return true, otherwise return the false.
 */
bool rgb2ycbcr601(const rgb_t * src, ycbcr_t * dest);

/**
 * Convert RGB to YCbCr(ITU-R BT.709).
 *
 * @remarks
 *  YCbCr(ITU-R BT.709) is used in the HDTV(high-definition television).
 *
 * @return
 *  Convert successful a return true, otherwise return the false.
 *
 * @warning
 *  This method is the not implemented.
 */
bool rgb2ycbcr709(const rgb_t * src, ycbcr_t * dest);

/**
 * Convert RGB to Hue (from HSI).
 *
 * @return 0.0 ~ 1.0 hue value.
 *
 * @remarks
 *  hue is 0 ~ (2 * M_PI) to assigned 0.0 ~ 1.0 percentage.
 */
float get_hue(const rgb_t & rgb);

/**
 * Convert RGB to Saturation (from HSI).
 *
 * @return 0.0 ~ 1.0 saturation value.
 *
 * @remarks
 *  saturation is 0 ~ 100 to assigned 0.0 ~ 1.0 percentage.
 */
float get_saturation(const rgb_t & rgb);

/**
 * Convert RGB to Intensity (from HSI).
 *
 * @return 0.0 ~ 1.0 saturation value.
 *
 * @remarks
 *  intensity is 0 ~ 100 to assigned 0.0 ~ 1.0 percentage.
 */
float get_intensity(const rgb_t & rgb);

// ---------------------
NAMESPACE_FIREBELL_CLOSE
// ---------------------

#endif // __INCLUDE_FIREBELL__FIREBELL_GRAPHIC_COLORSPACE_HPP__

