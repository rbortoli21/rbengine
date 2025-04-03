#include <iostream>
#include <ostream>
#include <SDL_stdinc.h>
#include <SDL_timer.h>
#include <SDL_video.h>

#include "core/Constants.h"
#include "core/Game.h"

int main() {
    float deltaTime = 0.0f;

    std::cout << "Starting Game..." << std::endl;

    rbengine::Game &game = rbengine::Game::getInstance();

    if (!game.init(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,
                   SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,
                   false)) {
        std::cerr << "Game initialization failed!" << std::endl;
        return -1;
    }

    while (game.isRunning()) {
        const Uint32 frameStart = SDL_GetTicks();

        game.handleEvents();
        game.update(deltaTime); // Pass delta time to update logic
        game.render();

        // Frame limiting and delta time calculation
        int frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < (1000 / TARGET_FPS)) {
            SDL_Delay((1000 / TARGET_FPS) - frameTime);
        }

        // Calculate actual delta time for the next frame's update
        // Use a cap (like TARGET_DELTATIME * 2) to prevent huge jumps if debugging/stalling
        deltaTime = (SDL_GetTicks() - frameStart) / 1000.0f;
        deltaTime = (deltaTime > TARGET_DELTATIME * 2.0f) ? TARGET_DELTATIME * 2.0f : deltaTime;
        if (deltaTime <= 0) {
            // Ensure delta time is positive
            deltaTime = TARGET_DELTATIME;
        }
    }

    std::cout << "Exiting main loop." << std::endl;
    game.clean();

    return 0;
}
