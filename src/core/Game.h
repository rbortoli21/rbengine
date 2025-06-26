//
// Created by bortoli on 08/03/25.
//
#pragma once
#ifndef GAME_H
#define GAME_H
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <vector>

#include "Constants.h"
#include "EngineDefines.h"

RB_ENGINE_NS
    class Camera;
    class Player;
    class Enemy;

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

        void update(float dt);

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

        void renderHUD() const;
        void renderHealthBar() const;
        void renderInventoryInfo() const;
        void renderAttributes() const;
        void renderControls() const;
        
        // Enemy management
        void spawnEnemies();
        void updateEnemies(float dt) const;
        void renderEnemies() const;
        void checkCollisions();
        void cleanupDeadEnemies();

        TTF_Font *hudFont = nullptr;
        TTF_Font *smallFont = nullptr;
        SDL_Window *window = nullptr;
        SDL_Renderer *renderer = nullptr;
        bool running = false;
        Uint32 frameStart{};
        float deltaTime = 0.0f;

        Player *player = nullptr;
        Camera *camera = nullptr;
        std::vector<Enemy*> enemies;

        // Collision cooldown to prevent excessive damage
        float playerDamageCooldown = 0.0f;
        static constexpr float DAMAGE_COOLDOWN_TIME = 1.0f;

        int levelWidth = 2000;
        int levelHeight = SCREEN_HEIGHT;
    };

RB_ENGINE_END_NS


#endif //GAME_H
