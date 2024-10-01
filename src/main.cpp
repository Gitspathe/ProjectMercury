#include <functional>
#include <chrono>
#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#include <thread>
#endif

#include "Engine/Input/InputManager.h"
#include "Game/TestGame.h"

bool running = true;
bool isInit = false;
std::function<void()> loop;
Engine::Input::InputManager* inputManager;
std::shared_ptr<TestGame> game;

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void mainLoop()
{
    loop();
}

bool init()
{
    Uint32 sdlInitFlags = SDL_INIT_TIMER | SDL_INIT_EVENTS;

#if CLIENT
    sdlInitFlags |= SDL_INIT_AUDIO;
#ifndef __EMSCRIPTEN__
    sdlInitFlags |= SDL_INIT_VIDEO;
#endif
#endif

    if (SDL_Init(sdlInitFlags) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    inputManager = new Engine::Input::InputManager();
    game = std::make_shared<TestGame>();
    if(!game->init()) {
        std::cerr << "Failed to initialize game" << std::endl;
        SDL_Quit();
        return false;
    }

    isInit = true;
    return true;
}

void run()
{
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        game->handleSDLEvent(e);
        switch(e.type) {
            case SDL_QUIT: {
                running = false;
                break;
            }
            case SDL_MOUSEMOTION: {
                int x, y;
                SDL_GetMouseState(&x, &y);
                break;
            }
            case SDL_KEYUP:
            case SDL_KEYDOWN: {
                Engine::Input::InputManager::onKeyboardEvent(e.key);
            }
            default:
                break;
        }
    }

    Engine::Input::InputManager::update(1.0f);
    game->update(0.1f);
    game->render();
}

// Target FPS
const int TARGET_FPS = 9999;
const double FRAME_DURATION = 1000.0 / TARGET_FPS;

int main()
{
    loop = [&] {
        if(!isInit) {
            if(!init())
                return;
        }

        const auto frameStart = std::chrono::high_resolution_clock::now();

        // Run entire game loop.
        run();

        // Framerate cap & timer logic.
        const auto frameEnd = std::chrono::high_resolution_clock::now();
        const std::chrono::duration<double, std::milli> frameTime = frameEnd - frameStart;
        double timeToWait = FRAME_DURATION - frameTime.count();

#ifdef __EMSCRIPTEN__
        // For Emscripten compatibility, use SDL_Delay.
        if (timeToWait > 0.0) {
            SDL_Delay(static_cast<Uint32>(timeToWait));
        }
#else
        if(timeToWait > 0.0) {
            // If the remaining time is large enough, use a sleep_for.
            if(timeToWait >= 1.0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(timeToWait)));
            }

            // Busy-wait for sub-millisecond precision.
            while(true) {
                auto curTime = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> loopDur = curTime - frameStart;
                if (loopDur.count() >= FRAME_DURATION)
                    break;
            }
        }
#endif
    };

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainLoop, 0, true);
#else
    while(running) mainLoop();
#endif

    game->destroy();
    SDL_Quit();
    return 0;
}
