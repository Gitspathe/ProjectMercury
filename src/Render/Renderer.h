#ifndef RENDERER_H
#define RENDERER_H
#include <memory>

namespace Render
{
    class Surface;

    class Renderer
    {
    protected:
        std::shared_ptr<Surface> backBuffer = nullptr;

        virtual void on_init() = 0;
        virtual void on_update() = 0;
        virtual void on_destroy() = 0;

    public:
        virtual ~Renderer() = default;

        void init(const std::shared_ptr<Surface> &backBuffer);
        void update();
        void destroy();
    };
}

#endif //RENDERER_H
