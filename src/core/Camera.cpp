//
// Created by bortoli on 01/04/25.
//

#include "Camera.h"

RB_ENGINE_NS
    void Camera::update(const int levelWidth, const int levelHeight) {
        if (target) {
            position.x = target->x - viewWidth;
            position.y = target->y - viewHeight;

            if (position.x < 0) {
                position.x = 0;
            }
            if (position.y < 0) {
                position.y = 0;
            }
            if (position.x > levelWidth - viewWidth) {
                position.x = levelWidth - viewWidth;
            }
            if (position.y > levelHeight - viewHeight) {
                position.y = levelHeight - viewHeight;
            }
            // Ensure level dimensions are larger than camera view
            if (levelWidth < viewWidth) position.x = (levelWidth - viewWidth) / 2.0f;
            if (levelHeight < viewHeight) position.y = (levelHeight - viewHeight) / 2.0f;
        }
    }

    Vector2D Camera::worldToScreen(const Vector2D &worldPos) const {
        return Vector2D(worldPos.x - position.x, worldPos.y - position.y);
    }

    SDL_Rect Camera::getViewport() const {
        return {
            static_cast<int>(position.x), static_cast<int>(position.y),
            viewWidth, viewHeight
        };
    }

RB_ENGINE_END_NS
