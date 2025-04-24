//
// Created by bortoli on 24/04/25.
//

#include "Arrow.h"

#include <iostream>


RB_ENGINE_NS
    void Arrow::update(float dt) {
        position += direction * speed * dt;
    }

    void Arrow::render(SDL_Renderer *renderer) {
        SDL_Rect destRect;
        destRect.x = static_cast<int>(position.x);
        destRect.y = static_cast<int>(position.y);
        destRect.w = width;
        destRect.h = height;

        std::cout << "Rendering arrow at (" << destRect.x << ", " << destRect.y << ")" << std::endl;

        TextureManager::getInstance().draw(textureId, destRect.x, destRect.y, destRect.w, destRect.h, renderer,
                                           arrowFlip, 0.15f, 0.15f);
    }

    bool Arrow::isOffScreen(int screenWidth, int screenHeight) const {
        return position.x + width < 0 || position.x > screenWidth || position.y + height < 0 || position.y >
               screenHeight;
    }

RB_ENGINE_END_NS
