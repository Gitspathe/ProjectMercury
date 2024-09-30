#include "ColorRGBA.h"
#include "ColorRGB.h"

namespace Common
{
    const ColorRGBA ColorRGBA::Clear = ColorRGBA(0, 0, 0, 0);
    const ColorRGBA ColorRGBA::Black = ColorRGBA(0, 0, 0);
    const ColorRGBA ColorRGBA::White = ColorRGBA(255, 255, 255);
    const ColorRGBA ColorRGBA::Red = ColorRGBA(255, 0, 0);
    const ColorRGBA ColorRGBA::Green = ColorRGBA(0, 255, 0);
    const ColorRGBA ColorRGBA::Blue = ColorRGBA(0, 0, 255);
}
