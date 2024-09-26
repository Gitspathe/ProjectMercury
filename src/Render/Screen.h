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

        int getWidth() const;
        int getHeight() const;
    private:
        std::vector<std::vector<Common::Color>> buffer;
        int width, height;
    };
}

#endif //SCREEN_H
