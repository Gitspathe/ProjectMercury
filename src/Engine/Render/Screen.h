#ifndef SCREEN_H
#define SCREEN_H
#if CLIENT

namespace Engine::Render
{
    class Screen
    {
    private:
        int width, height;

    public:
        Screen(const int width, const int height)
        {
            this->width = width;
            this->height = height;
        }

        int getWidth() const
        {
            return width;
        }

        int getHeight() const
        {
            return height;
        }
    };
}

#endif
#endif //SCREEN_H
