#ifndef COLORCONVERSION_H
#define COLORCONVERSION_H
#include "ColorRGB.h"
#include "ColorRGBA.h"

namespace Common::ColorConversion
{
    inline ColorRGB toRGB(const ColorRGBA& color)
    {
        return ColorRGB(color.r, color.g, color.b);
    }

    inline ColorRGBA toRGBA(const ColorRGB& color, const uint8_t alpha = 255)
    {
        return ColorRGBA(color.r, color.g, color.b, alpha);
    }

    template<typename TTarget, typename TSource>
    TTarget convertColor(const TSource& color)
    {
        if constexpr (std::is_same_v<TSource, ColorRGB> && std::is_same_v<TTarget, ColorRGBA>) {
            return ColorConversion::toRGBA(color);
        } else if constexpr (std::is_same_v<TSource, ColorRGBA> && std::is_same_v<TTarget, ColorRGB>) {
            return ColorConversion::toRGB(color);
        } else {
            return color;
        }
    }

    template<typename T>
    T getClear()
    {
        if constexpr (std::is_same_v<T, ColorRGB>) {
            return ColorRGB::Black;
        } else if constexpr (std::is_same_v<T, ColorRGBA>) {
            return ColorRGBA::Clear;
        } else {
            return T();
        }
    }
}

#endif //COLORCONVERSION_H
