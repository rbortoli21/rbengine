//
// Created by bortoli on 04/03/25.
//

#include "Application.h"
#include <cstdlib>

RB_ENGINE_NS
    Application *sharedApplication = nullptr;

    Application::Application() {
        sharedApplication = this;
    }

    Application::~Application() {
    }

    Application *Application::getInstance() {
        return sharedApplication;
    }

    int Application::run() {
        if (!start()) {
            return -1;
        }

        while (!stop()) {
            //execute game loop
        }

        return EXIT_SUCCESS;
    }

RB_ENGINE_END_NS
