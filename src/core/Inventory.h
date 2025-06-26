//
// Created by bortoli on 08/03/25.
//

#ifndef INVENTORY_H
#define INVENTORY_H
#include <vector>
#include <string>
#include "EngineDefines.h"

RB_ENGINE_NS
    enum class RelicType {
        QUICK_SHOT,      // Faster shooting
        POWER_SHOT,      // Increased damage
        SWIFT_DODGE,     // Improved dodging
        HEALING_AURA,    // Passive health regeneration
        ARROW_MASTER     // Increased arrow capacity
    };

    struct Relic {
        RelicType type;
        std::string name;
        std::string description;
        int level = 1;
        float effectValue = 1.0f;
        
        Relic(RelicType t, const std::string& n, const std::string& desc, float effect = 1.0f)
            : type(t), name(n), description(desc), effectValue(effect) {}
    };

    class Inventory {
    public:
        Inventory();
        
        // Arrow management
        bool useArrow();
        void addArrows(int count);
        void upgradeArrowCapacity(int additionalCapacity);
        [[nodiscard]] int getArrowCapacity() const { return maxArrowCapacity; }
        [[nodiscard]] int getCurrentArrows() const { return currentArrows; }
        
        // Health potion management
        bool useHealthPotion();
        void addHealthPotions(int count);
        [[nodiscard]] int getHealthPotions() const { return healthPotions; }
        [[nodiscard]] int getMaxHealthPotions() const { return maxHealthPotions; }
        
        // Relic management
        bool addRelic(const Relic& relic);
        bool removeRelic(int slot);
        [[nodiscard]] const std::vector<Relic>& getRelics() const { return relics; }
        [[nodiscard]] int getMaxRelicSlots() const { return maxRelicSlots; }
        
        // Relic effects
        [[nodiscard]] float getShootSpeedMultiplier() const;
        [[nodiscard]] float getDamageMultiplier() const;
        [[nodiscard]] float getDodgeSpeedMultiplier() const;
        [[nodiscard]] float getHealthRegenRate() const;
        
    private:
        int maxArrowCapacity = 20;
        int currentArrows = 20;
        int maxHealthPotions = 5;
        int healthPotions = 3;
        int maxRelicSlots = 3;
        std::vector<Relic> relics;
    };

RB_ENGINE_END_NS

#endif //INVENTORY_H 