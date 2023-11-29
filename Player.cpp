#include "Player.h"
#include "Game.h"
#include "GameObject.h"

#define FULL_PLAYERHPS 150

//restored health
bool stage1_clear = false;
bool stage2_clear = false;
bool stage3_clear = false;

//Player Constructer
Player::Player(std::string idlefile, std::string attackfile, std::string attackfile2, std::string runback, std::string deadani, int hps = 150 ,int baseatk = 10, bool isenemy=false):healthpts(hps),attack_point(baseatk),letter_point(0), isenemy(isenemy){
    
    SDL_Surface* idlesurface = IMG_Load(idlefile.c_str());
    idle_texture = SDL_CreateTextureFromSurface(Game::renderer, idlesurface);
    SDL_FreeSurface(idlesurface);
    pla_frame = 0;

    SDL_Surface* attacksurface = IMG_Load(attackfile.c_str());
    move_texture = SDL_CreateTextureFromSurface(Game::renderer, attacksurface);
    SDL_FreeSurface(attacksurface);
    plaatk_frame = 0;

    SDL_Surface* attacksurface2 = IMG_Load(attackfile2.c_str());
    attack_texture = SDL_CreateTextureFromSurface(Game::renderer, attacksurface2);
    SDL_FreeSurface(attacksurface2);
    playeratk2_frame = 0;

    SDL_Surface* runflip = IMG_Load(runback.c_str());
    run_texture = SDL_CreateTextureFromSurface(Game::renderer, runflip);
    SDL_FreeSurface(runflip);
    runback_frame = 0;

    SDL_Surface* dead = IMG_Load(deadani.c_str());
    death_texture = SDL_CreateTextureFromSurface(Game::renderer, dead);
    SDL_FreeSurface(dead);
    runback_frame = 0;
}

//Player Deconstructer
Player::~Player(){
    SDL_DestroyTexture(idle_texture);
    SDL_DestroyTexture(move_texture);
    SDL_DestroyTexture(attack_texture);
    SDL_DestroyTexture(death_texture);
}

//Player attack animation Function
void Player::Attack(){
    ismoving = true;
    isidle = false;
}

//Player function use to deal Damage to target object
void Player::dealdmg(){
    attack_target->healthpts -= attack_point+letter_point;
}

//Player function use to get hps
int Player::gethps(){return healthpts;}

//Player function use to set target
void Player::settarget(Player* target){
    attack_target = target;
}

//Player function use to draw the player animation and get pos
void Player::Draw(int x, int y, int w, int h)
{   
    startx = x;
    if(isenemy == false){
        
        if(atstart && isidle == true)
        {   
            if (enemy1 -> isDeathAnimationComplete == false){
                enemy1->Attack();
                }
            else if (enemy1 -> isDeathAnimationComplete == true && enemy2 -> isdead == false){
                enemy2->Attack();
                }
            else if (enemy2 -> isDeathAnimationComplete == true && enemy3 -> isdead == false){
                enemy3->Attack();
                }
            else if (enemy3 -> isDeathAnimationComplete == true && enemy4 -> isdead == false){
                enemy4->Attack();
                }
            atstart = false;
        }

        else if(ismoving == true && isidle == false && isattacking == false && isdead == false)
        {
            int newX = dstRect.x + 5;
            dstRect.x = newX;
            dstRect.y = y;
            dstRect.w = w;
            dstRect.h = h;

            if(ismoving == true && newX > 850 && isattacking == false && isidle == false)  
            {
            ismoving = false;
            isattacking = true;
            dstRect.x = newX;
            dstRect.y = y;
            dstRect.w = w;
            dstRect.h = h;
            }
        }

        else if(ismoving == false && isidle == true && isattacking == false)
        {
            dstRect.x = x;
            dstRect.y = y;
            dstRect.w = w;
            dstRect.h = h;
        }

        else if(isreturn == true && isdead == false)
        {
            int xpos = dstRect.x - 4;
            if(xpos < x)
            {
                xpos = x;
                isreturn = false;
                isidle = true;
                isattacking = false;
                ismoving = false;
            }
            dstRect.x = xpos;
            dstRect.y = y;
            dstRect.w = w;
            dstRect.h = h;
            if(dstRect.x <= -100)
            {
                atstart = true;
            }
        }

        else if(isdead == true)
        {
            dstRect.x = x;
            dstRect.y = y;
            dstRect.w = w;
            dstRect.h = h;
        }
    }

    else if(isenemy)
    {
        if(ismoving == true && isidle == false && isattacking == false && isdead == false)
        {
            int newX = dstRect.x - 4;
            dstRect.x = newX;
            dstRect.y = y;
            dstRect.w = w;
            dstRect.h = h;

            if(ismoving == true && newX < 0 && isattacking == false && isidle == false)  
            {
            ismoving = false;
            isattacking = true;
            dstRect.x = newX;
            dstRect.y = y;
            dstRect.w = w;
            dstRect.h = h;
            }
        }

        else if(ismoving == false && isidle == true && isattacking == false)
        {
            dstRect.x = x;
            dstRect.y = y;
            dstRect.w = w;
            dstRect.h = h;
        }
        else if(isreturn == true && isdead == false)
        {
            int xpos = dstRect.x + 4;
            if(xpos > x)
            {
                xpos = x;
                isreturn = false;
                isidle = true;
                isattacking = false;
                ismoving = false;
            }
      
            dstRect.x = xpos;
            dstRect.y = y;
            dstRect.w = w;
            dstRect.h = h;
        }
        else if(isdead == true)
        {
            dstRect.x = x;
            dstRect.y = y;
            dstRect.w = w;
            dstRect.h = h;
        }
    }
}

