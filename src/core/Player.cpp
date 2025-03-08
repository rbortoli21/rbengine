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
        : renderer(*renderer), texture(nullptr), rect(0, 0, 0, 0) {
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

        rect.x = 100;
        rect.y = 100;
        rect.w = 64;
        rect.h = 64;

        return true;
    }

    void Player::render() const {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
        SDL_RenderPresent(renderer);
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
