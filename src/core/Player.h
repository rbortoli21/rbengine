//
// Created by bortoli on 07/03/25.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <SDL_render.h>
#include <string>
#include <vector>

#include "Arrow.h"
#include "EngineDefines.h"
#include "GameObject.h"

RB_ENGINE_NS
    struct Vector2D;
    class Camera;

    class Player final : public GameObject {
    public:
        Player() = default;

        ~Player() override = default;

        void load(const Vector2D &pos, int w, int h,
                  const std::string &textureId) override;

        void update(float dt) override;

        void render(SDL_Renderer *renderer, Camera *camera) override;

        void handleInput();

        [[nodiscard]] int getArrowsQuantity() const {
            return arrowsQuantity;
        }

        [[nodiscard]] int getHealth() const {
            return currentHealth;
        }

    private:
        int pot = 10;
        int agi = 7;
        int res = 100;
        int currentHealth = 100;
        int arrowsQuantity = 20;

        bool isGrounded = false;
        bool isFacingRight = true;
        bool isRunning = false;

        int totalFrames = 1;
        float animTimer = 0.0f;
        int frameWidth = 0;
        int frameHeight = 0;

        float idleAnimSpeed = 0.08f;
        int idleFrames = 18;
        int idleWidth = 1440;

        int runningFrames = 24;
        float runAnimSpeed = 0.002f;
        int runningWidth = 1920;

        int spriteHeight = 80;

        std::vector<Arrow> arrows;
        float arrowSpeed = 1100.0f;
        std::string arrowTextureId = "arrow";
    };


RB_ENGINE_END_NS


#endif //PLAYER_H
