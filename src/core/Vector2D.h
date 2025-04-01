//
// Created by bortoli on 01/04/25.
//

#ifndef VECTOR2D_H
#define VECTOR2D_H
#include "EngineDefines.h"

RB_ENGINE_NS
    struct Vector2D {
        float x = 0.0f;
        float y = 0.0f;

        Vector2D() = default;

        Vector2D(const float x, const float y) : x(x), y(y) {
        }

        Vector2D operator+(const Vector2D &other) const {
            return {x + other.x, y + other.y};
        }

        Vector2D &operator+=(const Vector2D &other) {
            x += other.x;
            y += other.y;
            return *this;
        }

        Vector2D operator*(float scalar) const {
            return Vector2D(x * scalar, y * scalar);
        }
    };

RB_ENGINE_END_NS

#endif //VECTOR2D_H
