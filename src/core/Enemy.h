//
// Created by bortoli on 08/03/25.
//

#ifndef ENEMY_H
#define ENEMY_H
#include <SDL_render.h>
#include <string>
#include "EngineDefines.h"
#include "GameObject.h"
#include "Vector2D.h"

RB_ENGINE_NS
    enum class EnemyType {
        WOODEN_SENTINEL,
        FOREST_GUARDIAN,
        FIRE_SALAMANDER,
        OBSIDIAN_DRAGON,
        TEMPLE_SENTINEL,
        DARK_ENTITY
    };

    enum class EnemyState {
        IDLE,
        PATROLLING,
        CHASING,
        ATTACKING,
        STUNNED,
        DEAD
    };

    class Enemy : public GameObject {
    public:
        Enemy(EnemyType type, const Vector2D& pos, int w, int h, const std::string& textureId);
        virtual ~Enemy() = default;

        void load(const Vector2D &pos, int w, int h, const std::string &textureId) override;
        void update(float dt) override;
        void render(SDL_Renderer *renderer, Camera *camera) override;

        // Enemy-specific methods
        virtual void takeDamage(int damage);
        virtual void attack();
        virtual void patrol(float dt);
        virtual void chasePlayer(const Vector2D& playerPos, float dt);
        
        // Getters
        [[nodiscard]] bool isAlive() const { return currentHealth > 0; }
        [[nodiscard]] int getHealth() const { return currentHealth; }
        [[nodiscard]] int getMaxHealth() const { return maxHealth; }
        [[nodiscard]] EnemyType getType() const { return type; }
        [[nodiscard]] EnemyState getState() const { return state; }
        [[nodiscard]] bool isAttacking() const { return state == EnemyState::ATTACKING; }
        [[nodiscard]] float getAttackRange() const { return attackRange; }
        [[nodiscard]] float getDetectionRange() const { return detectionRange; }
        [[nodiscard]] float getAttackDamage() const { return attackDamage; }
        
        // Setters
        void setPlayerPosition(const Vector2D& pos) { playerPosition = pos; }
        void setState(EnemyState newState) { state = newState; }

    protected:
        EnemyType type;
        EnemyState state = EnemyState::IDLE;
        
        // Stats
        int maxHealth = 100;
        int currentHealth = 100;
        float moveSpeed = 100.0f;
        float attackDamage = 20.0f;
        float attackRange = 50.0f;
        float detectionRange = 200.0f;
        
        // Combat
        float attackCooldown = 0.0f;
        float attackDuration = 0.0f;
        float stunDuration = 0.0f;
        
        // Movement
        Vector2D patrolStart;
        Vector2D patrolEnd;
        Vector2D playerPosition;
        float patrolTimer = 0.0f;
        float patrolDuration = 3.0f;
        
        // Animation
        float animTimer = 0.0f;
        float animSpeed = 0.1f;
        int totalFrames = 1;
        int currentFrame = 0;
        int frameWidth = 0;
        int frameHeight = 0;
        
        // AI
        float lastAttackTime = 0.0f;
        float attackCooldownTime = 1.0f;
        
        // Virtual methods for specific enemy behaviors
        virtual void updateAI(float dt);
        virtual void updateAnimation(float dt);
        virtual void updateCombat(float dt);
        virtual void onDeath();
    };

RB_ENGINE_END_NS

#endif //ENEMY_H 