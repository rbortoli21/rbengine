//
// Created by bortoli on 07/03/25.
//

#include "Player.h"

#include <iostream>
#include <ostream>
#include <unistd.h>
#include <SDL2/SDL_image.h>

RB_ENGINE_NS
    constexpr int PLAYER_WIDTH = 24;
    constexpr int PLAYER_HEIGHT = 64;

    Player::Player(SDL_Renderer **renderer)
        : renderer(*renderer) {
    }

    bool Player::init() {
        if (!IMG_Init(IMG_INIT_PNG)) {
            std::cerr << "IMG_Init error " << IMG_GetError() << std::endl;
            return false;
        }

        idleTexture = loadTexture("../src/assets/player/idle/idle.png", renderer);
        idleLeftTexture = loadTexture("../src/assets/player/idle/idle_left.png", renderer);
        runLeftTexture = loadTexture("../src/assets/player/idle/run_left.png", renderer);
        runRightTexture = loadTexture("../src/assets/player/idle/run_right.png", renderer);
        runTurnAroundRightTexture = loadTexture("../src/assets/player/idle/run_turnaround_left.png", renderer);

        velocity = 130.0f;
        x = 100.0f;
        y = 100.0f;

        return true;
    }

    void Player::update(const int currentTime, const float deltaTime) {
        const Uint8 *currentKeyStates = SDL_GetKeyboardState(nullptr);
        changeState(currentKeyStates);

        const float velocityWithDeltaTime = velocity * deltaTime;

        if (currentKeyStates[SDL_SCANCODE_UP]) y -= velocityWithDeltaTime;
        if (currentKeyStates[SDL_SCANCODE_DOWN]) y += velocityWithDeltaTime;
        if (currentKeyStates[SDL_SCANCODE_LEFT]) x -= velocityWithDeltaTime;
        if (currentKeyStates[SDL_SCANCODE_RIGHT]) x += velocityWithDeltaTime;

        updateAnimationFrame(currentTime);
    }

    void Player::render() const {
        SDL_Texture *currentTexture = nullptr;

        switch (currentState) {
            case PlayerState::IDLE:
                currentTexture = idleTexture;
                break;
            case PlayerState::IDLE_LEFT:
                currentTexture = idleLeftTexture;
                break;
            case PlayerState::RUNNING_LEFT:
                currentTexture = runLeftTexture;
                break;
            case PlayerState::RUNNING_RIGHT:
                currentTexture = runRightTexture;
                break;
            case PlayerState::TURN_AROUND_RIGHT:
                currentTexture = runTurnAroundRightTexture;
                break;
            case PlayerState::TURN_AROUND_LEFT:
                currentTexture = runTurnAroundRightTexture;
                break;
        }

        const SDL_Rect srcRect = {currentFrame * frameWidth, 0, frameWidth, frameHeight};
        const SDL_Rect dstRect = {static_cast<int>(x), static_cast<int>(y), frameWidth, frameHeight};

        SDL_RenderCopy(renderer, currentTexture, &srcRect, &dstRect);
    }

    void Player::changeState(const Uint8 *keyStates) {
        PlayerState newState;

        const bool leftPressed = keyStates[SDL_SCANCODE_LEFT];
        const bool rightPressed = keyStates[SDL_SCANCODE_RIGHT];

        if (leftPressed && rightPressed)
            newState = PlayerState::IDLE;
        else if (leftPressed && previousState == PlayerState::RUNNING_RIGHT)
            newState = PlayerState::TURN_AROUND_LEFT;
        else if (rightPressed && previousState == PlayerState::RUNNING_LEFT)
            newState = PlayerState::TURN_AROUND_RIGHT;
        else if (leftPressed)
            newState = PlayerState::RUNNING_LEFT;
        else if (rightPressed)
            newState = PlayerState::RUNNING_RIGHT;
        else {
            if (previousState == PlayerState::RUNNING_LEFT || previousState == PlayerState::IDLE_LEFT)
                newState = PlayerState::IDLE_LEFT;
            else
                newState = PlayerState::IDLE;
        }

        if (newState != currentState) {
            switch (newState) {
                case PlayerState::IDLE:
                case PlayerState::IDLE_LEFT:
                    totalFrames = 18;
                    frameWidth = 1440 / totalFrames;
                    frameHeight = 80;
                    break;
                case PlayerState::RUNNING_LEFT:
                case PlayerState::RUNNING_RIGHT:
                    totalFrames = 24;
                    frameWidth = 1920 / totalFrames;
                    frameHeight = 80;
                    break;
                case PlayerState::TURN_AROUND_LEFT:
                case PlayerState::TURN_AROUND_RIGHT:
                    totalFrames = 5;
                    frameWidth = 400 / totalFrames;
                    frameHeight = 64;
                    break;
            }

            previousState = currentState;
            currentState = newState;
        }
    }

    void Player::updateAnimationFrame(const int currentTime) {
        currentFrame = (currentTime / 50) % totalFrames;
    }

    SDL_Texture *Player::loadTexture(const char *path, SDL_Renderer *renderer) {
        SDL_Surface *surface = IMG_Load(path);
        if (surface == nullptr) {
            std::cerr << "IMG_Load error " << IMG_GetError() << std::endl;
            return nullptr;
        }

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture == nullptr) {
            std::cerr << "SDL_CreateTextureFromSurface error " << SDL_GetError() << std::endl;
        }

        SDL_FreeSurface(surface);
        return texture;
    }


RB_ENGINE_END_NS
