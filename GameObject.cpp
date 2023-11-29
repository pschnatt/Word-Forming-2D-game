#include "GameObject.h"
#include "Game.h"
#include "TextureManager.h"


//GameObject constructor responsible for initializing a GameObject object with the provided texture, position, and dimensions.
GameObject::GameObject(const char* texturesheet, int x,int y,int width,int height,int srcw,int srch):objTexture(TextureManager::LoadTexture(texturesheet)),xpos(x),ypos(y),objwidth(width),objheight(height),srcwidth(srcw),srcheight(srch)
{};

//GameObject destructer responsible for cleaning up any resources allocated by the object before its memory is freed.
GameObject::~GameObject(){};

//GameObject function responsible for updating the position of the Object's on the screen.
void GameObject::Update(bool bg)
{   
    if (bg == true){
    srcRect.h = 600;
    srcRect.w = 800;
    srcRect.x = 0;
    srcRect.y = 0;
    }
    srcRect.h = 2000;
    srcRect.w = 2000;
    srcRect.x = 0;
    srcRect.y = 0;
    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = objwidth;
    destRect.h = objheight;
}

//GameObject function responsible for rendering the object's texture on the screen.
void GameObject::Render(bool bg)
{   
    if (bg == true)
    {
        SDL_RenderCopy(Game::renderer, objTexture, NULL, NULL);
    }
    else
    { 
    SDL_RenderCopy(Game::renderer,objTexture,&srcRect,&destRect);
    }
}