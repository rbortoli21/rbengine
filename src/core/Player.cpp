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
        
        // Initialize health based on progression
        applyProgressionEffects();
        currentHealth = maxHealth;
        
        // Initialize UI feedback
        uiFeedback.init(nullptr); // Will be set in Game class
        
        frameWidth = idleWidth / idleFrames;
        frameHeight = spriteHeight;
        totalFrames = idleFrames;

        std::cout << "Player loaded at (" << pos.x << ", " << pos.y << ")" << std::endl;
    }

    void Player::handleInput() {
        // Handle ability upgrades
        if (InputManager::getInstance().isActionTriggered(GameAction::UPGRADE_ABILITY_1)) {
            if (progression.upgradeAbility(AbilityType::FASTER_SHOOTING)) {
                uiFeedback.showAbilityUpgraded("Quick Draw");
            }
        }
        if (InputManager::getInstance().isActionTriggered(GameAction::UPGRADE_ABILITY_2)) {
            if (progression.upgradeAbility(AbilityType::INCREASED_DAMAGE)) {
                uiFeedback.showAbilityUpgraded("Power Shot");
            }
        }
        if (InputManager::getInstance().isActionTriggered(GameAction::UPGRADE_ABILITY_3)) {
            if (progression.upgradeAbility(AbilityType::IMPROVED_DODGING)) {
                uiFeedback.showAbilityUpgraded("Swift Dodge");
            }
        }
        if (InputManager::getInstance().isActionTriggered(GameAction::UPGRADE_ABILITY_4)) {
            if (progression.upgradeAbility(AbilityType::HEALTH_BOOST)) {
                uiFeedback.showAbilityUpgraded("Vitality");
                applyProgressionEffects();
            }
        }
        if (InputManager::getInstance().isActionTriggered(GameAction::UPGRADE_ABILITY_5)) {
            if (progression.upgradeAbility(AbilityType::ARROW_CAPACITY_BOOST)) {
                uiFeedback.showAbilityUpgraded("Quiver Mastery");
                inventory.upgradeArrowCapacity(progression.getArrowCapacityBonus());
            }
        }
        
        // Handle health potion usage
        if (InputManager::getInstance().isActionTriggered(GameAction::USE_HEALTH_POTION)) {
            if (inventory.useHealthPotion()) {
                heal(50); // Heal 50 HP
                uiFeedback.showHealthPotionUsed();
            }
        }
    }

    void Player::update(float dt) {
        handleInput();
        
        // Update cooldowns
        if (dodgeCooldown > 0) dodgeCooldown -= dt;
        if (shootCooldown > 0) shootCooldown -= dt;
        
        // Update systems
        updateMovement(dt);
        updateCombat(dt);
        updateHealthRegeneration(dt);
        updateAnimation(dt);
        
        // Update UI feedback
        uiFeedback.update(dt);
        
        // Update arrows
        for (auto &arrow: arrows) {
            arrow.update(dt);
        }

        arrows.erase(std::remove_if(arrows.begin(), arrows.end(),
                                    [](const Arrow &arrow) {
                                        return arrow.isOffScreen(SCREEN_WIDTH, SCREEN_HEIGHT);
                                    }),
                     arrows.end());
    }

    void Player::updateMovement(float dt) {
        if (isDodging) {
            handleDodge(dt);
            return;
        }
        
        if (isClimbing) {
            handleClimbing(dt);
            return;
        }
        
        velocity.x = 0;
        isRunning = false;
        
        // Basic movement
        if (InputManager::getInstance().isActionPressed(GameAction::MOVE_LEFT)) {
            velocity.x = -200.0f * (getAgi() / 5.0f);
            isFacingRight = false;
            flip = SDL_FLIP_HORIZONTAL;
            if (isGrounded) isRunning = true;
        }
        if (InputManager::getInstance().isActionPressed(GameAction::MOVE_RIGHT)) {
            velocity.x = 200.0f * (getAgi() / 5.0f);
            isFacingRight = true;
            flip = SDL_FLIP_NONE;
            if (isGrounded) isRunning = true;
        }

        // Jumping
        if (InputManager::getInstance().isActionTriggered(GameAction::JUMP) && isGrounded) {
            velocity.y = -450.0f;
            isRunning = false;
            isGrounded = false;
        }
        
        // Dodging
        if (InputManager::getInstance().isActionTriggered(GameAction::DODGE) && dodgeCooldown <= 0 && isGrounded) {
            isDodging = true;
            dodgeDuration = 0.3f;
            dodgeCooldown = 1.0f;
            dodgeDirection = isFacingRight ? Vector2D(1, 0) : Vector2D(-1, 0);
            uiFeedback.showDodgePerformed();
        }
        
        // Climbing (simplified - just move up when near climbable surface)
        if (InputManager::getInstance().isActionPressed(GameAction::CLIMB) && nearClimbableSurface) {
            isClimbing = true;
            velocity.y = -climbSpeed;
        }
        
        // Apply physics
        if (!isGrounded && !isClimbing) {
            acceleration.y = 980.0f;
        } else {
            acceleration.y = 0;
        }

        velocity += acceleration * dt;
        position += velocity * dt;

        // Ground collision
        const float floorLevel = 600.0f;
        if (position.y + height > floorLevel) {
            position.y = floorLevel - height;
            if (velocity.y > 0) {
                velocity.y = 0;
            }
            isGrounded = true;
        }
    }

    void Player::updateCombat(float dt) {
        if (InputManager::getInstance().isActionTriggered(GameAction::FIRE) && shootCooldown <= 0) {
            if (inventory.useArrow()) {
                Vector2D arrowDirection = isFacingRight ? Vector2D(1, 0) : Vector2D(-1, 0);
                Vector2D arrowPosition = position;
                arrowPosition.x += isFacingRight ? frameWidth : 0;
                arrowPosition.y += frameHeight * 1.3f;
                SDL_RendererFlip arrowFlip = !isFacingRight ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

                arrows.emplace_back(arrowPosition, arrowDirection, arrowSpeed, arrowTextureId, arrowFlip);
                uiFeedback.showArrowFired();
                
                // Apply shoot speed bonus
                float shootSpeedMultiplier = progression.getShootSpeedBonus() * inventory.getShootSpeedMultiplier();
                shootCooldown = baseShootCooldown / shootSpeedMultiplier;
            }
        }
    }

    void Player::updateHealthRegeneration(float dt) {
        float regenRate = inventory.getHealthRegenRate();
        if (regenRate > 0) {
            healthRegenTimer += dt;
            if (healthRegenTimer >= healthRegenInterval) {
                healthRegenTimer = 0.0f;
                heal(static_cast<int>(regenRate));
            }
        }
    }

    void Player::handleDodge(float dt) {
        dodgeDuration -= dt;
        if (dodgeDuration <= 0) {
            isDodging = false;
            velocity.x = 0;
        } else {
            float dodgeSpeedMultiplier = progression.getDodgeSpeedBonus() * inventory.getDodgeSpeedMultiplier();
            velocity.x = dodgeDirection.x * dodgeSpeed * dodgeSpeedMultiplier;
        }
    }

    void Player::handleClimbing(float dt) {
        if (!InputManager::getInstance().isActionPressed(GameAction::CLIMB)) {
            isClimbing = false;
        }
    }

    void Player::updateAnimation(float dt) {
        animTimer += dt;

        if (isDodging) {
            textureId = "player_run"; // Use running animation for dodge
            totalFrames = runningFrames;
            if (animTimer >= runAnimSpeed) {
                animTimer = 0.0f;
                currentFrame = (currentFrame + 1) % totalFrames;
            }
        } else if (isRunning) {
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
    }

    void Player::applyProgressionEffects() {
        maxHealth = progression.getAttribute("RES") + progression.getHealthBonus();
        if (currentHealth > maxHealth) {
            currentHealth = maxHealth;
        }
    }

    void Player::takeDamage(int damage) {
        currentHealth = std::max(0, currentHealth - damage);
        uiFeedback.showDamageTaken(damage);
    }

    void Player::heal(int amount) {
        currentHealth = std::min(maxHealth, currentHealth + amount);
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

        // Render arrows
        for (auto &arrow: arrows) {
            arrow.render(renderer);
        }
        
        // Render UI feedback
        uiFeedback.render(renderer);
    }

RB_ENGINE_END_NS
