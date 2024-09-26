#ifndef RENDERER_H

namespace Render
{
    class GameWorld;

    class Renderer
    {
    public:
        void init(GameWorld* world);
        void update(float deltaTime);
        void destroy();
    private:
        GameWorld* world;

        void on_init();
        void on_update();
        void on_destroy();
    };
}

#define RENDERER_H

#endif //RENDERER_H
