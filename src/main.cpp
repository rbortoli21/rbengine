#include <iostream>
#include <ostream>
#include <SDL_stdinc.h>
#include <SDL_timer.h>
#include <SDL_video.h>

#include "core/Constants.h"
#include "core/Game.h"

enum class MenuOption { START, EXIT };

bool showMenu(SDL_Renderer *renderer) {
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }

    TTF_Font *font = TTF_OpenFont("../src/assets/fonts/poppins.ttf", 48);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return false;
    }

    const char *options[] = {"Iniciar Jogo", "Sair"};
    int selected = 0;
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) return false;
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        selected = (selected + 1) % 2;
                        break;
                    case SDLK_DOWN:
                        selected = (selected + 1) % 2;
                        break;
                    case SDLK_RETURN:
                    case SDLK_KP_ENTER:
                        if (selected == 0) return true; // Iniciar Jogo
                        if (selected == 1) return false; // Sair
                        break;
                }
            }
        }

        // Render menu
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < 2; ++i) {
            SDL_Color color = (i == selected) ? SDL_Color{255, 0, 0, 255} : SDL_Color{255, 255, 255, 255};
            SDL_Surface *surf = TTF_RenderUTF8_Solid(font, options[i], color);
            SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);

            int x = (SCREEN_WIDTH - surf->w) / 2;
            int y = 250 + i * 80;
            SDL_Rect dst = {x, y, surf->w, surf->h};
            SDL_RenderCopy(renderer, tex, nullptr, &dst);

            SDL_FreeSurface(surf);
            SDL_DestroyTexture(tex);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    TTF_CloseFont(font);
    TTF_Quit();
    return false;
}

int main() {
    float deltaTime = 0.0f;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    if (!showMenu(renderer)) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }

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
