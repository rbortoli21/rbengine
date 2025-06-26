//
// Created by bortoli on 08/03/25.
//

#ifndef CHARACTERPROGRESSION_H
#define CHARACTERPROGRESSION_H
#include <string>
#include <vector>
#include "EngineDefines.h"

RB_ENGINE_NS
    enum class AbilityType {
        FASTER_SHOOTING,
        INCREASED_DAMAGE,
        IMPROVED_DODGING,
        HEALTH_BOOST,
        ARROW_CAPACITY_BOOST
    };

    struct Ability {
        AbilityType type;
        std::string name;
        std::string description;
        int level = 0;
        int maxLevel = 5;
        int cost = 10;
        float effectValue = 1.0f;
        
        Ability(AbilityType t, const std::string& n, const std::string& desc, int c = 10, float effect = 1.0f)
            : type(t), name(n), description(desc), cost(c), effectValue(effect) {}
    };

    class CharacterProgression {
    public:
        CharacterProgression();
        
        // Attribute management
        void upgradeAttribute(const std::string& attribute, int points = 1);
        [[nodiscard]] int getAttribute(const std::string& attribute) const;
        [[nodiscard]] int getMaxAttribute(const std::string& attribute) const;
        
        // Ability management
        bool upgradeAbility(AbilityType abilityType);
        [[nodiscard]] int getAbilityLevel(AbilityType abilityType) const;
        [[nodiscard]] const std::vector<Ability>& getAbilities() const { return abilities; }
        
        // Currency management
        void addMysticFeathers(int count);
        bool spendMysticFeathers(int amount);
        [[nodiscard]] int getMysticFeathers() const { return mysticFeathers; }
        
        // Effect calculations
        [[nodiscard]] float getShootSpeedBonus() const;
        [[nodiscard]] float getDamageBonus() const;
        [[nodiscard]] float getDodgeSpeedBonus() const;
        [[nodiscard]] int getHealthBonus() const;
        [[nodiscard]] int getArrowCapacityBonus() const;
        
    private:
        int pot = 10;  // Power
        int agi = 7;   // Agility
        int res = 100; // Resilience (max health)
        
        int maxPot = 50;
        int maxAgi = 30;
        int maxRes = 200;
        
        int mysticFeathers = 0;
        std::vector<Ability> abilities;
        
        void initializeAbilities();
    };

RB_ENGINE_END_NS

#endif //CHARACTERPROGRESSION_H 