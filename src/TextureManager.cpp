#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* fileName, SDL_Renderer* ren) {

    SDL_Surface* tmp = IMG_Load(fileName);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, tmp);
    SDL_FreeSurface(tmp);

    return texture;
}
