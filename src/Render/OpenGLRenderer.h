#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H
#include "Renderer.h"

namespace Render {
    class OpenGLRenderer : public Renderer
    {
    private:
        virtual void on_init() override;
        virtual void on_update() override;
        virtual void on_destroy() override;
    };
}


#endif //OPENGLRENDERER_H
