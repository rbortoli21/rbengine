//
// Created by bortoli on 08/03/25.
//

#include "Game.h"

#include <iostream>
#include <SDL_events.h>
#include <SDL_image.h>
#include <algorithm>

#include "Camera.h"
#include "InputManager.h"
#include "Player.h"
#include "TextureManager.h"
#include "WoodenSentinel.h"

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

        hudFont = TTF_OpenFont("src/assets/fonts/poppins.ttf", 24);
        if (!hudFont) {
            std::cerr << "Failed to load HUD font: " << TTF_GetError() << std::endl;
        }

        smallFont = TTF_OpenFont("src/assets/fonts/poppins.ttf", 16);
        if (!smallFont) {
            std::cerr << "Failed to load small font: " << TTF_GetError() << std::endl;
        }

        running = true;
        std::cout << "SDL Initialized Successfully" << std::endl;

        setup();

        return true;
    }

    void Game::setup() {
        if (!TextureManager::getInstance().load("player", "src/assets/player/idle/idle.png", renderer)) {
            std::cerr << "Failed to load player texture!" << std::endl;
        }

        if (!TextureManager::getInstance().load("player_run", "src/assets/player/idle/run_right.png", renderer)) {
            std::cerr << "Failed to load player run!" << std::endl;
        }

        if (!TextureManager::getInstance().load("arrow", "src/assets/player/arrow.png", renderer)) {
            std::cerr << "Failed to load arrow texture!" << std::endl;
        }

        if (!TextureManager::getInstance().load("background", "src/assets/jungle_background.jpg", renderer)) {
            std::cerr << "Failed to load background texture!" << std::endl;
        }

        if (!TextureManager::getInstance().load("enemy", "src/assets/enemy.png", renderer)) {
            std::cerr << "Failed to load enemy texture!" << std::endl;
        } else {
            std::cout << "Enemy texture loaded successfully!" << std::endl;
            // Try to get texture info
            SDL_Surface* surface = IMG_Load("src/assets/enemy.png");
            if (surface) {
                std::cout << "Enemy PNG dimensions: " << surface->w << "x" << surface->h << std::endl;
                SDL_FreeSurface(surface);
            }
        }

        player = new Player();
        player->load({50, 1000}, 80, 30, "player");
        
        // Initialize UI feedback with renderer
        player->getUIFeedback().init(renderer);

        camera = new Camera();
        camera->setTarget(player->getPositionPtr());

        // Spawn enemies
        spawnEnemies();

        std::cout << "Game setup complete." << std::endl;
    }

    void Game::spawnEnemies() {
        // Spawn Wooden Sentinels in the Ethereal Forest
        enemies.push_back(new WoodenSentinel({400, 1000}));
        enemies.push_back(new WoodenSentinel({800, 1000}));
        enemies.push_back(new WoodenSentinel({1200, 1000}));
        
        std::cout << "Spawned " << enemies.size() << " enemies" << std::endl;
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

    void Game::update(float dt) {
        if (player) {
            player->update(dt);
        }
        if (camera) {
            camera->update(levelWidth, levelHeight);
        }
        
        // Update collision cooldown
        if (playerDamageCooldown > 0) {
            playerDamageCooldown -= dt;
        }
        
        updateEnemies(dt);
        checkCollisions();
        cleanupDeadEnemies();
    }

    void Game::updateEnemies(float dt) const {
        for (auto* enemy : enemies) {
            if (enemy && enemy->isAlive()) {
                enemy->setPlayerPosition(player->getPosition());
                enemy->update(dt);
            }
        }
    }

    void Game::checkCollisions() {
        if (!player) return;
        
        // Check player-enemy collisions
        for (auto* enemy : enemies) {
            if (!enemy || !enemy->isAlive()) continue;
            
            // Simple bounding box collision
            SDL_Rect playerRect = player->getBoundingBox();
            SDL_Rect enemyRect = enemy->getBoundingBox();
            
            if (SDL_HasIntersection(&playerRect, &enemyRect)) {
                if (enemy->isAttacking() && playerDamageCooldown <= 0) {
                    // Player takes damage only if cooldown has expired
                    player->takeDamage(enemy->getAttackDamage());
                    std::cout << "Player hit by enemy! (Damage: " << enemy->getAttackDamage() << ")" << std::endl;
                    playerDamageCooldown = DAMAGE_COOLDOWN_TIME;
                } else if (enemy->isAttacking() && playerDamageCooldown > 0) {
                    // Debug: show when cooldown is active
                    std::cout << "Player collision detected but cooldown active (" << playerDamageCooldown << "s remaining)" << std::endl;
                }
            }
        }
        
        // Check arrow-enemy collisions
        auto& arrows = player->getArrows();
        constexpr int ARROW_DAMAGE = 25;
        for (auto arrowIt = arrows.begin(); arrowIt != arrows.end(); ) {
            SDL_Rect arrowRect = { static_cast<int>(arrowIt->getPosition().x), static_cast<int>(arrowIt->getPosition().y), 32, 8 };
            bool hit = false;
            for (auto* enemy : enemies) {
                if (!enemy || !enemy->isAlive()) continue;
                SDL_Rect enemyRect = enemy->getBoundingBox();
                if (SDL_HasIntersection(&arrowRect, &enemyRect)) {
                    enemy->takeDamage(ARROW_DAMAGE);
                    hit = true;
                    break;
                }
            }
            if (hit) {
                arrowIt = arrows.erase(arrowIt);
            } else {
                ++arrowIt;
            }
        }
    }

    void Game::cleanupDeadEnemies() {
        enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(),
                [](const Enemy* enemy) { 
                    return !enemy || !enemy->isAlive(); 
                }),
            enemies.end()
        );
    }

    void Game::render() const {
        SDL_SetRenderDrawColor(renderer, 100, 149, 237, 255); // Cornflower blue
        SDL_RenderClear(renderer);

        TextureManager::getInstance().draw("background", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, renderer);

        if (player && camera) {
            player->render(renderer, camera);
        }

        renderEnemies();

        renderHUD();

        SDL_RenderPresent(renderer);
    }

    void Game::renderEnemies() const {
        for (auto* enemy : enemies) {
            if (enemy && enemy->isAlive()) {
                enemy->render(renderer, camera);
            }
        }
    }

    void Game::renderHUD() const {
        renderHealthBar();
        renderInventoryInfo();
        renderAttributes();
        renderControls();
    }

    void Game::renderHealthBar() const {
        if (!player || !hudFont) return;

        constexpr SDL_Color white = {255, 255, 255, 255};

        // Health bar background
        SDL_SetRenderDrawColor(renderer, 139, 0, 0, 255);
        const SDL_Rect healthBarBg = {20, 20, 200, 25};
        SDL_RenderFillRect(renderer, &healthBarBg);

        // Health bar fill
        float healthPercent = static_cast<float>(player->getHealth()) / player->getMaxHealth();
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        const SDL_Rect healthBarFill = {20, 20, static_cast<int>(200 * healthPercent), 25};
        SDL_RenderFillRect(renderer, &healthBarFill);

        // Health text
        std::string healthText = "HP: " + std::to_string(player->getHealth()) + "/" + std::to_string(player->getMaxHealth());
        SDL_Surface *healthSurface = TTF_RenderText_Solid(hudFont, healthText.c_str(), white);
        if (healthSurface) {
            SDL_Texture *healthTexture = SDL_CreateTextureFromSurface(renderer, healthSurface);
            const SDL_Rect healthTextRect = {25, 22, healthSurface->w, healthSurface->h};
            SDL_RenderCopy(renderer, healthTexture, nullptr, &healthTextRect);
            SDL_FreeSurface(healthSurface);
            SDL_DestroyTexture(healthTexture);
        }
    }

    void Game::renderInventoryInfo() const {
        if (!player || !hudFont) return;

        constexpr SDL_Color white = {255, 255, 255, 255};
        constexpr SDL_Color yellow = {255, 255, 0, 255};

        // Arrows info
        std::string arrowText = "Arrows: " + std::to_string(player->getArrowsQuantity()) + "/" + 
                               std::to_string(player->getInventory().getArrowCapacity());
        SDL_Surface *arrowSurface = TTF_RenderText_Solid(hudFont, arrowText.c_str(), yellow);
        if (arrowSurface) {
            SDL_Texture *arrowTexture = SDL_CreateTextureFromSurface(renderer, arrowSurface);
            const SDL_Rect arrowTextRect = {20, 55, arrowSurface->w, arrowSurface->h};
            SDL_RenderCopy(renderer, arrowTexture, nullptr, &arrowTextRect);
            SDL_FreeSurface(arrowSurface);
            SDL_DestroyTexture(arrowTexture);
        }

        // Health potions
        std::string potionText = "Potions: " + std::to_string(player->getHealthPotions()) + "/" + 
                                std::to_string(player->getInventory().getMaxHealthPotions());
        SDL_Surface *potionSurface = TTF_RenderText_Solid(hudFont, potionText.c_str(), white);
        if (potionSurface) {
            SDL_Texture *potionTexture = SDL_CreateTextureFromSurface(renderer, potionSurface);
            const SDL_Rect potionTextRect = {20, 85, potionSurface->w, potionSurface->h};
            SDL_RenderCopy(renderer, potionTexture, nullptr, &potionTextRect);
            SDL_FreeSurface(potionSurface);
            SDL_DestroyTexture(potionTexture);
        }

        // Mystic feathers
        std::string featherText = "Mystic Feathers: " + std::to_string(player->getMysticFeathers());
        SDL_Surface *featherSurface = TTF_RenderText_Solid(hudFont, featherText.c_str(), yellow);
        if (featherSurface) {
            SDL_Texture *featherTexture = SDL_CreateTextureFromSurface(renderer, featherSurface);
            const SDL_Rect featherTextRect = {20, 115, featherSurface->w, featherSurface->h};
            SDL_RenderCopy(renderer, featherTexture, nullptr, &featherTextRect);
            SDL_FreeSurface(featherSurface);
            SDL_DestroyTexture(featherTexture);
        }
    }

    void Game::renderAttributes() const {
        if (!player || !hudFont) return;

        constexpr SDL_Color white = {255, 255, 255, 255};
        constexpr SDL_Color blue = {100, 149, 237, 255};

        // Attributes section
        std::string attrText = "Attributes:";
        SDL_Surface *attrSurface = TTF_RenderText_Solid(hudFont, attrText.c_str(), white);
        if (attrSurface) {
            SDL_Texture *attrTexture = SDL_CreateTextureFromSurface(renderer, attrSurface);
            const SDL_Rect attrTextRect = {SCREEN_WIDTH - 200, 20, attrSurface->w, attrSurface->h};
            SDL_RenderCopy(renderer, attrTexture, nullptr, &attrTextRect);
            SDL_FreeSurface(attrSurface);
            SDL_DestroyTexture(attrTexture);
        }

        // Individual attributes
        std::string potText = "POT: " + std::to_string(player->getPot());
        SDL_Surface *potSurface = TTF_RenderText_Solid(hudFont, potText.c_str(), blue);
        if (potSurface) {
            SDL_Texture *potTexture = SDL_CreateTextureFromSurface(renderer, potSurface);
            const SDL_Rect potTextRect = {SCREEN_WIDTH - 200, 50, potSurface->w, potSurface->h};
            SDL_RenderCopy(renderer, potTexture, nullptr, &potTextRect);
            SDL_FreeSurface(potSurface);
            SDL_DestroyTexture(potTexture);
        }

        std::string agiText = "AGI: " + std::to_string(player->getAgi());
        SDL_Surface *agiSurface = TTF_RenderText_Solid(hudFont, agiText.c_str(), blue);
        if (agiSurface) {
            SDL_Texture *agiTexture = SDL_CreateTextureFromSurface(renderer, agiSurface);
            const SDL_Rect agiTextRect = {SCREEN_WIDTH - 200, 80, agiSurface->w, agiSurface->h};
            SDL_RenderCopy(renderer, agiTexture, nullptr, &agiTextRect);
            SDL_FreeSurface(agiSurface);
            SDL_DestroyTexture(agiTexture);
        }

        std::string resText = "RES: " + std::to_string(player->getRes());
        SDL_Surface *resSurface = TTF_RenderText_Solid(hudFont, resText.c_str(), blue);
        if (resSurface) {
            SDL_Texture *resTexture = SDL_CreateTextureFromSurface(renderer, resSurface);
            const SDL_Rect resTextRect = {SCREEN_WIDTH - 200, 110, resSurface->w, resSurface->h};
            SDL_RenderCopy(renderer, resTexture, nullptr, &resTextRect);
            SDL_FreeSurface(resSurface);
            SDL_DestroyTexture(resTexture);
        }
    }

    void Game::renderControls() const {
        if (!smallFont) return;

        constexpr SDL_Color black = {0, 0, 0, 255};

        std::vector<std::string> controls = {
            "Controls:",
            "WASD - Move/Jump/Climb",
            "P - Fire Arrow",
            "H - Use Health Potion",
            "Shift - Dodge",
            "1-5 - Upgrade Abilities"
        };

        int yPos = SCREEN_HEIGHT - 150;
        for (const auto& control : controls) {
            SDL_Surface *controlSurface = TTF_RenderText_Solid(smallFont, control.c_str(), black);
            if (controlSurface) {
                SDL_Texture *controlTexture = SDL_CreateTextureFromSurface(renderer, controlSurface);
                const SDL_Rect controlTextRect = {20, yPos, controlSurface->w, controlSurface->h};
                SDL_RenderCopy(renderer, controlTexture, nullptr, &controlTextRect);
                SDL_FreeSurface(controlSurface);
                SDL_DestroyTexture(controlTexture);
            }
            yPos += 20;
        }
    }

    void Game::clean() {
        std::cout << "Cleaning up game..." << std::endl;

        // Clean up enemies
        for (auto* enemy : enemies) {
            delete enemy;
        }
        enemies.clear();

        delete player;
        player = nullptr;
        delete camera;
        camera = nullptr;

        TextureManager::getInstance().clearTextureMap();

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        renderer = nullptr;
        window = nullptr;

        if (hudFont) {
            TTF_CloseFont(hudFont);
            hudFont = nullptr;
        }
        if (smallFont) {
            TTF_CloseFont(smallFont);
            smallFont = nullptr;
        }

        // Mix_Quit();
        // TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        std::cout << "Cleanup complete." << std::endl;
    }


RB_ENGINE_END_NS
