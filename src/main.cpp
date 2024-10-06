#include <functional>
#include <chrono>
#include <cstring>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

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
    // Initialize logging functionality.
    log::setLogFile("logs/info.txt");
#if CLIENT
    log::init(&std::cout, &std::cout);
#else
    log::init(nullptr, &std::cout);
#endif

#if SERVER && CLIENT
    log::write(log::serverCmd) << "Running as a host." << log::endl;
#elif SERVER
    log::write(log::serverCmd) << "Running as a server." << log::endl;
#elif CLIENT
    log::write << "Running as a client." << log::endl;
#endif

    /*      SDL Initialization
     * --> Client = audio, video, timer, events
     * --> Web Client = audio, timer, events
     * --> NOT client (headless server) = timer, events, dummy vid driver
     */
    Uint32 sdlInitFlags = SDL_INIT_TIMER | SDL_INIT_EVENTS;
#if CLIENT
    sdlInitFlags |= SDL_INIT_AUDIO;
#ifndef __EMSCRIPTEN__
    sdlInitFlags |= SDL_INIT_VIDEO;
#endif
#else
    char env[] = "SDL_VIDEODRIVER=dummy";
    putenv(env);
#endif

    log::write << "Starting SDL." << log::endl;
    if (SDL_Init(sdlInitFlags) < 0) {
        log::write << "Failed to initialize SDL: " << SDL_GetError() << log::endl;
        SDL_Quit();
        return false;
    }

    game = std::make_shared<TestGame>();
    if(!game->init()) {
        log::write << "Failed to initialize game" << log::endl;
        SDL_Quit();
        return false;
    }
    
    isInit = true;
    return true;
}

bool run()
{
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        if(!game->handleSDLEvent(e))
            return false;
    }

    game->update(0.1f);
    game->render();
    if(game->quitRequested())
        return false;

    return true;
}

// Target FPS
const int TARGET_FPS = 30;
const double FRAME_DURATION = 1000.0 / TARGET_FPS;

std::string input;
int main()
{
    loop = [&] {
        if(!isInit) {
            if(!init())
                return;
        }

        const auto frameStart = std::chrono::high_resolution_clock::now();

        // Run entire game loop.
        if(!run()) {
            running = false;
            return;
        }

        // Framerate cap & timer logic.
        const auto frameEnd = std::chrono::high_resolution_clock::now();
        const std::chrono::duration<double, std::milli> frameTime = frameEnd - frameStart;
        const double timeToWait = FRAME_DURATION - frameTime.count();

#ifndef __EMSCRIPTEN__
        if(timeToWait > 0.0) {
            // If the remaining time is large enough, use a thread sleep.
            if(timeToWait >= 1.0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(timeToWait)));
            }

            // Busy-wait for sub-millisecond precision.
            while(true) {
                auto curTime = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> loopDur = curTime - frameStart;
                if(loopDur.count() >= FRAME_DURATION)
                    break;
            }
        }
#endif
    };

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainLoop, TARGET_FPS, true);
#else
    while(running) mainLoop();
#endif

    game->destroy();
    SDL_Quit();
    return 0;
}
