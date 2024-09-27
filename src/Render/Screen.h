#ifndef SCREEN_H
#define SCREEN_H
#include <vector>
#include "../Common/RectF.h"

namespace Common {
    struct Color3;
    struct RectF;
}

namespace Render
{
    using namespace Common;

    class Screen
    {
    public:
        Screen(int width, int height);

        Color3 get(int x, int y) const;
        Color3 getSafe(int x, int y) const;
        void set(int x, int y, Color3 color);
        void setSafe(int x, int y, Color3 color);
        void clear(Color3 color);
        void drawPoint(int size, int x, int y, Color3 color, bool centered = false);
        void drawLine(int x0, int y0, int x1, int y1, Color3 color);
        void drawRect(RectF rect, Color3 color, bool centered = false);

        int getWidth() const;
        int getHeight() const;
        std::vector<Color3> getBuffer();
    private:
        std::vector<Color3> buffer;
        int width, height;
    };
}

#endif //SCREEN_H
