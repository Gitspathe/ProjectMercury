#include "InputManager.h"
#include <list>
#include <SDL_events.h>
#include <stdexcept>
#include <algorithm>
#include <ostream>

namespace Input
{
    InputManager* InputManager::instance = nullptr;

    std::list<SDL_Scancode> keysDownThisFrame;
    std::list<SDL_Scancode> keysDownLastFrame;
    std::list<SDL_KeyboardEvent> keyboardEvents;

    InputManager::InputManager() { }

    void InputManager::init()
    {
        if(instance != nullptr) {
            throw std::runtime_error("InputManager already initialized");
        }

        instance = this;
        keyboardEvents = std::list<SDL_KeyboardEvent>();
        keysDownThisFrame = std::list<SDL_Scancode>();
        keysDownLastFrame = std::list<SDL_Scancode>();
    }

    void InputManager::update(float deltaTime)
    {
        keysDownLastFrame.clear();
        std::copy(
            keysDownThisFrame.begin(),
            keysDownThisFrame.end(),
            std::back_inserter(keysDownLastFrame)
        );

        // Handle keyboard events.
        for(const auto ev : keyboardEvents) {
            handleKeyboardEvent(ev);
        }

        // Clean-up for next frame.
        keyboardEvents.clear();
    }

    void InputManager::onKeyboardEvent(const SDL_KeyboardEvent &ev)
    {
        keyboardEvents.push_back(ev);
    }

    bool InputManager::isKeyDown(const SDL_Scancode key) const
    {
        return std::find(keysDownThisFrame.begin(), keysDownThisFrame.end(), key) != keysDownThisFrame.end();
    }

    bool InputManager::isKeyUp(const SDL_Scancode key) const
    {
        return !isKeyDown(key);
    }

    bool InputManager::wasKeyPressedThisFrame(const SDL_Scancode key) const
    {
        bool pressedLastFrame = std::find(keysDownLastFrame.begin(), keysDownLastFrame.end(), key) != keysDownLastFrame.end();
        bool pressedThisFrame = isKeyDown(key);
        return !pressedLastFrame && pressedThisFrame;
    }

    bool InputManager::wasKeyReleasedThisFrame(const SDL_Scancode key) const
    {
        bool pressedLastFrame = std::find(keysDownLastFrame.begin(), keysDownLastFrame.end(), key) != keysDownLastFrame.end();
        bool pressedThisFrame = isKeyDown(key);
        return pressedLastFrame && !pressedThisFrame;
    }

    void InputManager::handleKeyboardEvent(const SDL_KeyboardEvent& ev)
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
}