//Player function use to update
void Player::Update(int x,int y, int w, int h,int animationframeidle , int animationframeattack, int animationatk, int animationrunback, int animationdeath) {

    if (enemy1 -> isDeathAnimationComplete == false){
        
        player->settarget(enemy1);
        enemy1->settarget(player);
        }
    else if (enemy1 -> isDeathAnimationComplete == true && enemy2 -> isDeathAnimationComplete == false){
        if (!stage1_clear){
            stage1_clear = true;
            player->healthpts = FULL_PLAYERHPS;
        }
        player->settarget(enemy2);
        enemy2->settarget(player);
        }
    else if (enemy2 -> isDeathAnimationComplete == true && enemy3 -> isDeathAnimationComplete == false){
        if (!stage2_clear){
            stage2_clear = true;
            player->healthpts = FULL_PLAYERHPS;
        }
        player->settarget(enemy3);
        enemy3->settarget(player);
        }
    else if (enemy3 -> isDeathAnimationComplete == true && enemy4 -> isDeathAnimationComplete == false){
        if (!stage3_clear){
            stage3_clear = true;
            player->healthpts = FULL_PLAYERHPS;
        }
        player->settarget(enemy4);
        enemy4->settarget(player);
        }
        
    //Change death state to true when hp less than 0
    if(healthpts <= 0)
    {   
        if (!isenemy){
            player_death = true;
        }
        isdead = true;
    }

    frame += 1;

    //Play the death animation
    if(isdead)
    {   
        if (!isDeathAnimationComplete){
        if (frame % 4 == 0) 
        {
            death_frame += 1;
        }
        }

        if (death_frame > animationdeath)
        {
            death_frame = animationdeath;
            isDeathAnimationComplete = true;
        }
        srcRect.x = x + w * death_frame;
        srcRect.y = y;
        srcRect.w = w;
        srcRect.h = h;
    }
    else if (ismoving == true && isidle == false && isattacking == false || isreturn == true)//walking animation
    {
        if (frame % 4 == 0) 
        {
            plaatk_frame += 1;
        }
        if (plaatk_frame > animationframeattack)
        {plaatk_frame = 0;}
        
        srcRect.x = x + w * plaatk_frame;
        srcRect.y = y;
        srcRect.w = w;
        srcRect.h = h;

    }
    else if(ismoving == false && isidle == true && isattacking == false)//idle animation
    {
        if (frame % 4 == 0) 
        {
            pla_frame += 1;
        }
        if (pla_frame > animationframeidle)
        {
            pla_frame = 0;
        }
        srcRect.x = x + w * pla_frame;
        srcRect.y = y;
        srcRect.w = w;
        srcRect.h = h; 
    }

    else if(ismoving == false && isidle == false && isattacking == true)//attacking animation and dealing damage 
    {
        if (frame % 4 == 0) 
        {
            playeratk2_frame += 1;
        }
        if (playeratk2_frame > animationatk)
        {
            playeratk2_frame = 0;
            isattacking = false;
            dealdmg();
            isreturn = true;
        }
        srcRect.x = x + w * playeratk2_frame;
        srcRect.y = y;
        srcRect.w = w;
        srcRect.h = h; 
    }
 
    }

//Player function to render the player
void Player::Render() {
    SDL_RendererFlip flip = SDL_FLIP_NONE;  // Default flip value
    if (isenemy)
    {
        flip = SDL_FLIP_HORIZONTAL;
    }

    if (isdead == true) 
    {
        SDL_RenderCopyEx(Game::renderer, death_texture, &srcRect, &dstRect, 0, nullptr, flip);
    } 
    else if (ismoving && isidle == false && isattacking == false) 
    {
        SDL_RenderCopy(Game::renderer, move_texture, &srcRect, &dstRect);
    } 
    else if(ismoving == false && isidle == false && isattacking == true)
    {
        SDL_RenderCopyEx(Game::renderer, attack_texture, &srcRect, &dstRect, 0, nullptr, flip);
    }
    else if(isreturn == true)
    {   
        
        SDL_RenderCopy(Game::renderer, run_texture, &srcRect, &dstRect);
    }
    else
    {   
        SDL_RenderCopy(Game::renderer, idle_texture, &srcRect, &dstRect);
    }
}

//Player function use to set letter point
void Player::setletter_point(int letterdmg){letter_point = letterdmg;}