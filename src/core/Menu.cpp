//
// Created by bortoli on 24/04/25.
//

#include "Menu.h"
#include <iostream>

RB_ENGINE_NS
    Menu::Menu(SDL_Renderer *renderer)
        : renderer(renderer), selectedOption(0), running(true) {
        font = TTF_OpenFont("assets/fonts/poppins.ttf", 32);
        if (!font) {
            std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
            running = false;
        }
        options = {"Iniciar Jogo", "Configurações", "Sair"};

        normalColor = {255, 255, 255, 255}; // Branco
        selectedColor = {255, 0, 0, 255}; // Vermelho
    }

    Menu::~Menu() {
        if (font) {
            TTF_CloseFont(font);
        }
    }

    void Menu::handleInput(SDL_Event &event) {
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    selectedOption = (selectedOption - 1 + options.size()) % options.size();
                    break;
                case SDLK_DOWN:
                    selectedOption = (selectedOption + 1) % options.size();
                    break;
                case SDLK_RETURN:
                    if (selectedOption == 0) {
                        running = false;
                    } else if (selectedOption == 1) {
                        std::cout << "Configurações selecionadas (não implementado)" << std::endl;
                    } else if (selectedOption == 2) {
                        running = false;
                    }
                    break;
            }
        }
    }

    void Menu::update() {
    }

    void Menu::render() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        int y = 200;
        for (size_t i = 0; i < options.size(); ++i) {
            SDL_Color color = (i == selectedOption) ? selectedColor : normalColor;

            SDL_Surface *surface = TTF_RenderText_Solid(font, options[i].c_str(), color);
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

            SDL_Rect destRect;
            destRect.x = 400;
            destRect.y = y;
            destRect.w = surface->w;
            destRect.h = surface->h;

            SDL_RenderCopy(renderer, texture, nullptr, &destRect);

            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);

            y += 50;
        }

        SDL_RenderPresent(renderer);
    }

RB_ENGINE_END_NS
