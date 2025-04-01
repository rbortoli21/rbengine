//
// Created by bortoli on 01/04/25.
//

#include "InputManager.h"

#include <SDL_keyboard.h>

RB_ENGINE_NS
    InputManager::InputManager() {
        currentKeyStates = SDL_GetKeyboardState(nullptr);
        memcpy(previousKeyStates, currentKeyStates, SDL_NUM_SCANCODES);
    }

    void InputManager::update() {
        memcpy(previousKeyStates, currentKeyStates, SDL_NUM_SCANCODES);
        currentKeyStates = SDL_GetKeyboardState(nullptr);
    }

    bool InputManager::isKeyDown(const SDL_Scancode key) const {
        if (key >= SDL_NUM_SCANCODES) return false;
        return currentKeyStates[key] == 1;
    }

    bool InputManager::isActionPressed(GameAction action) const {
        const SDL_Scancode key = actionToKey[static_cast<int>(action)];
        if (key == SDL_SCANCODE_UNKNOWN) return false;
        return isKeyDown(key);
    }

    bool InputManager::isActionTriggered(GameAction action) const {
        const SDL_Scancode key = actionToKey[static_cast<int>(action)];
        if (key == SDL_SCANCODE_UNKNOWN || key >= SDL_NUM_SCANCODES) return false;
        return previousKeyStates[key] == 0 && currentKeyStates[key] == 1;
    }

RB_ENGINE_END_NS

