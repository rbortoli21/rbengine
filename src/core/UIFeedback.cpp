//
// Created by bortoli on 08/03/25.
//

#include "UIFeedback.h"
#include "Constants.h"
#include "Vector2D.h"
#include <algorithm>
#include <iostream>

RB_ENGINE_NS
    UIFeedback::UIFeedback() : renderer(nullptr), font(nullptr) {}

    UIFeedback::~UIFeedback() {
        if (font) {
            TTF_CloseFont(font);
            font = nullptr;
        }
    }

    void UIFeedback::init(SDL_Renderer* r) {
        renderer = r;
        font = TTF_OpenFont("src/assets/fonts/poppins.ttf", 20);
        if (!font) {
            std::cerr << "Failed to load UI feedback font: " << TTF_GetError() << std::endl;
        }
    }

    void UIFeedback::update(float dt) {
        for (auto& message : messages) {
            if (message.isActive) {
                message.currentTime += dt;
                message.position.y -= 30.0f * dt; // Float upward
                
                // Fade out effect
                if (message.currentTime > message.duration * 0.7f) {
                    float fadeProgress = (message.currentTime - message.duration * 0.7f) / (message.duration * 0.3f);
                    message.alpha = 255.0f * (1.0f - fadeProgress);
                }
                
                if (message.currentTime >= message.duration) {
                    message.isActive = false;
                }
            }
        }
        
        cleanupExpiredMessages();
    }

    void UIFeedback::render(SDL_Renderer* r) {
        if (!font) return;
        
        for (const auto& message : messages) {
            if (message.isActive) {
                SDL_Color color = getColorForType(message.type);
                color.a = static_cast<Uint8>(message.alpha);
                
                SDL_Surface* surface = TTF_RenderText_Solid(font, message.text.c_str(), color);
                if (surface) {
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(r, surface);
                    if (texture) {
                        SDL_Rect destRect = {
                            static_cast<int>(message.position.x),
                            static_cast<int>(message.position.y),
                            surface->w,
                            surface->h
                        };
                        
                        SDL_SetTextureAlphaMod(texture, static_cast<Uint8>(message.alpha));
                        SDL_RenderCopy(r, texture, nullptr, &destRect);
                        SDL_DestroyTexture(texture);
                    }
                    SDL_FreeSurface(surface);
                }
            }
        }
    }

    void UIFeedback::showHealthPotionUsed() {
        Vector2D pos(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 100);
        showMessage("Drinking Health Potion...", FeedbackType::HEALTH_POTION, 2.0f);
    }

    void UIFeedback::showArrowFired() {
        Vector2D pos(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 50);
        showMessage("Arrow Fired!", FeedbackType::ARROW_FIRED, 1.5f);
    }

    void UIFeedback::showDodgePerformed() {
        Vector2D pos(SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2 - 60);
        showMessage("Dodge!", FeedbackType::DODGE_PERFORMED, 1.0f);
    }

    void UIFeedback::showAbilityUpgraded(const std::string& abilityName) {
        std::string message = "Ability Upgraded: " + abilityName;
        Vector2D pos(SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 - 120);
        showMessage(message, FeedbackType::ABILITY_UPGRADED, 3.0f);
    }

    void UIFeedback::showAttributeUpgraded(const std::string& attribute, int newValue) {
        std::string message = attribute + " Upgraded to " + std::to_string(newValue);
        Vector2D pos(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 100);
        showMessage(message, FeedbackType::ATTRIBUTE_UPGRADED, 3.0f);
    }

    void UIFeedback::showRelicEquipped(const std::string& relicName) {
        std::string message = "Relic Equipped: " + relicName;
        Vector2D pos(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 100);
        showMessage(message, FeedbackType::RELIC_EQUIPPED, 3.0f);
    }

    void UIFeedback::showFeatherCollected(int amount) {
        std::string message = "+" + std::to_string(amount) + " Mystic Feathers";
        Vector2D pos(SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 - 80);
        showMessage(message, FeedbackType::FEATHER_COLLECTED, 2.5f);
    }

    void UIFeedback::showDamageTaken(int damage) {
        std::string message = "-" + std::to_string(damage) + " HP";
        Vector2D pos(SCREEN_WIDTH / 2 - 30, SCREEN_HEIGHT / 2 - 30);
        showMessage(message, FeedbackType::DAMAGE_TAKEN, 2.0f);
    }

    void UIFeedback::showMessage(const std::string& message, FeedbackType type, float duration) {
        Vector2D pos(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50);
        messages.emplace_back(message, type, duration, pos);
    }

    SDL_Color UIFeedback::getColorForType(FeedbackType type) const {
        switch (type) {
            case FeedbackType::HEALTH_POTION:
                return {0, 255, 0, 255}; // Green
            case FeedbackType::ARROW_FIRED:
                return {255, 255, 0, 255}; // Yellow
            case FeedbackType::DODGE_PERFORMED:
                return {0, 255, 255, 255}; // Cyan
            case FeedbackType::ABILITY_UPGRADED:
                return {255, 165, 0, 255}; // Orange
            case FeedbackType::ATTRIBUTE_UPGRADED:
                return {138, 43, 226, 255}; // Purple
            case FeedbackType::RELIC_EQUIPPED:
                return {255, 215, 0, 255}; // Gold
            case FeedbackType::FEATHER_COLLECTED:
                return {255, 255, 255, 255}; // White
            case FeedbackType::DAMAGE_TAKEN:
                return {255, 0, 0, 255}; // Red
            default:
                return {255, 255, 255, 255}; // White
        }
    }

    void UIFeedback::cleanupExpiredMessages() {
        messages.erase(
            std::remove_if(messages.begin(), messages.end(),
                [](const FeedbackMessage& msg) { return !msg.isActive; }),
            messages.end()
        );
    }

RB_ENGINE_END_NS 