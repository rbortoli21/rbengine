//
// Created by bortoli on 3/6/25.
//

#include "Window.h"

#include <iostream>
#include <SDL2/SDL.h>

RB_ENGINE_NS
    Window::Window(const std::string &title,
                   const int width,
                   const int height,
                   SDL_Renderer **renderer)
        : window(nullptr), renderer(*renderer), running(true) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
            running = false;
            return;
        }

        window = SDL_CreateWindow(title.c_str(),
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  width,
                                  height,
                                  SDL_WINDOW_SHOWN);
        if (!window) {
            std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
            running = false;
            return;
        }

        *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
            running = false;
        }
    }

    Window::~Window() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void Window::pollEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
    }

    void Window::clear() const {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    bool Window::isRunning() const {
        return running;
    }


RB_ENGINE_END_NS
