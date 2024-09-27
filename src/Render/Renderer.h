#ifndef RENDERER_H
#define RENDERER_H

namespace World
{
    class GameWorld;
}

namespace Render
{
    class Screen;

    using namespace World;

    class Renderer
    {
    public:
        virtual ~Renderer() = default;

        void init(GameWorld* world, Screen* screen);
        void update();
        void destroy();
    protected:
        GameWorld* world;
        Screen* screen;

        virtual void on_init() = 0;
        virtual void on_update() = 0;
        virtual void on_destroy() = 0;
    };
}

#endif //RENDERER_H
