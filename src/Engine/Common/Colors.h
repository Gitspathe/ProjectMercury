#ifndef COLOROPERATORS_H
#define COLOROPERATORS_H
#include "ColorRGB.h"
#include "ColorRGBA.h"
#include "ColorConstants.h"

namespace Engine::Common
{
    inline std::uint8_t clamp(const int val)
    {
        const int t = val < 0 ? 0 : val;
        return t > 255 ? 255 : t;
    }

    inline float clamp(const float val)
    {
        const float t = val < 0.0f ? 0.0f : val;
        return t > 1.0f ? 1.0f : t;
    }

    // =============================================================================================================================================
    //     RGB (RED, GREEN, BLUE)
    // =============================================================================================================================================

    inline ColorRGB ColorRGB::operator+(const ColorRGB& rhs) const {
        return ColorRGB(
            clamp(static_cast<int>(r) + rhs.r),
            clamp(static_cast<int>(g) + rhs.g),
            clamp(static_cast<int>(b) + rhs.b)
        );
    }

    inline ColorRGB ColorRGB::operator-(const ColorRGB& rhs) const {
        return ColorRGB(
            clamp(static_cast<int>(r) - rhs.r),
            clamp(static_cast<int>(g) - rhs.g),
            clamp(static_cast<int>(b) - rhs.b)
        );
    }

    inline ColorRGB ColorRGB::operator*(const ColorRGB& rhs) const {
        return ColorRGB(
            clamp(static_cast<int>(r) * rhs.r),
            clamp(static_cast<int>(g) * rhs.g),
            clamp(static_cast<int>(b) * rhs.b)
        );
    }

    inline ColorRGB ColorRGB::operator/(const ColorRGB& rhs) const
    {
        return ColorRGB(
            clamp(static_cast<int>(r) / rhs.r),
            clamp(static_cast<int>(g) / rhs.g),
            clamp(static_cast<int>(b) / rhs.b)
        );
    }

    inline ColorRGB ColorRGB::operator+(const ColorRGBA& rhs) const {
        return ColorRGB(
            clamp(static_cast<int>(r) + static_cast<int>(rhs.r) * rhs.a / 255),
            clamp(static_cast<int>(g) + static_cast<int>(rhs.g) * rhs.a / 255),
            clamp(static_cast<int>(b) + static_cast<int>(rhs.b) * rhs.a / 255)
        );
    }

    inline ColorRGB ColorRGB::operator-(const ColorRGBA& rhs) const {
        return ColorRGB(
            clamp(static_cast<int>(r) / 255 - static_cast<int>(rhs.r) * rhs.a / 255),
            clamp(static_cast<int>(g) / 255 - static_cast<int>(rhs.g) * rhs.a / 255),
            clamp(static_cast<int>(b) / 255 - static_cast<int>(rhs.b) * rhs.a / 255)
        );
    }

    inline ColorRGB ColorRGB::operator*(const ColorRGBA& rhs) const {
        return ColorRGB(
            clamp(static_cast<int>(r) * static_cast<int>(rhs.r) / 255),
            clamp(static_cast<int>(g) * static_cast<int>(rhs.g) / 255),
            clamp(static_cast<int>(b) * static_cast<int>(rhs.b) / 255)
        );
    }

    inline ColorRGB ColorRGB::operator/(const ColorRGBA& rhs) const {
        return ColorRGB(
            rhs.r == 0 ? 0 : clamp(static_cast<int>(r) / rhs.r * 255),
            rhs.g == 0 ? 0 : clamp(static_cast<int>(g) / rhs.g * 255),
            rhs.b == 0 ? 0 : clamp(static_cast<int>(b) / rhs.b * 255)
        );
    }

    // =============================================================================================================================================
    //     RGBA (RED, GREEN, BLUE, ALPHA)
    // =============================================================================================================================================

    inline ColorRGBA ColorRGBA::operator+(const ColorRGBA& rhs) const {
        return ColorRGBA(
            clamp(static_cast<int>(r) * a / 255 + static_cast<int>(rhs.r) * rhs.a / 255),
            clamp(static_cast<int>(g) * a / 255 + static_cast<int>(rhs.g) * rhs.a / 255),
            clamp(static_cast<int>(b) * a / 255 + static_cast<int>(rhs.b) * rhs.a / 255),
            clamp(a + rhs.a)
        );
    }

    inline ColorRGBA ColorRGBA::operator-(const ColorRGBA& rhs) const {
        return ColorRGBA(
            clamp(static_cast<int>(r) * a / 255 - static_cast<int>(rhs.r) * rhs.a / 255),
            clamp(static_cast<int>(g) * a / 255 - static_cast<int>(rhs.g) * rhs.a / 255),
            clamp(static_cast<int>(b) * a / 255 - static_cast<int>(rhs.b) * rhs.a / 255),
            clamp(a - rhs.a)
        );
    }

    inline ColorRGBA ColorRGBA::operator*(const ColorRGBA& rhs) const {
        return ColorRGBA(
            clamp(static_cast<int>(r) * static_cast<int>(rhs.r) / 255),
            clamp(static_cast<int>(g) * static_cast<int>(rhs.g) / 255),
            clamp(static_cast<int>(b) * static_cast<int>(rhs.b) / 255),
            clamp(a * rhs.a / 255)
        );
    }

    inline ColorRGBA ColorRGBA::operator/(const ColorRGBA& rhs) const {
        return ColorRGBA(
            rhs.r == 0 ? 0 : clamp(static_cast<int>(r) * 255 / rhs.r),
            rhs.g == 0 ? 0 : clamp(static_cast<int>(g) * 255 / rhs.g),
            rhs.b == 0 ? 0 : clamp(static_cast<int>(b) * 255 / rhs.b),
            rhs.a == 0 ? 0 : clamp(a / rhs.a * 255)
        );
    }

    inline ColorRGBA ColorRGBA::operator+(const ColorRGB& rhs) const {
        return ColorRGBA(
            clamp(static_cast<int>(r) + rhs.r),
            clamp(static_cast<int>(g) + rhs.g),
            clamp(static_cast<int>(b) + rhs.b),
            a
        );
    }

    inline ColorRGBA ColorRGBA::operator-(const ColorRGB& rhs) const {
        return ColorRGBA(
            clamp(static_cast<int>(r) - rhs.r),
            clamp(static_cast<int>(g) - rhs.g),
            clamp(static_cast<int>(b) - rhs.b),
            a
        );
    }

    inline ColorRGBA ColorRGBA::operator*(const ColorRGB& rhs) const {
        return ColorRGBA(
            clamp(static_cast<int>(r) * rhs.r / 255),
            clamp(static_cast<int>(g) * rhs.g / 255),
            clamp(static_cast<int>(b) * rhs.b / 255),
            a
        );
    }

    inline ColorRGBA ColorRGBA::operator/(const ColorRGB& rhs) const {
        return ColorRGBA(
            rhs.r == 0 ? 0 : clamp(static_cast<int>(r) / rhs.r * 255),
            rhs.g == 0 ? 0 : clamp(static_cast<int>(g) / rhs.g * 255),
            rhs.b == 0 ? 0 : clamp(static_cast<int>(b) / rhs.b * 255),
            a
        );
    }
}
#endif //COLOROPERATORS_H
