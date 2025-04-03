//
// Created by bortoli on 07/03/25.
//

#include "Player.h"

#include <iostream>
#include <ostream>

#include "Camera.h"
#include "Constants.h"
#include "InputManager.h"
#include "TextureManager.h"
#include "Vector2D.h"

RB_ENGINE_NS
    struct Vector2D;
    class Camera;

    void Player::load(const Vector2D &pos, int w, int h,
                      const std::string &textureId) {
        GameObject::load(pos, w, h, textureId);
        currentHealth = res;

        totalFrames = idleFrames;
        frameWidth = idleWidth / idleFrames;
        frameHeight = spriteHeight;

        std::cout << "Player loaded at (" << pos.x << ", " << pos.y << ")" << std::endl;
    }

    void Player::handleInput() {
        velocity.x = 0;
        isRunning = false;

        if (InputManager::getInstance().isActionPressed(GameAction::MOVE_LEFT)) {
            velocity.x = -200.0f * (agi / 5.0f);
            isFacingRight = false;
            flip = SDL_FLIP_HORIZONTAL;
            if (isGrounded) isRunning = true;
        }
        if (InputManager::getInstance().isActionPressed(GameAction::MOVE_RIGHT)) {
            velocity.x = 200.0f * (agi / 5.0f);
            isFacingRight = true;
            flip = SDL_FLIP_NONE;
            if (isGrounded) isRunning = true;
        }

        if (InputManager::getInstance().isActionTriggered(GameAction::JUMP) && isGrounded) {
            velocity.y = -450.0f;
            isRunning = false;
            isGrounded = false;
            std::cout << "Jump!" << std::endl;
        }
    }

    void Player::update(float dt) {
        handleInput();

        if (isRunning) {
            totalFrames = runningFrames;
            frameWidth = runningWidth / runningFrames;
            frameHeight = spriteHeight;
            animTimer += dt;

            if (animTimer >= animSpeed) {
                animTimer -= animSpeed;
                currentFrame = (currentFrame + 1) % totalFrames;
            }
        } else if (isGrounded) {
            totalFrames = idleFrames;
            frameWidth = idleWidth / idleFrames;
            frameHeight = spriteHeight;
            currentFrame = 0;
            animTimer = 0.0f;
        } else {
            currentFrame = 0;
        }

        if (!isGrounded) {
            acceleration.y = 980.0f; // Pixels/second^2
        } else {
            acceleration.y = 0;
        }

        velocity += acceleration * dt;
        position += velocity * dt;

        const float floorLevel = 600.0f;
        if (position.y + height > floorLevel) {
            position.y = floorLevel - height;
            if (velocity.y > 0) {
                velocity.y = 0;
            }
            isGrounded = true;
        }

        // Update animation frame/row based on state (running, idle, jumping)
        // currentFrame = int(((SDL_GetTicks() / 100) % 6)); // Example animation
    }

    void Player::render(SDL_Renderer *renderer, Camera *camera) {
        const Vector2D screenPos = camera->worldToScreen(position);

        TextureManager::getInstance().drawFrame(
            textureId,
            static_cast<int>(screenPos.x),
            static_cast<int>(screenPos.y),
            frameWidth,
            frameHeight,
            0,
            currentFrame,
            renderer,
            0.0,
            nullptr,
            flip
        );
    }

RB_ENGINE_END_NS
