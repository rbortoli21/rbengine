//
// Created by bortoli on 08/03/25.
//

#ifndef GAME_H
#define GAME_H
#include "EngineDefines.h"
#include "Player.h"
#include "Window.h"

RB_ENGINE_NS
    class Game {
    public:
        Game();

        ~Game() = default;

        [[nodiscard]] bool init() const;

        void run() const;

    private:
        SDL_Renderer *renderer;
        Window *window;
        Player *player;
    };

RB_ENGINE_END_NS


#endif //GAME_H
