#ifndef TESTGAME_H
#define TESTGAME_H
#include "../Engine/Core/Game.h"
#include "../Engine/Render/OpenGLRenderer.h"
#include "../Engine/Render/Primitives/RectGraphic.h"

class TestGame final : public Engine::Core::Game
{
private:
    std::unique_ptr<Engine::Render::OpenGLRenderer> renderer = nullptr;

protected:
    void setup() override
    {
        renderer = Engine::Render::OpenGLRenderer::create();
        screen = Engine::Render::Screen::create(512,256);
        renderer->setRenderScale(2.0f);
        renderer->init(*screen);
    }

    void onInit() override
    {

    }

    void onUpdate(float deltaTime) override
    {
        std::cout << "Update from TestGame" << std::endl;
    }

    void onRender() override
    {
        renderer->getBackBuffer().clear(Engine::Common::ColorRGB::Black);

        const int size = 8;
        const int w = screen->getWidth();
        const int h = screen->getHeight();
        for(size_t i = 0; i < 100000; i++) {
            auto rectG = Engine::Render::Primitives::RectGraphic(
                Engine::Common::RectF((rand() % w) - size / 2, (rand() % h) - size / 2, size, size),
                Engine::Common::ColorRGB(rand() % 255, rand() % 255, rand() % 255), rand() % 5
            );
            rectG.draw(renderer->getBackBuffer());
        }

        renderer->update();
    }

    void onDestroy() override
    {

    }
};

#endif //TESTGAME_H
