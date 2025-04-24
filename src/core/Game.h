//
// Created by bortoli on 08/03/25.
//
#pragma once
#ifndef GAME_H
#define GAME_H
#include <SDL_render.h>
#include <SDL_ttf.h>

#include "Constants.h"
#include "EngineDefines.h"

RB_ENGINE_NS
    class Camera;
    class Player;

    class Game {
    public:
        static Game &getInstance() {
            static Game instance;
            return instance;
        }

        bool init(const char *title, int xpos, int ypos, int width, int height,
                  bool fullscreen);

        void setup();

        void handleEvents();

        void update(float dt) const;

        void render() const;

        void clean();

        [[nodiscard]] bool isRunning() const { return running; }
        void quit() { running = false; }

        [[nodiscard]] SDL_Renderer *getRenderer() const { return renderer; }

    private:
        Game() = default;

        ~Game() = default;

        Game(const Game &) = delete;

        Game &operator=(const Game &) = delete;

        TTF_Font *hudFont = nullptr;
        SDL_Window *window = nullptr;
        SDL_Renderer *renderer = nullptr;
        bool running = false;
        Uint32 frameStart{};
        float deltaTime = 0.0f;

        Player *player = nullptr;
        Camera *camera = nullptr;

        int levelWidth = 2000;
        int levelHeight = SCREEN_HEIGHT;
    };

RB_ENGINE_END_NS


#endif //GAME_H
