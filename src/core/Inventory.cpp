//
// Created by bortoli on 08/03/25.
//

#include "Inventory.h"
#include <algorithm>

RB_ENGINE_NS
    Inventory::Inventory() {
        // Initialize with some starting items
        currentArrows = maxArrowCapacity;
        healthPotions = 3;
    }
    
    bool Inventory::useArrow() {
        if (currentArrows > 0) {
            currentArrows--;
            return true;
        }
        return false;
    }
    
    void Inventory::addArrows(int count) {
        currentArrows = std::min(currentArrows + count, maxArrowCapacity);
    }
    
    void Inventory::upgradeArrowCapacity(int additionalCapacity) {
        maxArrowCapacity += additionalCapacity;
        currentArrows += additionalCapacity; // Refill when upgrading
    }
    
    bool Inventory::useHealthPotion() {
        if (healthPotions > 0) {
            healthPotions--;
            return true;
        }
        return false;
    }
    
    void Inventory::addHealthPotions(int count) {
        healthPotions = std::min(healthPotions + count, maxHealthPotions);
    }
    
    bool Inventory::addRelic(const Relic& relic) {
        if (relics.size() < maxRelicSlots) {
            relics.push_back(relic);
            return true;
        }
        return false;
    }
    
    bool Inventory::removeRelic(int slot) {
        if (slot >= 0 && slot < static_cast<int>(relics.size())) {
            relics.erase(relics.begin() + slot);
            return true;
        }
        return false;
    }
    
    float Inventory::getShootSpeedMultiplier() const {
        float multiplier = 1.0f;
        for (const auto& relic : relics) {
            if (relic.type == RelicType::QUICK_SHOT) {
                multiplier *= relic.effectValue;
            }
        }
        return multiplier;
    }
    
    float Inventory::getDamageMultiplier() const {
        float multiplier = 1.0f;
        for (const auto& relic : relics) {
            if (relic.type == RelicType::POWER_SHOT) {
                multiplier *= relic.effectValue;
            }
        }
        return multiplier;
    }
    
    float Inventory::getDodgeSpeedMultiplier() const {
        float multiplier = 1.0f;
        for (const auto& relic : relics) {
            if (relic.type == RelicType::SWIFT_DODGE) {
                multiplier *= relic.effectValue;
            }
        }
        return multiplier;
    }
    
    float Inventory::getHealthRegenRate() const {
        float regenRate = 0.0f;
        for (const auto& relic : relics) {
            if (relic.type == RelicType::HEALING_AURA) {
                regenRate += relic.effectValue;
            }
        }
        return regenRate;
    }

RB_ENGINE_END_NS 