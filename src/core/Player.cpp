//
// Created by bortoli on 07/03/25.
//

#include "Player.h"

#include <iostream>
#include <ostream>
#include <unistd.h>
#include <SDL2/SDL_image.h>

RB_ENGINE_NS
    Player::Player(SDL_Renderer **renderer)
        : renderer(*renderer), texture(nullptr) {
    }

    bool Player::init() {
        if (!IMG_Init(IMG_INIT_PNG)) {
            std::cerr << "IMG_Init error " << IMG_GetError() << std::endl;
            return false;
        }

        texture = loadTexture("../src/assets/monkey_run.png", renderer);
        if (!texture) {
            return false;
        }

        velocity = 5;
        x = 100;
        y = 100;

        return true;
    }

    void Player::render() {
        handleInput();
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, new SDL_Rect(x, y, 64, 64));
        SDL_RenderPresent(renderer);
    }

    void Player::handleInput() {
        const Uint8 *currentKeyStates = SDL_GetKeyboardState(nullptr);
        if (currentKeyStates[SDL_SCANCODE_UP]) {
            y -= velocity;
        }
        if (currentKeyStates[SDL_SCANCODE_DOWN]) {
            y += velocity;
        }
        if (currentKeyStates[SDL_SCANCODE_LEFT]) {
            x -= velocity;
        }
        if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
            x += velocity;
        }
    }

    SDL_Texture *Player::loadTexture(const char *path, SDL_Renderer *renderer) {
        SDL_Surface *surface = IMG_Load(path);
        if (surface == nullptr) {
            std::cerr << "IMG_Load error " << IMG_GetError() << std::endl;
            return nullptr;
        }

        texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture == nullptr) {
            std::cerr << "SDL_CreateTextureFromSurface error " << SDL_GetError() << std::endl;
        }

        SDL_FreeSurface(surface);
        return texture;
    }


RB_ENGINE_END_NS
