//
// Created by bortoli on 11/03/25.
//

#ifndef COMPONENT_H
#define COMPONENT_H
#include "EngineDefines.h"

RB_ENGINE_NS
    class Component {
    public:
        virtual ~Component();

        virtual void init();

        virtual void update(float deltaTime);

        virtual void render(SDL_Renderer *renderer);
    };

RB_ENGINE_END_NS

#endif //COMPONENT_H
