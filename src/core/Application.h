//
// Created by bortoli on 04/03/25.
//
#pragma once

#ifndef APPLICATION_H
#define APPLICATION_H
#include "ApplicationInterface.h"
#include "EngineDefines.h"
#include "Window.h"

RB_ENGINE_NS
    class Application : public ApplicationInterface {
    public:
        Application();

        ~Application() override;

        static Application *getInstance();

        int run();

    private:
        Window *window{};
    };

RB_ENGINE_END_NS
#endif //APPLICATION_H
