#ifndef RENDERER_H
#define RENDERER_H
#if CLIENT
#include <memory>
#include "Surface.h"
#include "Screen.h"

namespace Engine::Render
{
    template<typename T>
    class Renderer
    {
    protected:
        std::unique_ptr<Surface<T>> backBuffer;

        virtual void onInit() = 0;
        virtual void onPrepare() = 0;
        virtual void onFinalizeRender() = 0;
        virtual void onDestroy() = 0;

    public:
        virtual ~Renderer() = default;

        template<typename TRenderer>
        static std::shared_ptr<TRenderer> create()
        {
            static_assert(std::is_base_of_v<Renderer, TRenderer>, "T must derive from Renderer");
            return std::make_unique<TRenderer>();
        }

        void init(const Screen& screen)
        {
            backBuffer = std::make_unique<Surface<T>>(screen.getWidth(), screen.getHeight());
            onInit();
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
