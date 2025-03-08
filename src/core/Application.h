//
// Created by bortoli on 04/03/25.
//
#pragma once

#ifndef APPLICATION_H
#define APPLICATION_H
#include "EngineDefines.h"

RB_ENGINE_NS
    class Game;

    class Application {
    public:
        Application();

        ~Application() = default;

        [[nodiscard]] int run() const;

    private:
        Game *game;
    };

RB_ENGINE_END_NS
#endif //APPLICATION_H
