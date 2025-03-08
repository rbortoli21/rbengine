//
// Created by bortoli on 04/03/25.
//

#include "Application.h"
#include "Game.h"

RB_ENGINE_NS
    Application::Application() {
        game = new Game();
    }

    int Application::run() const {
        if (!game->init()) {
            return EXIT_FAILURE;
        }

        game->run();

        return EXIT_SUCCESS;
    }

RB_ENGINE_END_NS
