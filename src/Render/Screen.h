#ifndef SCREEN_H
#define SCREEN_H
#include <vector>


namespace Common {
    struct Color;
}

namespace Render
{
    class Screen
    {
    public:
        Screen(int width, int height);

        Common::Color get(int x, int y) const;
        void set(int x, int y, Common::Color color);
        void clear(Common::Color color);
        void drawPoint(int size, int x, int y, Common::Color color, bool centered = false);
        void drawLine(int x0, int y0, int x1, int y1, Common::Color color);

        int getWidth() const;
        int getHeight() const;
        std::vector<Common::Color> getBuffer();
    private:
        std::vector<Common::Color> buffer;
        int width, height;
    };
}

#endif //SCREEN_H
