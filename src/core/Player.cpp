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
        std::cout << "Player loaded at (" << pos.x << ", " << pos.y << ")" << std::endl;
    }

    void Player::handleInput() {
        velocity.x = 0;

        if (InputManager::getInstance().isActionPressed(GameAction::MOVE_LEFT)) {
            velocity.x = -200.0f * (agi / 5.0f);
            isFacingRight = false;
            flip = SDL_FLIP_HORIZONTAL;
        }
        if (InputManager::getInstance().isActionPressed(GameAction::MOVE_RIGHT)) {
            velocity.x = 200.0f * (agi / 5.0f);
            isFacingRight = true;
            flip = SDL_FLIP_NONE;
        }

        if (InputManager::getInstance().isActionTriggered(GameAction::JUMP) && isGrounded) {
            velocity.y = -450.0f;
            isGrounded = false;
            std::cout << "Jump!" << std::endl;
        }
    }

    void Player::update(float dt) {
        handleInput();

        if (!isGrounded) {
            acceleration.y = 980.0f; // Pixels/second^2
        } else {
            acceleration.y = 0;
        }

        velocity += acceleration * dt;
        position += velocity * dt;

        if (position.y + height > SCREEN_HEIGHT - 50) {
            position.y = SCREEN_HEIGHT - 50 - height;
            velocity.y = 0;
            isGrounded = true;
        }

        if (position.x < 0) {
            position.x = 0;
        }
        if (position.x + width > SCREEN_WIDTH) {
            position.x = SCREEN_WIDTH - width;
        }

        // Update animation frame/row based on state (running, idle, jumping)
        // currentFrame = int(((SDL_GetTicks() / 100) % 6)); // Example animation
    }

    void Player::render(SDL_Renderer *renderer, Camera *camera) {
        Vector2D screenPos = camera->worldToScreen(position);

        TextureManager::getInstance().draw(textureId,
                                           static_cast<int>(screenPos.x),
                                           static_cast<int>(screenPos.y),
                                           width, height, renderer, flip);
    }

RB_ENGINE_END_NS
