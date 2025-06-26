//
// Created by bortoli on 08/03/25.
//

#include "CharacterProgression.h"
#include <algorithm>

RB_ENGINE_NS
    CharacterProgression::CharacterProgression() {
        initializeAbilities();
        mysticFeathers = 50; // Starting currency
    }
    
    void CharacterProgression::initializeAbilities() {
        abilities = {
            Ability(AbilityType::FASTER_SHOOTING, "Quick Draw", "Increases shooting speed", 15, 1.2f),
            Ability(AbilityType::INCREASED_DAMAGE, "Power Shot", "Increases arrow damage", 20, 1.3f),
            Ability(AbilityType::IMPROVED_DODGING, "Swift Dodge", "Improves dodge speed and distance", 12, 1.25f),
            Ability(AbilityType::HEALTH_BOOST, "Vitality", "Increases maximum health", 25, 20.0f),
            Ability(AbilityType::ARROW_CAPACITY_BOOST, "Quiver Mastery", "Increases arrow capacity", 18, 5.0f)
        };
    }
    
    void CharacterProgression::upgradeAttribute(const std::string& attribute, int points) {
        if (attribute == "POT" && pot < maxPot) {
            pot = std::min(pot + points, maxPot);
        } else if (attribute == "AGI" && agi < maxAgi) {
            agi = std::min(agi + points, maxAgi);
        } else if (attribute == "RES" && res < maxRes) {
            res = std::min(res + points, maxRes);
        }
    }
    
    int CharacterProgression::getAttribute(const std::string& attribute) const {
        if (attribute == "POT") return pot;
        if (attribute == "AGI") return agi;
        if (attribute == "RES") return res;
        return 0;
    }
    
    int CharacterProgression::getMaxAttribute(const std::string& attribute) const {
        if (attribute == "POT") return maxPot;
        if (attribute == "AGI") return maxAgi;
        if (attribute == "RES") return maxRes;
        return 0;
    }
    
    bool CharacterProgression::upgradeAbility(AbilityType abilityType) {
        for (auto& ability : abilities) {
            if (ability.type == abilityType && ability.level < ability.maxLevel) {
                if (spendMysticFeathers(ability.cost)) {
                    ability.level++;
                    return true;
                }
            }
        }
        return false;
    }
    
    int CharacterProgression::getAbilityLevel(AbilityType abilityType) const {
        for (const auto& ability : abilities) {
            if (ability.type == abilityType) {
                return ability.level;
            }
        }
        return 0;
    }
    
    void CharacterProgression::addMysticFeathers(int count) {
        mysticFeathers += count;
    }
    
    bool CharacterProgression::spendMysticFeathers(int amount) {
        if (mysticFeathers >= amount) {
            mysticFeathers -= amount;
            return true;
        }
        return false;
    }
    
    float CharacterProgression::getShootSpeedBonus() const {
        float bonus = 1.0f;
        for (const auto& ability : abilities) {
            if (ability.type == AbilityType::FASTER_SHOOTING) {
                bonus *= (1.0f + (ability.level * 0.1f));
            }
        }
        return bonus;
    }
    
    float CharacterProgression::getDamageBonus() const {
        float bonus = 1.0f;
        for (const auto& ability : abilities) {
            if (ability.type == AbilityType::INCREASED_DAMAGE) {
                bonus *= (1.0f + (ability.level * 0.15f));
            }
        }
        return bonus;
    }
    
    float CharacterProgression::getDodgeSpeedBonus() const {
        float bonus = 1.0f;
        for (const auto& ability : abilities) {
            if (ability.type == AbilityType::IMPROVED_DODGING) {
                bonus *= (1.0f + (ability.level * 0.12f));
            }
        }
        return bonus;
    }
    
    int CharacterProgression::getHealthBonus() const {
        int bonus = 0;
        for (const auto& ability : abilities) {
            if (ability.type == AbilityType::HEALTH_BOOST) {
                bonus += static_cast<int>(ability.level * ability.effectValue);
            }
        }
        return bonus;
    }
    
    int CharacterProgression::getArrowCapacityBonus() const {
        int bonus = 0;
        for (const auto& ability : abilities) {
            if (ability.type == AbilityType::ARROW_CAPACITY_BOOST) {
                bonus += static_cast<int>(ability.level * ability.effectValue);
            }
        }
        return bonus;
    }

RB_ENGINE_END_NS 