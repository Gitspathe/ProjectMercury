#include "Renderer.h"

using namespace World;
namespace Render
{
    void Renderer::init(GameWorld* world, Screen* screen)
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
