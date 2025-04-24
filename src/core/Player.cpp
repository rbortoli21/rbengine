#include "Player.h"

#include <algorithm>
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

    void Player::load(const Vector2D &pos, int w, int h, const std::string &textureId) {
        GameObject::load(pos, w, h, textureId);
        currentHealth = res;

        frameWidth = idleWidth / idleFrames;
        frameHeight = spriteHeight;

        totalFrames = idleFrames;

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

        if (InputManager::getInstance().isActionTriggered(GameAction::FIRE)) {
            Vector2D arrowDirection = isFacingRight ? Vector2D(1, 0) : Vector2D(-1, 0);

            Vector2D arrowPosition = position;
            arrowPosition.x += isFacingRight ? frameWidth : 0;
            arrowPosition.y += frameHeight * 1.3f;
            SDL_RendererFlip arrowFlip = !isFacingRight ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

            if (arrowsQuantity > 0) {
                arrows.emplace_back(arrowPosition, arrowDirection, arrowSpeed, arrowTextureId, arrowFlip);
                std::cout << "Arrow fired from (" << arrowPosition.x << ", " << arrowPosition.y << ")" << std::endl;
                arrowsQuantity -= 1;
            }
        }
    }

    void Player::update(float dt) {
        handleInput();

        animTimer += dt;

        if (isRunning) {
            textureId = "player_run";
            totalFrames = runningFrames;

            if (animTimer >= runAnimSpeed) {
                animTimer = 0.0f;
                currentFrame = (currentFrame + 1) % totalFrames;
            }
        } else if (isGrounded) {
            textureId = "player";
            totalFrames = idleFrames;

            if (animTimer >= idleAnimSpeed) {
                animTimer = 0.0f;
                currentFrame = (currentFrame + 1) % totalFrames;
            }
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

        for (auto &arrow: arrows) {
            arrow.update(dt);
        }

        arrows.erase(std::remove_if(arrows.begin(), arrows.end(),
                                    [](const Arrow &arrow) {
                                        return arrow.isOffScreen(SCREEN_WIDTH, SCREEN_HEIGHT);
                                    }),
                     arrows.end());
    }

    void Player::render(SDL_Renderer *renderer, Camera *camera) {
        const Vector2D screenPos = camera->worldToScreen(position);

        constexpr int scaleFactor = 3;

        const int originalFrameWidth = frameWidth;
        const int originalFrameHeight = frameHeight;

        const int scaledWidth = originalFrameWidth * scaleFactor;
        const int scaledHeight = originalFrameHeight * scaleFactor;

        SDL_Rect destRect;
        destRect.x = static_cast<int>(screenPos.x);
        destRect.y = static_cast<int>(screenPos.y);
        destRect.w = scaledWidth;
        destRect.h = scaledHeight;

        TextureManager::getInstance().drawFrame(
            textureId,
            destRect.x,
            destRect.y,
            destRect.w,
            destRect.h,
            0,
            currentFrame,
            renderer,
            0.0,
            nullptr,
            flip,
            originalFrameWidth,
            originalFrameHeight
        );

        std::cout << arrows.size() << std::endl;
        for (auto &arrow: arrows) {
            arrow.render(renderer);
        }
    }

RB_ENGINE_END_NS
