#ifndef COLOROPERATORS_H
#define COLOROPERATORS_H

#include "ColorRGB.h"
#include "ColorRGBA.h"

namespace Common
{
    inline ColorRGB ColorRGB::operator+(const ColorRGB& rhs) const {
        return ColorRGB(
            std::clamp(static_cast<int>(r) + rhs.r, 0, 255),
            std::clamp(static_cast<int>(g) + rhs.g, 0, 255),
            std::clamp(static_cast<int>(b) + rhs.b, 0, 255)
        );
    }

    inline ColorRGB ColorRGB::operator-(const ColorRGB& rhs) const {
        return ColorRGB(
            std::clamp(static_cast<int>(r) - rhs.r, 0, 255),
            std::clamp(static_cast<int>(g) - rhs.g, 0, 255),
            std::clamp(static_cast<int>(b) - rhs.b, 0, 255)
        );
    }

    inline ColorRGB ColorRGB::operator*(const ColorRGB& rhs) const {
        return ColorRGB(
            std::clamp(static_cast<int>(r) * rhs.r, 0, 255),
            std::clamp(static_cast<int>(g) * rhs.g, 0, 255),
            std::clamp(static_cast<int>(b) * rhs.b, 0, 255)
        );
    }

    inline ColorRGB ColorRGB::operator/(const ColorRGB& rhs) const
    {
        return ColorRGB(
            std::clamp(static_cast<int>(r) / rhs.r, 0, 255),
            std::clamp(static_cast<int>(g) / rhs.g, 0, 255),
            std::clamp(static_cast<int>(b) / rhs.b, 0, 255)
        );
    }

    inline ColorRGB ColorRGB::operator+(const ColorRGBA& rhs) const {
        return ColorRGB(
            std::clamp(static_cast<int>(r) / 255 + static_cast<int>(rhs.r) * rhs.a / 255, 0, 255),
            std::clamp(static_cast<int>(g) / 255 + static_cast<int>(rhs.g) * rhs.a / 255, 0, 255),
            std::clamp(static_cast<int>(b) / 255 + static_cast<int>(rhs.b) * rhs.a / 255, 0, 255)
        );
    }

    inline ColorRGB ColorRGB::operator-(const ColorRGBA& rhs) const {
        return ColorRGB(
            std::clamp(static_cast<int>(r) / 255 - static_cast<int>(rhs.r) * rhs.a / 255, 0, 255),
            std::clamp(static_cast<int>(g) / 255 - static_cast<int>(rhs.g) * rhs.a / 255, 0, 255),
            std::clamp(static_cast<int>(b) / 255 - static_cast<int>(rhs.b) * rhs.a / 255, 0, 255)
        );
    }

    inline ColorRGB ColorRGB::operator*(const ColorRGBA& rhs) const {
        return ColorRGB(
            std::clamp(static_cast<int>(r) * static_cast<int>(rhs.r) / 255, 0, 255),
            std::clamp(static_cast<int>(g) * static_cast<int>(rhs.g) / 255, 0, 255),
            std::clamp(static_cast<int>(b) * static_cast<int>(rhs.b) / 255, 0, 255)
        );
    }

    inline ColorRGB ColorRGB::operator/(const ColorRGBA& rhs) const {
        return ColorRGB(
            rhs.r == 0 ? 0 : std::clamp(static_cast<int>(r) / rhs.r * 255, 0, 255),
            rhs.g == 0 ? 0 : std::clamp(static_cast<int>(g) / rhs.g * 255, 0, 255),
            rhs.b == 0 ? 0 : std::clamp(static_cast<int>(b) / rhs.b * 255, 0, 255)
        );
    }

    inline ColorRGBA ColorRGBA::operator+(const ColorRGBA& rhs) const {
        return ColorRGBA(
            std::clamp(static_cast<int>(r) * a / 255 + static_cast<int>(rhs.r) * rhs.a / 255, 0, 255),
            std::clamp(static_cast<int>(g) * a / 255 + static_cast<int>(rhs.g) * rhs.a / 255, 0, 255),
            std::clamp(static_cast<int>(b) * a / 255 + static_cast<int>(rhs.b) * rhs.a / 255, 0, 255),
            std::clamp(a + rhs.a, 0, 255)
        );
    }

    inline ColorRGBA ColorRGBA::operator-(const ColorRGBA& rhs) const {
        return ColorRGBA(
            std::clamp(static_cast<int>(r) * a / 255 - static_cast<int>(rhs.r) * rhs.a / 255, 0, 255),
            std::clamp(static_cast<int>(g) * a / 255 - static_cast<int>(rhs.g) * rhs.a / 255, 0, 255),
            std::clamp(static_cast<int>(b) * a / 255 - static_cast<int>(rhs.b) * rhs.a / 255, 0, 255),
            std::clamp(a - rhs.a, 0, 255)
        );
    }

    inline ColorRGBA ColorRGBA::operator*(const ColorRGBA& rhs) const {
        return ColorRGBA(
            std::clamp(static_cast<int>(r) * static_cast<int>(rhs.r) / 255, 0, 255),
            std::clamp(static_cast<int>(g) * static_cast<int>(rhs.g) / 255, 0, 255),
            std::clamp(static_cast<int>(b) * static_cast<int>(rhs.b) / 255, 0, 255),
            std::clamp(a * rhs.a / 255, 0, 255)
        );
    }

    inline ColorRGBA ColorRGBA::operator/(const ColorRGBA& rhs) const {
        return ColorRGBA(
            rhs.r == 0 ? 0 : std::clamp(static_cast<int>(r) * 255 / rhs.r, 0, 255),
            rhs.g == 0 ? 0 : std::clamp(static_cast<int>(g) * 255 / rhs.g, 0, 255),
            rhs.b == 0 ? 0 : std::clamp(static_cast<int>(b) * 255 / rhs.b, 0, 255),
            rhs.a == 0 ? 0 : std::clamp(a / rhs.a * 255, 0, 255)
        );
    }

    inline ColorRGBA ColorRGBA::operator+(const ColorRGB& rhs) const {
        return ColorRGBA(
            std::clamp(static_cast<int>(r) + rhs.r, 0, 255),
            std::clamp(static_cast<int>(g) + rhs.g, 0, 255),
            std::clamp(static_cast<int>(b) + rhs.b, 0, 255),
            a
        );
    }

    inline ColorRGBA ColorRGBA::operator-(const ColorRGB& rhs) const {
        return ColorRGBA(
            std::clamp(static_cast<int>(r) - rhs.r, 0, 255),
            std::clamp(static_cast<int>(g) - rhs.g, 0, 255),
            std::clamp(static_cast<int>(b) - rhs.b, 0, 255),
            a
        );
    }

    inline ColorRGBA ColorRGBA::operator*(const ColorRGB& rhs) const {
        return ColorRGBA(
            std::clamp(static_cast<int>(r) * rhs.r / 255, 0, 255),
            std::clamp(static_cast<int>(g) * rhs.g / 255, 0, 255),
            std::clamp(static_cast<int>(b) * rhs.b / 255, 0, 255),
            a
        );
    }

    inline ColorRGBA ColorRGBA::operator/(const ColorRGB& rhs) const {
        return ColorRGBA(
            rhs.r == 0 ? 0 : std::clamp(static_cast<int>(r) / rhs.r * 255, 0, 255),
            rhs.g == 0 ? 0 : std::clamp(static_cast<int>(g) / rhs.g * 255, 0, 255),
            rhs.b == 0 ? 0 : std::clamp(static_cast<int>(b) / rhs.b * 255, 0, 255),
            a
        );
    }
}
#endif //COLOROPERATORS_H
