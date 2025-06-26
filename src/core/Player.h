//
// Created by bortoli on 07/03/25.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <SDL_render.h>
#include <string>
#include <vector>

#include "Arrow.h"
#include "EngineDefines.h"
#include "GameObject.h"
#include "Inventory.h"
#include "CharacterProgression.h"
#include "UIFeedback.h"

RB_ENGINE_NS
    struct Vector2D;
    class Camera;

    class Player final : public GameObject {
    public:
        Player() = default;

        ~Player() override = default;

        void load(const Vector2D &pos, int w, int h,
                  const std::string &textureId) override;

        void update(float dt) override;

        void render(SDL_Renderer *renderer, Camera *camera) override;

        void handleInput();

        // Getters for UI
        [[nodiscard]] int getArrowsQuantity() const { return inventory.getCurrentArrows(); }
        [[nodiscard]] int getHealth() const { return currentHealth; }
        [[nodiscard]] int getMaxHealth() const { return maxHealth; }
        [[nodiscard]] int getHealthPotions() const { return inventory.getHealthPotions(); }
        [[nodiscard]] int getMysticFeathers() const { return progression.getMysticFeathers(); }
        
        // Attribute getters
        [[nodiscard]] int getPot() const { return progression.getAttribute("POT"); }
        [[nodiscard]] int getAgi() const { return progression.getAttribute("AGI"); }
        [[nodiscard]] int getRes() const { return progression.getAttribute("RES"); }
        
        // Systems access
        [[nodiscard]] const Inventory& getInventory() const { return inventory; }
        [[nodiscard]] const CharacterProgression& getProgression() const { return progression; }
        UIFeedback& getUIFeedback() { return uiFeedback; }
        
        // Combat
        void takeDamage(int damage);
        std::vector<Arrow>& getArrows() { return arrows; }

    private:
        // Core systems
        Inventory inventory;
        CharacterProgression progression;
        UIFeedback uiFeedback;
        
        // Health and stats
        int currentHealth = 100;
        int maxHealth = 100;
        
        // Movement states
        bool isGrounded = false;
        bool isFacingRight = true;
        bool isRunning = false;
        bool isDodging = false;
        bool isClimbing = false;
        bool canClimb = false;
        
        // Dodge mechanics
        float dodgeCooldown = 0.0f;
        float dodgeDuration = 0.0f;
        float dodgeSpeed = 400.0f;
        Vector2D dodgeDirection;
        
        // Climbing mechanics
        float climbSpeed = 150.0f;
        bool nearClimbableSurface = false;
        
        // Animation
        int totalFrames = 1;
        float animTimer = 0.0f;
        int frameWidth = 0;
        int frameHeight = 0;

        float idleAnimSpeed = 0.08f;
        int idleFrames = 18;
        int idleWidth = 1440;

        int runningFrames = 24;
        float runAnimSpeed = 0.002f;
        int runningWidth = 1920;

        int spriteHeight = 80;

        // Combat
        std::vector<Arrow> arrows;
        float arrowSpeed = 1100.0f;
        std::string arrowTextureId = "arrow";
        float shootCooldown = 0.0f;
        float baseShootCooldown = 0.5f;
        
        // Health regeneration
        float healthRegenTimer = 0.0f;
        float healthRegenInterval = 1.0f;
        
        // Private methods
        void updateMovement(float dt);
        void updateCombat(float dt);
        void updateHealthRegeneration(float dt);
        void handleDodge(float dt);
        void handleClimbing(float dt);
        void updateAnimation(float dt);
        void applyProgressionEffects();
        void heal(int amount);
    };


RB_ENGINE_END_NS


#endif //PLAYER_H
