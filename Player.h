#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// Player and Enemy class
class Player
{
public:
    friend class Game;
    Player(std::string idlefile, std::string attackfile, std::string attackfile2, std::string runback, std::string deadani, int hps, int baseatk, bool isenemy); // Constructor
    ~Player();                                                                                                                                                   // Destructor

    bool ismoving = false;
    bool isreturn = false;
    bool isattacking = false;
    bool isidle = true;
    bool isdead = false;
    bool isDeathAnimationComplete = false;
    bool deadstatecomplete = false;
    void Attack();
    void Draw(int x, int y, int w, int h);
    void Update(int x, int y, int w, int h, int animationframeidle, int animationframeattack, int animationatk, int animationrunback, int animationdeath);
    void Render();
    int gethps();
    void setletter_point(int letterdmg);
    bool playerisdead() { return player_death; }
    void dealdmg();
    void settarget(Player *target);
    SDL_Rect GetDstRect() const { return dstRect; }

private:
    bool player_death = false;
    bool isenemy;
    bool atstart = false;
    int healthpts;
    int attack_point;
    int letter_point;
    Player *attack_target;

    // animation
    int startx;
    int newX;
    int newestX;
    int ypos;
    int xpos;
    int frame = 0;
    double pla_frame;
    double plaatk_frame;
    double playeratk2_frame;
    double runback_frame;
    double death_frame;
    SDL_Rect srcRect;
    SDL_Rect dstRect;
    SDL_Texture *idle_texture;
    SDL_Texture *move_texture;
    SDL_Texture *attack_texture;
    SDL_Texture *run_texture;
    SDL_Texture *death_texture;

};

#endif