//
// Created by bortoli on 07/03/25.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <SDL_render.h>

#include "EngineDefines.h"

RB_ENGINE_NS
    class Player {
    public:
        explicit Player(SDL_Renderer **renderer);

        ~Player() = default;

        bool init();

        void render();

    private:
        void handleInput();

        SDL_Texture *loadTexture(const char *path, SDL_Renderer *renderer);

        SDL_Renderer *renderer;
        SDL_Texture *texture;
        int x = 0, y = 0, velocity = 0;
    };

RB_ENGINE_END_NS


#endif //PLAYER_H
