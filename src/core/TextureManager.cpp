//
// Created by bortoli on 01/04/25.
//

#include "TextureManager.h"

#include <iostream>
#include <ostream>
#include <SDL_image.h>

RB_ENGINE_NS
    bool TextureManager::load(const std::string &id, const std::string &filename,
                              SDL_Renderer *renderer) {
        SDL_Surface *tempSurface = IMG_Load(filename.c_str());
        if (!tempSurface) {
            std::cerr << "Failed to load image: " << filename << " "
                    << IMG_GetError() << std::endl;
            return false;
        }

        SDL_Texture *texture =
                SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);

        if (!texture) {
            std::cerr << "Failed to create texture from surface: "
                    << SDL_GetError() << std::endl;
            return false;
        }

        textureMap[id] = texture;
        return true;
    }

    void TextureManager::draw(const std::string &id, const int x, const int y, const int width,
                              const int height, SDL_Renderer *renderer,
                              const SDL_RendererFlip flip) {
        SDL_Rect srcRect;
        SDL_Rect destRect;

        srcRect.x = 0;
        srcRect.y = 0;
        srcRect.w = width;
        srcRect.h = height;

        destRect.x = x;
        destRect.y = y;
        destRect.w = width;
        destRect.h = height;

        if (textureMap.contains(id)) {
            SDL_RenderCopyEx(renderer, textureMap[id], &srcRect, &destRect, 0,
                             nullptr, flip);
        } else {
            std::cerr << "Attempted to draw unknown texture ID: " << id
                    << std::endl;
        }
    }

    void TextureManager::drawFrame(const std::string &id, int x, int y, int width,
                                   int height, int currentRow, int currentFrame,
                                   SDL_Renderer *renderer, double angle,
                                   SDL_Point *center, SDL_RendererFlip flip) {
        SDL_Rect srcRect;
        SDL_Rect destRect;

        srcRect.x = width * currentFrame;
        srcRect.y = height * currentRow;
        srcRect.w = width;
        srcRect.h = height;

        destRect.x = x;
        destRect.y = y;
        destRect.w = width;
        destRect.h = height;

        if (textureMap.contains(id)) {
            SDL_RenderCopyEx(renderer, textureMap[id], &srcRect, &destRect,
                             angle, center, flip);
        } else {
            std::cerr << "Attempted to drawFrame for unknown texture ID: " << id
                    << std::endl;
        }
    }

    void TextureManager::clearTextureMap() {
        for (auto const &[id, texture]: textureMap) {
            SDL_DestroyTexture(texture);
        }
        textureMap.clear();
    }

RB_ENGINE_END_NS
