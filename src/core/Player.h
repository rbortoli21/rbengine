//
// Created by bortoli on 07/03/25.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <SDL_render.h>
#include <string>

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

    private:
        int pot = 10;
        int agi = 5;
        int res = 100;
        int currentHealth = 100;

        bool isGrounded = false;
        bool isFacingRight = true;
        bool isRunning = false;

        int totalFrames = 1;
        float animSpeed = 0.002f;
        float animTimer = 0.0f;
        int frameWidth = 0;
        int frameHeight = 0;

        int idleFrames = 18;
        int runningFrames = 24;
        int idleWidth = 1440;
        int runningWidth = 1920;
        int spriteHeight = 80;
    };


RB_ENGINE_END_NS


#endif //PLAYER_H
