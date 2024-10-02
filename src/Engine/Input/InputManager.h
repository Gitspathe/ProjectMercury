#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include <list>
#include <algorithm>
#include <string>
#include <SDL_events.h>
#include <SDL_scancode.h>
#include "../Core/Subsystem.h"
#include "../Core/SubsystemExecOrder.h"

namespace Engine::Input
{
    class InputManager final : public Core::Subsystem, public Core::ISubsystemSDLEventReceiver
    {
    protected:
        std::list<SDL_Scancode> keysDownThisFrame;
        std::list<SDL_Scancode> keysDownLastFrame;
        std::list<SDL_KeyboardEvent> keyboardEvents;

        void handleEvent(SDL_Event& ev) override
        {
            if(ev.type != SDL_KEYDOWN && ev.type != SDL_KEYUP)
                return;

            const SDL_KeyboardEvent keyEv = ev.key;
            switch(keyEv.type) {
                case SDL_KEYDOWN: {
                    const bool found = std::find(
                        keysDownThisFrame.begin(),
                        keysDownThisFrame.end(),
                        keyEv.keysym.scancode
                    ) != keysDownThisFrame.end();
                    if(!found) {
                        keysDownThisFrame.push_back(keyEv.keysym.scancode);
                    }
                } break;
                case SDL_KEYUP: {
                    keysDownThisFrame.remove(keyEv.keysym.scancode);
                } break;
            }
        }

        void onUpdate(float deltaTime) override
        {
#if CLIENT
            keysDownLastFrame.clear();
            std::copy(
                keysDownThisFrame.begin(),
                keysDownThisFrame.end(),
                std::back_inserter(keysDownLastFrame)
            );

            // Handle keyboard events.
            for(const auto ev : keyboardEvents) {
                processKeyboardEvent(ev);
            }

            // Clean-up for next frame.
            keyboardEvents.clear();
#endif
        }

        void processKeyboardEvent(const SDL_KeyboardEvent& ev)
        {
            switch(ev.type) {
                case SDL_KEYDOWN: {
                    const bool found = std::find(
                        keysDownThisFrame.begin(),
                        keysDownThisFrame.end(),
                        ev.keysym.scancode
                    ) != keysDownThisFrame.end();
                    if(!found) {
                        keysDownThisFrame.push_back(ev.keysym.scancode);
                    }
                } break;
                case SDL_KEYUP: {
                    keysDownThisFrame.remove(ev.keysym.scancode);
                } break;
            }
        }
    public:
        std::string getName() override
        {
            return "InputManager";
        }

        uint32_t getOrder() override
        {
            return Core::SubsystemExecOrder::INPUT_MANAGER;
        }

        void onKeyboardEvent(const SDL_KeyboardEvent& ev)
        {
            keyboardEvents.push_back(ev);
        }

        bool isKeyDown(const SDL_Scancode key)
        {
            return std::find(keysDownThisFrame.begin(), keysDownThisFrame.end(), key) != keysDownThisFrame.end();
        }

        bool isKeyUp(const SDL_Scancode key)
        {
            return !isKeyDown(key);
        }

        bool wasKeyPressedThisFrame(const SDL_Scancode key)
        {
            const bool pressedLastFrame = std::find(keysDownLastFrame.begin(), keysDownLastFrame.end(), key) != keysDownLastFrame.end();
            const bool pressedThisFrame = isKeyDown(key);
            return !pressedLastFrame && pressedThisFrame;
        }

        bool wasKeyReleasedThisFrame(const SDL_Scancode key)
        {
            const bool pressedLastFrame = std::find(keysDownLastFrame.begin(), keysDownLastFrame.end(), key) != keysDownLastFrame.end();
            const bool pressedThisFrame = isKeyDown(key);
            return pressedLastFrame && !pressedThisFrame;
        }
    };
}

#endif //INPUTMANAGER_H
