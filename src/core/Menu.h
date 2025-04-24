//
// Created by bortoli on 24/04/25.
//

#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>

#include "EngineDefines.h"
RB_ENGINE_NS
    class Menu {
    public:
        Menu(SDL_Renderer *renderer);

        ~Menu();

        void handleInput(SDL_Event &event);

        void update();

        void render();

        bool isRunning() const { return running; }
        int getSelectedOption() const { return selectedOption; }

    private:
        SDL_Renderer *renderer;
        TTF_Font *font;
        std::vector<std::string> options;
        int selectedOption;
        bool running;

        SDL_Color normalColor;
        SDL_Color selectedColor;
    };

RB_ENGINE_END_NS
#endif // MENU_H

