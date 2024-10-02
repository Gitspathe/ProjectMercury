#include <functional>
#include <chrono>
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

TCPsocket client;
TCPsocket server;
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

    // TEST
    if (SDLNet_Init() == -1) {
        std::cerr << "SDLNet_Init: " << SDLNet_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Create a TCP socket for listening
    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, nullptr, 12345) == -1) {  // Listening on port 12345
        std::cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
        SDLNet_Quit();
        SDL_Quit();
        return -1;
    }
    server = SDLNet_TCP_Open(&ip);
    if (!server) {
        std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
        SDLNet_Quit();
        SDL_Quit();
        return -1;
    }
    std::cout << "Server listening on port 12345" << std::endl;

    Uint32 sdlInitFlags = SDL_INIT_TIMER | SDL_INIT_EVENTS;

    /*      SDL Initialization
     * --> Client = audio, video, timer, events
     * --> Web Client = audio, timer, events
     * --> NOT client (headless server) = timer, events, dummy vid driver
     */
#if CLIENT
    sdlInitFlags |= SDL_INIT_AUDIO;
#ifndef __EMSCRIPTEN__
    sdlInitFlags |= SDL_INIT_VIDEO;
#endif
#else
    putenv("SDL_VIDEODRIVER=dummy");
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

    client = SDLNet_TCP_Accept(server);  // Wait for a connection
    if (client) {
        std::cout << "Client connected!" << std::endl;

        // Receive and echo back data
        char buffer[512];
        int len = SDLNet_TCP_Recv(client, buffer, sizeof(buffer) - 1);
        if (len > 0) {
            buffer[len] = '\0';  // Null-terminate the string
            std::cout << "Received: " << buffer << std::endl;

            // Echo the received message back to the client
            SDLNet_TCP_Send(client, buffer, len);
        }

        // Close the client connection
        SDLNet_TCP_Close(client);
        std::cout << "Client disconnected!" << std::endl;
    }

    return true;
}

// Target FPS
const int TARGET_FPS = 9999;
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
        double timeToWait = FRAME_DURATION - frameTime.count();

#ifdef __EMSCRIPTEN__
        // For Emscripten compatibility, use SDL_Delay.
        if (timeToWait > 0.0) {
            SDL_Delay(static_cast<Uint32>(timeToWait));
        }
#else
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
    emscripten_set_main_loop(mainLoop, 0, true);
#else
    while(running) mainLoop();
#endif

    SDLNet_TCP_Close(server);
    SDLNet_Quit();

    game->destroy();
    SDL_Quit();
    return 0;
}
