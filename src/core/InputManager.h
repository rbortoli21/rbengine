//
// Created by bortoli on 01/04/25.
//

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include <SDL_scancode.h>

#include "EngineDefines.h"


RB_ENGINE_NS
    enum class GameAction { 
        MOVE_LEFT, 
        MOVE_RIGHT, 
        JUMP, 
        DODGE, 
        CLIMB,
        AIM, 
        FIRE,
        USE_HEALTH_POTION,
        UPGRADE_ABILITY_1,
        UPGRADE_ABILITY_2,
        UPGRADE_ABILITY_3,
        UPGRADE_ABILITY_4,
        UPGRADE_ABILITY_5
    };

    class InputManager {
    public:
        static InputManager &getInstance() {
            static InputManager instance;
            return instance;
        }

        void update();

        bool isKeyDown(SDL_Scancode key) const;

        bool isActionPressed(GameAction action) const;

        bool isActionTriggered(GameAction action) const;

    private:
        InputManager();

        ~InputManager() = default;

        InputManager(const InputManager &) = delete;

        InputManager &operator=(const InputManager &) = delete;

        const Uint8 *currentKeyStates;
        Uint8 previousKeyStates[SDL_NUM_SCANCODES];

        SDL_Scancode actionToKey[13] = {
            SDL_SCANCODE_A, // MOVE_LEFT
            SDL_SCANCODE_D, // MOVE_RIGHT
            SDL_SCANCODE_SPACE, // JUMP
            SDL_SCANCODE_LSHIFT, // DODGE
            SDL_SCANCODE_W, // CLIMB
            SDL_SCANCODE_UNKNOWN, // AIM
            SDL_SCANCODE_P, // FIRE
            SDL_SCANCODE_H, // USE_HEALTH_POTION
            SDL_SCANCODE_1, // UPGRADE_ABILITY_1
            SDL_SCANCODE_2, // UPGRADE_ABILITY_2
            SDL_SCANCODE_3, // UPGRADE_ABILITY_3
            SDL_SCANCODE_4, // UPGRADE_ABILITY_4
            SDL_SCANCODE_5  // UPGRADE_ABILITY_5
        };
    };

RB_ENGINE_END_NS


#endif //INPUTMANAGER_H
