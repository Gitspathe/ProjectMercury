#ifndef COLORCONSTANTS_H
#define COLORCONSTANTS_H

namespace Engine::Common
{
    namespace BlendMode
    {
        constexpr uint8_t CLEAR    = 0;
        constexpr uint8_t COLOR    = 1;
        constexpr uint8_t ADD      = 2;
        constexpr uint8_t SUBTRACT = 3;
        constexpr uint8_t MULTIPLY = 4;
        constexpr uint8_t DIVIDE   = 5;
    }

    namespace ColorFormat
    {
        constexpr uint8_t RGB = 0;
        constexpr uint8_t RGBA = 1;
    }
}

#endif //COLORCONSTANTS_H
