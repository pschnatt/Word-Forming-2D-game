#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

//Texture Manager Class
class TextureManager {
    public:
        static SDL_Texture* LoadTexture(const char* filename);
};



#endif