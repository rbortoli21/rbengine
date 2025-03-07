//
// Created by bortoli on 04/03/25.
//

#include "Application.h"
#include <cstdlib>
#include <SDL2/SDL.h>

RB_ENGINE_NS
    Application *sharedApplication = nullptr;

    Application::Application(): window(nullptr) {
        sharedApplication = this;
        window = new Window("Application Window", 800, 600);
    }

    Application::~Application() {
        delete window;
    }

    Application *Application::getInstance() {
        return sharedApplication;
    }

    int Application::run() {
        if (!window->isRunning()) {
            return -1;
        }

        while (window->isRunning()) {
            window->pollEvents();
            window->clear();
            SDL_Delay(16);
        }

        return EXIT_SUCCESS;
    }

RB_ENGINE_END_NS
