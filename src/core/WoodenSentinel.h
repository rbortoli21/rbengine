//
// Created by bortoli on 08/03/25.
//

#ifndef WOODENSENTINEL_H
#define WOODENSENTINEL_H
#include "Enemy.h"

RB_ENGINE_NS
    class WoodenSentinel : public Enemy {
    public:
        WoodenSentinel(const Vector2D& pos);
        ~WoodenSentinel() override = default;
        
        void update(float dt) override;
        void takeDamage(int damage) override;
        void attack() override;
        
    private:
        void updateAI(float dt) override;
        void onDeath() override;
        
        // Wooden Sentinel specific stats
        static constexpr int WOODEN_SENTINEL_HEALTH = 80;
        static constexpr float WOODEN_SENTINEL_SPEED = 80.0f;
        static constexpr float WOODEN_SENTINEL_DAMAGE = 15.0f;
        static constexpr float WOODEN_SENTINEL_ATTACK_RANGE = 60.0f;
        static constexpr float WOODEN_SENTINEL_DETECTION_RANGE = 150.0f;
        
        // Special abilities
        bool canSummonBackup = true;
        float backupSummonCooldown = 0.0f;
        static constexpr float BACKUP_SUMMON_TIME = 10.0f;
    };

RB_ENGINE_END_NS

#endif //WOODENSENTINEL_H 