#ifndef RENDERER_H
#define RENDERER_H
#include <memory>
#include "Surface.h"

namespace Render
{
    template<typename T>
    class Renderer
    {
    protected:
        std::shared_ptr<Surface<T>> backBuffer = nullptr;

        virtual void onInit() = 0;
        virtual void onUpdate() = 0;
        virtual void onDestroy() = 0;

    public:
        virtual ~Renderer() = default;

        void init(const std::shared_ptr<Surface<T>> &backBuffer)
        {
            this->backBuffer = backBuffer;
            onInit();
        }

        void update()
        {
            onUpdate();
        }

        void destroy()
        {
            onDestroy();
        }
    };
}

#endif //RENDERER_H
