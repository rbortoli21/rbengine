//
// Created by bortoli on 11/03/25.
//

#ifndef ENTITY_H
#define ENTITY_H
#include <memory>
#include <SDL_render.h>
#include <vector>

#include "Component.h"
#include "EngineDefines.h"

RB_ENGINE_NS
    class Entity final : public Component {
        bool active = true;
        std::vector<std::shared_ptr<Component> > components;

    public:
        int id;
        std::string name;

        void update(const float deltaTime) override {
            if (!active) return;
            for (const auto &component: components) {
                component->update(deltaTime);
            }
        };

        void render(SDL_Renderer *renderer) override {
            if (!active) return;
            for (const auto &component: components) {
                component->render(renderer);
            }
        };

        void addComponent(const std::shared_ptr<Component> &component) {
            components.push_back(component);
            component->init();
        };

        template<typename T>
        std::shared_ptr<T> getComponent() {
            for (const auto &component: components) {
                std::shared_ptr<T> casted = std::dynamic_pointer_cast<T>(component);
                if (casted != nullptr) {
                    return casted;
                }
            }
            return nullptr;
        };

        void activate() {
            active = true;
        };

        void deactivate() {
            active = false;
        };
    };

RB_ENGINE_END_NS


#endif //ENTITY_H
