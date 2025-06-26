//
// Created by bortoli on 01/04/25.
//

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <SDL_render.h>
#include <string>

#include "EngineDefines.h"
#include "Vector2D.h"

RB_ENGINE_NS
    struct Vector2D;
    class Camera;

    class GameObject {
    public:
        virtual ~GameObject() = default;

        virtual void load(const Vector2D &pos, int w, int h,
                          const std::string &textureId) {
            position = pos;
            width = w;
            height = h;
            this->textureId = textureId;
        }

        virtual void update(float dt) = 0;

        virtual void render(SDL_Renderer *renderer, Camera *camera) = 0;

        [[nodiscard]] Vector2D getPosition() const { return position; }
        Vector2D *getPositionPtr() { return &position; }
        [[nodiscard]] int getWidth() const { return width; }
        [[nodiscard]] int getHeight() const { return height; }

        [[nodiscard]] SDL_Rect getBoundingBox() const {
            return {
                static_cast<int>(position.x), static_cast<int>(position.y),
                width, height
            };
        }

    protected:
        Vector2D position;
        Vector2D velocity;
        Vector2D acceleration;

        int width = 0;
        int height = 0;
        std::string textureId;
        int currentFrame = 0;
        int currentRow = 0;
        int spriteRow = 0;
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        bool isGrounded = false;
    };

RB_ENGINE_END_NS

#endif //GAMEOBJECT_H
