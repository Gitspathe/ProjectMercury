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
    class Screen
    {
    public:
        Screen(int width, int height);

        Common::Color3 get(int x, int y) const;
        Common::Color3 getSafe(int x, int y) const;
        void set(int x, int y, Common::Color3 color);
        void setSafe(int x, int y, Common::Color3 color);
        void clear(Common::Color3 color);
        void drawPoint(int size, int x, int y, Common::Color3 color, bool centered = false);
        void drawLine(int x0, int y0, int x1, int y1, Common::Color3 color);
        void drawRect(Common::RectF rect, Common::Color3 color, bool centered = false);

        int getWidth() const;
        int getHeight() const;
        std::vector<Common::Color3> getBuffer();
    private:
        std::vector<Common::Color3> buffer;
        int width, height;
    };
}

#endif //SCREEN_H
