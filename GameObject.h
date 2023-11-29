#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

//GameObject Class
class GameObject{
    public:
        GameObject(const char* texturesheet,int x,int y,int width,int height,int srcw=0,int srch=0);
        ~GameObject();

        void Update(bool background = false);
        void Render(bool background = false);

    protected:
        int xpos;
        int ypos;
        int objwidth;
        int objheight;
        int srcheight;
        int srcwidth;

        SDL_Texture* objTexture;
        SDL_Rect srcRect, destRect;
};

#endif 