//
// Created by bortoli on 3/6/25.
//

#ifndef WINDOWS_H
#define WINDOWS_H
#include <string>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include "EngineDefines.h"

RB_ENGINE_NS
    class Window {
    public:
        Window(const std::string &title, int width, int height, SDL_Renderer **renderer);

        ~Window();

        void pollEvents();

        void clear() const;

        [[nodiscard]] bool isRunning() const;

    private:
        SDL_Window *window;
        SDL_Renderer *renderer;
        bool running;
    };


RB_ENGINE_END_NS


#endif //WINDOWS_H
