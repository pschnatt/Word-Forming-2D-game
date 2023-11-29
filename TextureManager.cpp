#include "TextureManager.h"
#include "Game.h"

//TextureManager function responsible for loading an image file, creating an SDL texture from the loaded surface, and returning the result texture.
SDL_Texture* TextureManager::LoadTexture(const char* texture)
{
    SDL_Surface* tempSurface = IMG_Load(texture);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer,tempSurface);
    SDL_FreeSurface(tempSurface);

    return tex;
}