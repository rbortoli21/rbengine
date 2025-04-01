//
// Created by bortoli on 08/03/25.
//

#include "Game.h"

#include <iostream>
#include <SDL_events.h>
#include <SDL_image.h>

#include "Camera.h"
#include "InputManager.h"
#include "Player.h"
#include "TextureManager.h"

RB_ENGINE_NS
    bool Game::init(const char *title, const int xpos, const int ypos, const int width, const int height,
                    const bool fullscreen) {
        int flags = 0;
        if (fullscreen) {
            flags = SDL_WINDOW_FULLSCREEN;
        }

        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
            std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError()
                    << std::endl;
            return false;
        }

        if (constexpr int imgFlags = IMG_INIT_PNG; !(IMG_Init(imgFlags) & imgFlags)) {
            std::cerr << "SDL_image could not initialize! IMG_Error: "
                    << IMG_GetError() << std::endl;
        }

        // if (TTF_Init() == -1) {
        //     std::cerr << "SDL_ttf could not initialize! TTF_Error: "
        //             << TTF_GetError() << std::endl;
        // }
        //
        // if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        //     std::cerr << "SDL_mixer could not initialize! Mix_Error: "
        //             << Mix_GetError() << std::endl;
        // }


        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags | SDL_WINDOW_SHOWN);
        if (window == nullptr) {
            std::cerr << "Window could not be created! SDL_Error: "
                    << SDL_GetError() << std::endl;
            return false;
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (renderer == nullptr) {
            std::cerr << "Renderer could not be created! SDL Error: "
                    << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            return false;
        }

        SDL_SetRenderDrawColor(renderer, 100, 149, 237, 255);

        running = true;
        std::cout << "SDL Initialized Successfully" << std::endl;

        setup();

        return true;
    }

    void Game::setup() {
        if (!TextureManager::getInstance().load("player", "../src/assets/player/idle/idle.png", renderer)) {
            std::cerr << "Failed to load player texture!" << std::endl;
        }

        player = new Player();
        player->load({100, 100}, 64, 64, "player");

        camera = new Camera();
        camera->setTarget(player->getPositionPtr());

        std::cout << "Game setup complete." << std::endl;
    }


    void Game::handleEvents() {
        InputManager::getInstance().update();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        running = false;
                    }
                    break;
                default:
                    break;
            }
        }
    }

    void Game::update(const float dt) const {
        if (player) {
            player->update(dt);
        }
        if (camera) {
            camera->update(levelWidth, levelHeight);
        }
    }

    void Game::render() const {
        SDL_SetRenderDrawColor(renderer, 100, 149, 237, 255); // Cornflower blue
        SDL_RenderClear(renderer);

        if (player && camera) {
            player->render(renderer, camera);
        }

        SDL_RenderPresent(renderer);
    }

    void Game::clean() {
        std::cout << "Cleaning up game..." << std::endl;

        delete player;
        player = nullptr;
        delete camera;
        camera = nullptr;

        TextureManager::getInstance().clearTextureMap();

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        renderer = nullptr;
        window = nullptr;

        // Mix_Quit();
        // TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        std::cout << "Cleanup complete." << std::endl;
    }


RB_ENGINE_END_NS
