#ifndef RENDERER_H
#define RENDERER_H
#if CLIENT
#include <memory>
#include "Surface.h"

namespace Engine::Render
{
    template<typename T>
    class Renderer
    {
    protected:
        std::shared_ptr<Surface<T>> backBuffer;

        virtual bool onInit() = 0;
        virtual void onPrepare() = 0;
        virtual void onFinalizeRender() = 0;
        virtual void onDestroy() = 0;

    public:
        virtual ~Renderer() = default;

        bool init(const std::shared_ptr<Surface<T>> backBuffer)
        {
            this->backBuffer = backBuffer;
            return onInit();
        }

        void prepare()
        {
            onPrepare();
        }

        void finalizeRender()
        {
            onFinalizeRender();
        }

        void destroy()
        {
            onDestroy();
        }

        Surface<T>& getBackBuffer()
        {
            if (!backBuffer)
                throw std::runtime_error("backBuffer is not initialized");

            return *backBuffer;
        }
    };
}

#endif
#endif //RENDERER_H
