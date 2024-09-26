#include "Renderer.h"

namespace Render
{
    void Renderer::init(World::GameWorld* world, Screen* screen)
    {
        this->world = world;
        this->screen = screen;
        on_init();
    }

    void Renderer::update()
    {
        on_update();
    }

    void Renderer::destroy()
    {
        on_destroy();
    }

}
