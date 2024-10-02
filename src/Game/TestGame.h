#ifndef TESTGAME_H
#define TESTGAME_H
#include <iostream>
#include "../Engine/Core/Game.h"
#include "../Engine/Input/InputManager.h"

#if CLIENT
#include "../Engine/GUI/ImGUIManager.h"
#include "../Engine/Render/OpenGLRenderer.h"
#include "../Engine/Render/Primitives/RectGraphic.h"
#include "../Engine/Render/Screen.h"
#else
#if SERVER
#include "../Engine/Core/ServerCommandManager.h"
#endif
#endif

#include "../Engine.h"

class TestGame final : public Core::Game
{
private:
#if CLIENT
    std::unique_ptr<Render::OpenGLRenderer> renderer;
#endif

protected:
#if CLIENT
    std::shared_ptr<Render::Surface<ColorRGB>> backBuffer;
    std::vector<Render::Primitives::RectGraphic<ColorRGB>*> graphics;
#endif

    bool setup() override
    {
#if CLIENT
        screen = std::make_unique<Render::Screen>(720,384);
        log::write << "Created screen (W: " << screen->getWidth() << " H: " << screen->getHeight() << ")." << log::endl;

        backBuffer = std::make_shared<Render::Surface<ColorRGB>>(*screen);
        log::write << "Created back buffer." << log::endl;

        renderer = std::make_unique<Render::OpenGLRenderer>();
        log::write << "Created OpenGL renderer." << log::endl;

        renderer->setRenderScale(2.0f);
        log::write << "Render scale set: " << renderer->getRenderScale() << "." << log::endl;

        if(!renderer->init(backBuffer)) {
            log::write << "Failed to initialize renderer." << log::endl;
            return false;
        }
        log::write << "Initialized renderer." << log::endl;

#endif

        setupSubsystems();
        return true;
    }

    void setupSubsystems()
    {
        addSubsystem(std::make_shared<Input::InputManager>());

#if CLIENT && DEV_MODE
        addSubsystem(std::make_shared<GUI::ImGUIManager>());
#endif

#if SERVER && !CLIENT
        addSubsystem(std::make_shared<Core::ServerCommandManager>());
#endif
    }

    bool onInit() override
    {
#if CLIENT
        const int size = 64;
        const int w = backBuffer->getWidth();
        const int h = backBuffer->getHeight();
        for(size_t i = 0; i < 10000; i++) {
            auto rectG = new Render::Primitives::RectGraphic(
                RectF((rand() % w) - size / 2, (rand() % h) - size / 2, size, size),
                ColorRGB(rand() % 255, rand() % 255, rand() % 255), BlendMode::COLOR
            );
            graphics.push_back(rectG);
        }
#endif
        return true;
    }

    void onUpdate(float deltaTime) override {}

    void onRender() override
    {
#if CLIENT
        renderer->getBackBuffer().clear(ColorRGB::Black);

        for(const auto g : graphics) {
            g->draw(*backBuffer);
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
#if CLIENT
        renderer->destroy();
#endif
        SDL_Quit();
    }
};

#endif //TESTGAME_H
