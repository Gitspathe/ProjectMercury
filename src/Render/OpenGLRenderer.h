#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H
#include <vector>
#include "Renderer.h"
#include "../Common/Color3.h"

namespace Render {
    using namespace Common;

    class OpenGLRenderer : public Renderer
    {
    public:
        void setRenderScale(float val);

    protected:
        float renderScale = 1.0f;

        void assignData(std::vector<Color3>& data, Screen* screen);
        virtual void on_init() override;
        virtual void on_update() override;
        virtual void on_destroy() override;
    };
}


#endif //OPENGLRENDERER_H
