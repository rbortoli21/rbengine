//
// Created by bortoli on 07/03/25.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <SDL_render.h>

#include "EngineDefines.h"

RB_ENGINE_NS
    enum class PlayerState {
        IDLE,
        IDLE_LEFT,
        RUNNING_LEFT,
        RUNNING_RIGHT,
        TURN_AROUND_LEFT,
        TURN_AROUND_RIGHT
    };

    class Player {
    public:
        explicit Player(SDL_Renderer **renderer);

        ~Player() = default;

        bool init();

        void update(int currentTime, float deltaTime);

        void render() const;

    private:
        void changeState(const Uint8 *keyStates);

        void updateAnimationFrame(int currentTime);

        static SDL_Texture *loadTexture(const char *path, SDL_Renderer *renderer);

        PlayerState currentState = PlayerState::IDLE;
        PlayerState previousState = currentState;
        SDL_Renderer *renderer;
        SDL_Texture *idleTexture = nullptr,
                *idleLeftTexture = nullptr,
                *runRightTexture = nullptr,
                *runLeftTexture = nullptr,
                *runTurnAroundRightTexture = nullptr;
        float x = 0, y = 0, velocity = 0;
        int totalFrames = 18, frameWidth = 80, frameHeight = 80, currentFrame = 0;
    };

RB_ENGINE_END_NS


#endif //PLAYER_H
