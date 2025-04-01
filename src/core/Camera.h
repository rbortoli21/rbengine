//
// Created by bortoli on 01/04/25.
//
#pragma once

#ifndef CAMERA_H
#define CAMERA_H
#include <SDL_rect.h>

#include "Constants.h"
#include "EngineDefines.h"
#include "Vector2D.h"

RB_ENGINE_NS
    struct Vector2D;

    class Camera {
    public:
        Camera() : position(0, 0), target(nullptr) {
        }

        void setTarget(Vector2D *targetPos) { target = targetPos; }

        void update(int levelWidth, int levelHeight);

        [[nodiscard]] Vector2D worldToScreen(const Vector2D &worldPos) const;

        [[nodiscard]] SDL_Rect getViewport() const;

        [[nodiscard]] Vector2D getPosition() const { return position; }

    private:
        Vector2D position;
        Vector2D *target;
        int viewWidth = SCREEN_WIDTH;
        int viewHeight = SCREEN_HEIGHT;
    };

RB_ENGINE_END_NS
#endif //CAMERA_H
