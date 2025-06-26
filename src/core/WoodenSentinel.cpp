//
// Created by bortoli on 08/03/25.
//

#include "WoodenSentinel.h"
#include <iostream>

RB_ENGINE_NS
    WoodenSentinel::WoodenSentinel(const Vector2D& pos)
        : Enemy(EnemyType::WOODEN_SENTINEL, pos, 40, 60, "enemy") {
        
        // Set Wooden Sentinel specific stats
        maxHealth = WOODEN_SENTINEL_HEALTH;
        currentHealth = maxHealth;
        moveSpeed = WOODEN_SENTINEL_SPEED;
        attackDamage = WOODEN_SENTINEL_DAMAGE;
        attackRange = WOODEN_SENTINEL_ATTACK_RANGE;
        detectionRange = WOODEN_SENTINEL_DETECTION_RANGE;
        
        // Animation settings
        totalFrames = 1; // Single frame for now
        animSpeed = 0.15f;
        
        std::cout << "Wooden Sentinel spawned at (" << pos.x << ", " << pos.y << ")" << std::endl;
    }

    void WoodenSentinel::update(float dt) {
        // Update backup summon cooldown
        if (backupSummonCooldown > 0) {
            backupSummonCooldown -= dt;
        }
        
        Enemy::update(dt);
    }

    void WoodenSentinel::takeDamage(int damage) {
        Enemy::takeDamage(damage);
        
        // Wooden Sentinel specific damage response
        if (currentHealth < maxHealth * 0.3f && canSummonBackup && backupSummonCooldown <= 0) {
            // Low health - could summon backup (placeholder for now)
            std::cout << "Wooden Sentinel is calling for backup!" << std::endl;
            backupSummonCooldown = BACKUP_SUMMON_TIME;
        }
    }

    void WoodenSentinel::attack() {
        if (attackCooldown <= 0) {
            Enemy::attack();
            std::cout << "Wooden Sentinel swings its wooden staff!" << std::endl;
        }
    }

    void WoodenSentinel::updateAI(float dt) {
        // Call base AI first
        Enemy::updateAI(dt);
        
        // Wooden Sentinel specific AI behaviors
        if (state == EnemyState::CHASING) {
            // Wooden Sentinels are slower but more persistent
            moveSpeed = WOODEN_SENTINEL_SPEED * 0.8f;
        } else {
            moveSpeed = WOODEN_SENTINEL_SPEED;
        }
    }

    void WoodenSentinel::onDeath() {
        std::cout << "Wooden Sentinel has been defeated!" << std::endl;
        // Could drop items or trigger events here
    }

RB_ENGINE_END_NS 