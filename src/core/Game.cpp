//
// Created by bortoli on 08/03/25.
//

#include "Game.h"

#include <SDL_events.h>
#include <SDL_image.h>
#include <SDL_timer.h>

RB_ENGINE_NS
    constexpr int SCREEN_WIDTH = 800;
    constexpr int SCREEN_HEIGHT = 600;
    constexpr int TOTAL_FRAMES = 18;
    constexpr int FRAME_DELAY = 100;

    Game::Game(): renderer(nullptr) {
        window = new Window("Application Window", SCREEN_WIDTH, SCREEN_HEIGHT, &renderer);
        player = new Player(&renderer);
    }

    bool Game::init() const {
        if (!window->isRunning()) {
            return false;
        }

        if (!player->init()) {
            return false;
        }

        return true;
    }

    void Game::run() const {
        Uint32 lastTime = SDL_GetTicks();

        while (window->isRunning()) {
            window->pollEvents();
            window->clear();

            const Uint32 currentTime = SDL_GetTicks();
            const float deltaTime = (currentTime - lastTime) / 1000.0f;
            lastTime = currentTime;

            SDL_RenderClear(renderer);

            player->update(currentTime, deltaTime);
            player->render();

            SDL_RenderPresent(renderer);
            SDL_Delay(16);
        }

        IMG_Quit();
        SDL_Quit();
    }


RB_ENGINE_END_NS
