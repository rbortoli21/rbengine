//
// Created by bortoli on 24/04/25.
//

#ifndef ARROW_H
#define ARROW_H
#include <SDL_render.h>
#include <string>
#include "Vector2D.h"
#include "TextureManager.h"

RB_ENGINE_NS
    class Arrow {
    public:
        Arrow(const Vector2D &pos, const Vector2D &dir, float speed, const std::string &textureId,
              SDL_RendererFlip flip)
            : position(pos), direction(dir), speed(speed), textureId(textureId), arrowFlip(flip) {
        }

        void update(float dt);

        void render(SDL_Renderer *renderer);

        bool isOffScreen(int screenWidth, int screenHeight) const;

    private:
        Vector2D position;
        Vector2D direction;
        SDL_RendererFlip arrowFlip;
        float speed;
        std::string textureId;
        int width = 700;
        int height = 200;
    };

RB_ENGINE_END_NS

#endif //ARROW_H
