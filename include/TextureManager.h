#include "Game.h"

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H


class TextureManager
{
    public:
        static SDL_Texture* LoadTexture(const char* fileName, SDL_Renderer* ren);
};

#endif // TEXTUREMANAGER_H
