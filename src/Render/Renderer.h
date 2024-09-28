#ifndef RENDERER_H
#define RENDERER_H
#include <memory>

namespace World {
    class GameWorld;
}

namespace Render
{
    class Screen;

    class Renderer
    {
    protected:
        std::shared_ptr<World::GameWorld> world = nullptr;
        Screen* screen = nullptr;

        virtual void on_init() = 0;
        virtual void on_update() = 0;
        virtual void on_destroy() = 0;

    public:
        virtual ~Renderer() = default;

        void init(std::shared_ptr<World::GameWorld> world, Screen* screen);
        void update();
        void destroy();
    };
}

#endif //RENDERER_H
