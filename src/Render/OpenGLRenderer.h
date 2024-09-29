#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H
#include <vector>
#include "Renderer.h"

namespace Common {
    struct ColorRGB;
}

namespace Render {
    class Screen;

    class OpenGLRenderer : public Renderer
    {
    protected:
        float renderScale = 1.0f;

        void on_init() override;
        void on_update() override;
        void on_destroy() override;

    public:
        void setRenderScale(float val);
    };
}


#endif //OPENGLRENDERER_H
