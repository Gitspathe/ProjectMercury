#ifndef RENDERER_H
#define RENDERER_H

namespace World {
    class GameWorld;
}

namespace Render
{
    class Screen;

    class Renderer
    {
    public:
        virtual ~Renderer() = default;

        void init(World::GameWorld* world, Screen* screen);
        void update();
        void destroy();
    protected:
        World::GameWorld* world;
        Screen* screen;

        virtual void on_init() = 0;
        virtual void on_update() = 0;
        virtual void on_destroy() = 0;
    };
}

#endif //RENDERER_H
