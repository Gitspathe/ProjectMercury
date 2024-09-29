#include <GL/gl.h>
#include <SDL2/SDL.h>
#include "OpenGLRenderer.h"
#include "Surface.h"

namespace Render
{
    template<>
    void OpenGLRenderer<Common::ColorRGB>::updateTexture()
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->backBuffer->getWidth(), this->backBuffer->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, textureData.data());
    }

    template<>
    void OpenGLRenderer<Common::ColorRGBA>::updateTexture()
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->backBuffer->getWidth(), this->backBuffer->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data());
    }

    template class OpenGLRenderer<Common::ColorRGB>;
    template class OpenGLRenderer<Common::ColorRGBA>;
}
