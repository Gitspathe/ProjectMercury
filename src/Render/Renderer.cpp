#include "Renderer.h"

#include <memory>

using namespace World;
namespace Render
{
    void Renderer::init(std::shared_ptr<GameWorld> world, Screen* screen)
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
