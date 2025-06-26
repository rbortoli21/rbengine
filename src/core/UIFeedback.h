//
// Created by bortoli on 08/03/25.
//

#ifndef UIFEEDBACK_H
#define UIFEEDBACK_H
#include <vector>
#include <string>
#include <SDL_render.h>
#include <SDL_ttf.h>
#include "EngineDefines.h"
#include "Vector2D.h"

RB_ENGINE_NS
    enum class FeedbackType {
        HEALTH_POTION,
        ARROW_FIRED,
        DODGE_PERFORMED,
        ABILITY_UPGRADED,
        ATTRIBUTE_UPGRADED,
        RELIC_EQUIPPED,
        FEATHER_COLLECTED,
        DAMAGE_TAKEN
    };

    struct FeedbackMessage {
        std::string text;
        FeedbackType type;
        float duration;
        float currentTime;
        float alpha;
        Vector2D position;
        bool isActive;
        
        FeedbackMessage(const std::string& t, FeedbackType ft, float d, const Vector2D& pos)
            : text(t), type(ft), duration(d), currentTime(0.0f), alpha(255.0f), position(pos), isActive(true) {}
    };

    class UIFeedback {
    public:
        UIFeedback();
        ~UIFeedback();
        
        void init(SDL_Renderer* renderer);
        void update(float dt);
        void render(SDL_Renderer* renderer);
        
        // Add feedback messages
        void showHealthPotionUsed();
        void showArrowFired();
        void showDodgePerformed();
        void showAbilityUpgraded(const std::string& abilityName);
        void showAttributeUpgraded(const std::string& attribute, int newValue);
        void showRelicEquipped(const std::string& relicName);
        void showFeatherCollected(int amount);
        void showDamageTaken(int damage);
        
        // Custom message
        void showMessage(const std::string& message, FeedbackType type, float duration = 3.0f);
        
    private:
        SDL_Renderer* renderer;
        TTF_Font* font;
        std::vector<FeedbackMessage> messages;
        
        SDL_Color getColorForType(FeedbackType type) const;
        void cleanupExpiredMessages();
    };

RB_ENGINE_END_NS

#endif //UIFEEDBACK_H 