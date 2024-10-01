#ifndef TESTGAME_H
#define TESTGAME_H
#include <iostream>
#include "../Engine/Core/Game.h"
#include "../Engine/GUI/ImGUIManager.h"
#include "../Engine/Render/OpenGLRenderer.h"
#include "../Engine/Render/Primitives/RectGraphic.h"

class TestGame final : public Engine::Core::Game
{
private:
#if CLIENT
    std::unique_ptr<Engine::Render::OpenGLRenderer> renderer = nullptr;
#endif

protected:
    bool setup() override
    {
#if CLIENT
        renderer = Engine::Render::OpenGLRenderer::create();
        screen = Engine::Render::Screen::create(720,384);
        renderer->setRenderScale(2.0f);
        if(!renderer->init(*screen))
            return false;

#endif

#if DEV_MODE && CLIENT
        addSubSystem<Engine::GUI::ImGUIManager>(std::make_shared<Engine::GUI::ImGUIManager>());
#endif

        return true;
    }

    std::vector<Engine::Render::Primitives::RectGraphic<Engine::Common::ColorRGBA>*> graphics;
    bool onInit() override
    {
        const int size = 64;
        const int w = screen->getWidth();
        const int h = screen->getHeight();
        for(size_t i = 0; i < 1000; i++) {
            auto rectG = new Engine::Render::Primitives::RectGraphic(
                Engine::Common::RectF((rand() % w) - size / 2, (rand() % h) - size / 2, size, size),
                Engine::Common::ColorRGBA(rand() % 255, rand() % 255, rand() % 255, 20), Engine::Common::BlendMode::ADD
            );
            graphics.push_back(rectG);
        }
        return true;
    }

    void onUpdate(float deltaTime) override
    {

    }

    void onRender() override
    {
#if CLIENT
        renderer->getBackBuffer().clear(Engine::Common::ColorRGB::Black);

        for(auto g : graphics) {
            g->draw(renderer->getBackBuffer());
        }

        renderer->prepare();
#endif
    }

    void onFinalizeRender() override
    {
#if CLIENT
        renderer->finalizeRender();
#endif
    }

    void onDestroy() override
    {

    }
};

#endif //TESTGAME_H
