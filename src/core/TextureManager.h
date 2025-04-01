//
// Created by bortoli on 01/04/25.
//

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include <map>
#include <SDL_render.h>
#include <string>

#include "EngineDefines.h"


RB_ENGINE_NS
    class TextureManager {
    public:
        static TextureManager &getInstance() {
            static TextureManager instance;
            return instance;
        }

        bool load(const std::string &id, const std::string &filename,
                  SDL_Renderer *renderer);

        void draw(const std::string &id, int x, int y, int width, int height,
                  SDL_Renderer *renderer, SDL_RendererFlip flip = SDL_FLIP_NONE);

        void drawFrame(const std::string &id, int x, int y, int width, int height,
                       int currentRow, int currentFrame, SDL_Renderer *renderer,
                       double angle = 0, SDL_Point *center = nullptr,
                       SDL_RendererFlip flip = SDL_FLIP_NONE);

        void clearTextureMap();

    private:
        TextureManager() = default;

        ~TextureManager() = default;

        TextureManager(const TextureManager &) = delete;

        TextureManager &operator=(const TextureManager &) = delete;

        std::map<std::string, SDL_Texture *> textureMap;
    };

RB_ENGINE_END_NS


#endif //TEXTUREMANAGER_H
