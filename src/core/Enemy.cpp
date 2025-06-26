//
// Created by bortoli on 08/03/25.
//

#include "Enemy.h"
#include <iostream>
#include <cmath>
#include "Camera.h"
#include "TextureManager.h"
#include "Constants.h"

RB_ENGINE_NS
    Enemy::Enemy(EnemyType type, const Vector2D& pos, int w, int h, const std::string& textureId)
        : type(type) {
        load(pos, w, h, textureId);
        
        // Set patrol area
        patrolStart = pos;
        patrolEnd = pos + Vector2D(200, 0);
        
        // Initialize animation
        frameWidth = w;
        frameHeight = h;
        totalFrames = 1;
    }

    void Enemy::load(const Vector2D &pos, int w, int h, const std::string &textureId) {
        GameObject::load(pos, w, h, textureId);
        currentHealth = maxHealth;
    }

    void Enemy::update(float dt) {
        if (!isAlive()) return;
        
        // Update cooldowns
        if (attackCooldown > 0) attackCooldown -= dt;
        if (stunDuration > 0) {
            stunDuration -= dt;
            if (stunDuration <= 0) {
                state = EnemyState::IDLE;
            }
            return;
        }
        
        updateAI(dt);
        updateAnimation(dt);
        updateCombat(dt);
        
        // Apply physics
        if (!isGrounded) {
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

    void Enemy::updateAI(float dt) {
        // Calculate distance to player
        float distanceToPlayer = std::sqrt(
            std::pow(playerPosition.x - position.x, 2) + 
            std::pow(playerPosition.y - position.y, 2)
        );
        
        // State machine
        switch (state) {
            case EnemyState::IDLE:
                if (distanceToPlayer <= detectionRange) {
                    state = EnemyState::CHASING;
                } else {
                    patrol(dt);
                }
                break;
                
            case EnemyState::PATROLLING:
                if (distanceToPlayer <= detectionRange) {
                    state = EnemyState::CHASING;
                } else {
                    patrol(dt);
                }
                break;
                
            case EnemyState::CHASING:
                if (distanceToPlayer > detectionRange * 1.5f) {
                    state = EnemyState::IDLE;
                } else if (distanceToPlayer <= attackRange) {
                    state = EnemyState::ATTACKING;
                } else {
                    chasePlayer(playerPosition, dt);
                }
                break;
                
            case EnemyState::ATTACKING:
                if (distanceToPlayer > attackRange) {
                    state = EnemyState::CHASING;
                } else {
                    attack();
                }
                break;
                
            default:
                break;
        }
    }

    void Enemy::updateAnimation(float dt) {
        animTimer += dt;
        
        if (animTimer >= animSpeed) {
            animTimer = 0.0f;
            currentFrame = (currentFrame + 1) % totalFrames;
        }
    }

    void Enemy::updateCombat(float dt) {
        if (state == EnemyState::ATTACKING && attackCooldown <= 0) {
            attackCooldown = attackCooldownTime;
        }
    }

    void Enemy::takeDamage(int damage) {
        currentHealth = std::max(0, currentHealth - damage);
        
        if (currentHealth <= 0) {
            state = EnemyState::DEAD;
            onDeath();
        } else {
            // Stun effect
            stunDuration = 0.5f;
            state = EnemyState::STUNNED;
        }
    }

    void Enemy::attack() {
        if (attackCooldown <= 0) {
            attackCooldown = attackCooldownTime;
            attackDuration = 0.5f;
            std::cout << "Enemy attacks for " << attackDamage << " damage!" << std::endl;
        }
    }

    void Enemy::patrol(float dt) {
        patrolTimer += dt;
        
        if (patrolTimer >= patrolDuration) {
            patrolTimer = 0.0f;
            // Swap patrol direction
            Vector2D temp = patrolStart;
            patrolStart = patrolEnd;
            patrolEnd = temp;
        }
        
        // Move towards patrol end
        Vector2D direction = patrolEnd - position;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        
        if (distance > 10.0f) {
            direction = direction * (1.0f / distance);
            velocity.x = direction.x * moveSpeed;
            
            // Update facing direction
            if (direction.x > 0) {
                flip = SDL_FLIP_NONE;
            } else if (direction.x < 0) {
                flip = SDL_FLIP_HORIZONTAL;
            }
        }
    }

    void Enemy::chasePlayer(const Vector2D& playerPos, float dt) {
        Vector2D direction = playerPos - position;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        
        if (distance > 0) {
            direction = direction * (1.0f / distance);
            velocity.x = direction.x * moveSpeed;
            
            // Update facing direction
            if (direction.x > 0) {
                flip = SDL_FLIP_NONE;
            } else if (direction.x < 0) {
                flip = SDL_FLIP_HORIZONTAL;
            }
        }
    }

    void Enemy::onDeath() {
        std::cout << "Enemy defeated!" << std::endl;
    }

    void Enemy::render(SDL_Renderer *renderer, Camera *camera) {
        if (!isAlive()) return;
        
        const Vector2D screenPos = camera->worldToScreen(position);
        
        // Use actual PNG dimensions instead of frame dimensions
        constexpr int actualWidth = 634;  // Use the detected PNG width
        constexpr int actualHeight = 512; // Use the detected PNG height
        constexpr float scaleFactor = 0.1f; // Scale down the large PNG
        const int scaledWidth = static_cast<int>(actualWidth * scaleFactor);
        const int scaledHeight = static_cast<int>(actualHeight * scaleFactor);
        
        // Debug output
        static int debugCounter = 0;
        if (debugCounter++ % 120 == 0) { // Print every 2 seconds
            std::cout << "Enemy render debug - PNG: " << actualWidth << "x" << actualHeight 
                      << " Scaled: " << scaledWidth << "x" << scaledHeight 
                      << " Pos: (" << screenPos.x << ", " << screenPos.y << ")" << std::endl;
        }
        
        SDL_Rect destRect;
        destRect.x = static_cast<int>(screenPos.x);
        destRect.y = static_cast<int>(screenPos.y);
        destRect.w = scaledWidth;
        destRect.h = scaledHeight;
        
        if (totalFrames <= 1) {
            // For single-frame images, use the actual texture dimensions
            TextureManager::getInstance().draw(
                textureId,
                destRect.x,
                destRect.y,
                actualWidth,  // Use actual PNG width as source
                actualHeight, // Use actual PNG height as source
                renderer,
                flip,
                scaleFactor,  // Scale X
                scaleFactor   // Scale Y
            );
        } else {
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
                frameWidth,
                frameHeight
            );
        }
        
        // Render health bar
        if (currentHealth < maxHealth) {
            constexpr SDL_Color red = {255, 0, 0, 255};
            constexpr SDL_Color green = {0, 255, 0, 255};
            
            float healthPercent = static_cast<float>(currentHealth) / maxHealth;
            
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_Rect healthBarBg = {
                static_cast<int>(screenPos.x),
                static_cast<int>(screenPos.y - 20),
                scaledWidth,
                5
            };
            SDL_RenderFillRect(renderer, &healthBarBg);
            
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_Rect healthBarFill = {
                static_cast<int>(screenPos.x),
                static_cast<int>(screenPos.y - 20),
                static_cast<int>(scaledWidth * healthPercent),
                5
            };
            SDL_RenderFillRect(renderer, &healthBarFill);
        }
    }

RB_ENGINE_END_NS 