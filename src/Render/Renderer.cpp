#include "Renderer.h"
#include "Surface.h"
#include <memory>

namespace Render
{
    void Renderer::init(const std::shared_ptr<Surface>& backBuffer)
    {
        this->backBuffer = backBuffer;
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
