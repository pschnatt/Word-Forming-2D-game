#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <ctime>
#include <algorithm>
#include <string>

// #include "Game.h"
#include "RandomLetter.h"
// #include "Player.h"
#include "GameObject.h"
#include "TextureManager.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define FPS 60
#define frameDelay 1000/FPS
#define MAX_LETTER 16
#define MIN_LETTER 3
#define FULL_PLAYERHPS 150
#define BUTTON_ROWS 4
#define BUTTON_COLS 4
#define BUTTON_WIDTH 60
#define BUTTON_HEIGHT 50
#define BUTTON_MARGIN 10
#define TEXTBOX_HEIGHT BUTTON_HEIGHT
#define TEXTBOX_WIDTH (BUTTON_WIDTH + BUTTON_MARGIN) * BUTTON_COLS
#define TEXTBOX_MARGIN BUTTON_MARGIN * 25

/*----------------------------------------------------------------------------------------------------------------------*/

//restored health
bool stage1_clear = false;
bool stage2_clear = false;
bool stage3_clear = false;
int delay = 0;


//Player and Enemy class
class Player{
    public:
Player(std::string idlefile, std::string attackfile, std::string attackfile2, std::string runback,std::string deadani,int hps,int baseatk, bool isenemy);        // Destructor
~Player();
    
    bool ismoving = false;
    bool isreturn = false;
    bool isattacking = false;
    bool isidle = true;
    bool isdead = false;
    bool isDeathAnimationComplete = false;
    bool deadstatecomplete = false;
    void Attack();
    void Draw(int x, int y, int w, int h);
    void Update(int x,int y, int w, int h,int animationframeidle, int animationframeattack, int animationatk, int animationrunback, int animationdeath);
    void Render();
    int  gethps();
    void setletter_point(int letterdmg);
    bool playerisdead(){return player_death;}
    void dealdmg();
    void settarget(Player* target);
    SDL_Rect GetDstRect() const {return dstRect;}

    private:
    bool player_death = false;
    bool isenemy ;
    bool atstart = false;
    int healthpts;
    int attack_point;
    int letter_point;
    Player* attack_target;


    //animation
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
    SDL_Texture* idle_texture;
    SDL_Texture* move_texture;
    SDL_Texture* attack_texture;
    SDL_Texture* run_texture;
    SDL_Texture* death_texture;
};


//Declare Players
Player* player;
Player* enemy1;
Player* enemy2;
Player* enemy3;
Player* enemy4;

//Game Class
class Game {
public:
    Game(Player* player,Player* enemy1,Player* enemy2,Player* enemy3,Player* enemy4);
    ~Game();

    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen,std::vector<std::string>& vecofranlet);
    void handleEvents();
    void update(std::vector<std::string>& randomletter,std::vector<std::string> dictionary);
    void render();
    void clean();
    inline bool isRunning() const { return running; }
    static SDL_Renderer* renderer;

private:
    bool gameOver = false;
    bool running;
    bool inMenu = true;
    SDL_Window* window;
    TTF_Font* font;
    struct Button {
        int x;
        int y;
        int width;
        int height;
        std::string letter;
        SDL_Texture* texture;
        std::string text;
        bool selected = false;
        bool letter_used = false;
    };
    std::vector<Button> buttons;
    void createButtons();
    void renderButton(Button& button);
    void UpdateButtons(std::vector<std::string>& randomletter,std::vector<std::string> dictionary);
    struct TextBox {
        int x;
        int y;
        int width;
        int height;
        std::string text;
        SDL_Texture* texture;
    };
    TextBox textBox;
    void createTextBox();
    void renderTextBox();
};


SDL_Renderer* Game::renderer = nullptr;






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
inline void Player::setletter_point(int letterdmg){letter_point = letterdmg;}

// ----------------------------------------------------------------------------------------------------------------------------------------------

GameObject* background ;
GameObject* mainMenubackground;
GameObject* playerheart ;
GameObject* Gameover ;
GameObject* enemyheart;



Game::Game(Player* player,Player* enemy1,Player* enemy2,Player* enemy3,Player* enemy4)
{}

Game::~Game()
{}

//Function to initialize the game
void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen,std::vector<std::string>& vecofranlet)
{   
    if (SDL_Init(SDL_INIT_VIDEO) != 0 || TTF_Init() != 0) {
    std::cout << "Failed to initialize SDL or SDL_ttf: " << SDL_GetError() << std::endl;//if fail to initialize SDL or SDL_ttf
    // Handle the error
}
    font = TTF_OpenFont("assets/Roboto-Black.ttf", 24);
    if (!font) {
    std::cout << "Failed to load font: " << TTF_GetError() << std::endl;//if fail to load font
    // Handle the error
}

    int flags = 0;//fullscreen
    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

    if (SDL_Init(SDL_INIT_VIDEO) == 0)//initialize the game window
    {   
        
        std::cout << "SDL Initialized" << std::endl;
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (window)
        {
            std::cout << "Window created" << std::endl;
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            std::cout << "Renderer created" << std::endl;
        }

        createButtons();
        createTextBox();

        running = true;

    }
    background = new GameObject("assets/castlemain.jpg",0,0,800,600); //make background
    mainMenubackground = new GameObject("assets/castlemenu.jpg",0,0,800,600); //make background
    playerheart = new GameObject("assets/heart.png",30,WINDOW_HEIGHT-100,100,100,2000,2000);
    Gameover = new GameObject("assets/go1.png", (WINDOW_WIDTH/2)-500, (WINDOW_HEIGHT/2)-600,1000,1000,360,360);
    enemyheart = new GameObject("assets/heart.png",WINDOW_WIDTH-130,WINDOW_HEIGHT-100,100,100,2000,2000);
    player = new Player("assets/Idle_hero.png","assets/Run.png","assets/Attack2.png","assets/runflip.png","assets/Death.png",FULL_PLAYERHPS,999,false);//make player
    enemy1 = new Player("assets/flipidle.png","assets/runflip.png","assets/Attack2.png","assets/Run.png","assets/Death.png",150,200,true);//make player
    enemy2 = new Player("assets/evilIdle.png","assets/evilrun.png","assets/evilattack2.png","assets/evilrunback.png","assets/evildeath.png",200,10,true);
    enemy3 = new Player("assets/fanidle.png","assets/fanrunleft.png","assets/fanattack.png","assets/fanrunright.png","assets/fandeath.png",225,10,true);
    enemy4 = new Player("assets/CatIdle.png","assets/CatWalk.png","assets/catattack.png","assets/CatRun.png","assets/CatLicking.png",999,999,true);

    
    
}

//Game funtion to handle events from mouse and keyboard
void Game::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    int mouseX, mouseY;
    switch (event.type)
    {
    case SDL_QUIT:
        running = false;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
            if (inMenu){
            std::cout << "Game stopped by Esc" << std::endl;
            running = false;}
            else{
                inMenu = true;
            }
            break;
        }
    case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT) {
            mouseX = event.button.x;
            mouseY = event.button.y;
            if (inMenu) {
                    // Check if the "Play" button is clicked
                    if (mouseX >= (WINDOW_WIDTH/2)-115 && mouseX <= (WINDOW_WIDTH/2)+135 && mouseY >= (WINDOW_HEIGHT/2)-50 && mouseY <= (WINDOW_HEIGHT/2)+50) {
                        inMenu = false;
                        // Start the main game here
                        std::cout << "Starting the game..." << std::endl;
                    }
            }
            else{
            for (auto& button : buttons) {
                if (mouseX >= button.x && mouseX <= button.x + button.width &&
                    mouseY >= button.y && mouseY <= button.y + button.height && !button.letter_used) {
                    // Button is clicked
                    button.selected = true;
                    std::cout << "Button clicked: " << button.text<< std::endl;
                    }
                }
            }
        break;
    default:
        break;
        }
    }
}

//Game function use to update the frame
void Game::update(std::vector<std::string>& randomletter,std::vector<std::string> dictionary)
{   
    UpdateButtons(randomletter,dictionary);
    background->Update(true);
    mainMenubackground->Update(true);
    playerheart ->Update(false);
    enemyheart -> Update(false);
    Gameover->Update(false);
    player->Draw(-150,111,400,400);
    player->Update(0,0,180,180,10,7,6,6,10);
    
    
    if (enemy1 -> isDeathAnimationComplete == false){
        enemy1->Draw(950,111,400,400);
        enemy1->Update(0,0,180,180,10,7,6,6,10);
        }
    else if (enemy1 -> isDeathAnimationComplete == true && enemy2 -> isDeathAnimationComplete == false){
        enemy2->Draw(950,111,400,400);
        enemy2->Update(0,0,250,250,7,7,7,7,6);
        }
    else if (enemy2 -> isDeathAnimationComplete == true && enemy3 -> isDeathAnimationComplete == false){
        enemy3->Draw(950,111,400,400);
        enemy3->Update(0,0,162,162,9,7,6,7,6);
        }
    else if (enemy3 -> isDeathAnimationComplete == true && enemy4 -> isDeathAnimationComplete == false){
        enemy4->Draw(950,111,400,400);
        enemy4->Update(0,0,50,50,9,7,4,7,4);
        }

    if (player->playerisdead()){
        gameOver = true;
    }
    
    
}

//Game function use to render
void Game::render()
{   
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    if (inMenu) {

        mainMenubackground->Render(true);
        // Render the game menu
        SDL_Rect playButtonRect = { (WINDOW_WIDTH/2)-115,(WINDOW_HEIGHT/2)-50, 250, 100}; 
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &playButtonRect);

        // Render the "Play" button text
        SDL_Color textColor = { 255, 255, 255, 255 };
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Play", textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = { (WINDOW_WIDTH/2)-55, (WINDOW_HEIGHT/2)-30, 125, 55 };
        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);

        // Render title text
        SDL_Color textColor2 = { 0, 0, 0, 0 };
        SDL_Surface* textSurface2 = TTF_RenderText_Solid(font, "WORD KNIGHTMARE", textColor2);
        SDL_Texture* textTexture2 = SDL_CreateTextureFromSurface(renderer, textSurface2);
        SDL_Rect textRect2 = { (WINDOW_WIDTH/2)-225, (WINDOW_HEIGHT/2)-150, 450, 80 };
        SDL_RenderCopy(renderer, textTexture2, nullptr, &textRect2);

        SDL_DestroyTexture(textTexture2);
        SDL_FreeSurface(textSurface2);
        
        // Render instruction text
        SDL_Color textColor3 = { 0, 0, 0, 0 };
        SDL_Surface* textSurface3 = TTF_RenderText_Solid(font, "PRESS [ESC] TO QUIT", textColor3);
        SDL_Texture* textTexture3 = SDL_CreateTextureFromSurface(renderer, textSurface3);
        SDL_Rect textRect3 = { (WINDOW_WIDTH/2)-225, (WINDOW_HEIGHT/2)+70, 450, 80 };
        SDL_RenderCopy(renderer, textTexture3, nullptr, &textRect3);

        SDL_DestroyTexture(textTexture3);
        SDL_FreeSurface(textSurface3);

    } else {
    background->Render(true);
    
    player->Render();

    if (enemy1 -> isDeathAnimationComplete == false){
        enemy1->Render();
    }
    else if (enemy1 -> isDeathAnimationComplete == true && enemy2 -> isDeathAnimationComplete == false){
        enemy2->Render();
    }
    else if (enemy2 -> isDeathAnimationComplete == true && enemy3 -> isDeathAnimationComplete == false){
        enemy3->Render();
    }
    else if (enemy3 -> isDeathAnimationComplete == true && enemy4 -> isDeathAnimationComplete == false){
        enemy4->Render();
    }
    
    renderTextBox();

    for (auto& button : buttons) {
        renderButton(button);
    }
    
    playerheart->Render();
    enemyheart->Render();
    
    
    std::string numberHps = std::to_string(player->gethps());
    if (player->gethps() <= 0){
        numberHps = "0";
    }
    SDL_Color hps = { 0, 0, 0, 0 };
    SDL_Surface* hpsurface = TTF_RenderText_Solid(font, numberHps.c_str(), hps); 
    SDL_Texture* hpTexture = SDL_CreateTextureFromSurface(renderer, hpsurface);
    SDL_Rect hpRect2 = { 55, WINDOW_HEIGHT-80, 50, 50 };
    SDL_RenderCopy(renderer, hpTexture, nullptr, &hpRect2);

    std::string numbereHps;
    if (enemy1 -> isDeathAnimationComplete == false){
                numbereHps = std::to_string(enemy1->gethps());
                }
            else if (enemy1 -> isDeathAnimationComplete == true && enemy2 -> isDeathAnimationComplete == false){
                numbereHps = std::to_string(enemy2->gethps());
                }
            else if (enemy2 -> isDeathAnimationComplete == true && enemy3 -> isDeathAnimationComplete == false){
                numbereHps = std::to_string(enemy3->gethps());
                }
            else if (enemy3 -> isDeathAnimationComplete == true && enemy4 -> isDeathAnimationComplete == false){
                numbereHps = std::to_string(enemy4->gethps());
                }
    
    SDL_Color ehps = { 0, 0, 0, 0 };
    SDL_Surface* ehpsurface = TTF_RenderText_Solid(font, numbereHps.c_str(), ehps);
    SDL_Texture* ehpTexture = SDL_CreateTextureFromSurface(renderer, ehpsurface);
    SDL_Rect ehpRect2 = { WINDOW_WIDTH-105, WINDOW_HEIGHT-80, 50, 50 };
    SDL_RenderCopy(renderer, ehpTexture, nullptr, &ehpRect2);

    if (gameOver && player->isDeathAnimationComplete){
        Gameover->Render();
        delay++;
        if (delay == 250){
            running = false;
            std::cout << "Game Over" << std::endl;
        }
    }
    
    }
    SDL_RenderPresent(renderer);
    
}

//Game function use to quit
void Game::clean()
{   
    for (auto& button : buttons) {
        SDL_DestroyTexture(button.texture);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);

    TTF_Quit();
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}

//Game function to update the letter buttons
void Game::UpdateButtons(std::vector<std::string>& randomletter,std::vector<std::string> dictionary){
    int i = 0;
    for (auto& button:buttons){
        if (i==0){ //delete
            if(button.selected){
                for (auto& but : buttons){
                    but.selected = false;
                    but.letter_used = false;
                }
                textBox.text.clear();
            }
        }
        if (i==1){//enter
            if (button.selected){
                    if(std::find(dictionary.begin(),dictionary.end(),textBox.text)!=dictionary.end())//find if word is in the dictionary
                    {//if yes calculate score and replace letter
                        int j = 0;
                        int score = textBox.text.size();
                        player->Attack();
                        player->setletter_point(score);
                        std::cout << "Score: " << score << std::endl;
                        for (auto& butt : buttons)
                        {
                            if (butt.selected && j>=2){
                                randomletter[j-2] = "";
                                butt.selected = false;
                                butt.letter_used = false; 
                            }
                            j++;
                        }
                    }
                    else
                    {//if no clear textbox
                        if (enemy1 -> isDeathAnimationComplete == false){
                            enemy1->Attack();
                        }
                        else if (enemy1 -> isDeathAnimationComplete == true && enemy2 -> isDeathAnimationComplete == false){
                            enemy2->Attack();
                        }
                        else if (enemy2 -> isDeathAnimationComplete == true && enemy3 -> isDeathAnimationComplete == false){
                            enemy3->Attack();
                        }
                        else if (enemy3 -> isDeathAnimationComplete == true && enemy4 -> isDeathAnimationComplete == false){
                            enemy4->Attack();
                        }
                        int j = 0;
                        for (auto& butt : buttons){
                            if (butt.selected && j>=2){
                                butt.selected = false;
                                butt.letter_used = false; 
                            }
                            j++;
                        }
                        
                    }
                
                button.selected = false;
                textBox.text.clear();
                
            }
        }
        
        if (i>=2){ //not include delete and enter button
        button.text = randomletter[i-2];
        SDL_Surface* buttonTextSurface = TTF_RenderText_Solid(font, button.text.c_str(), { 255, 255, 255 });
        button.texture = SDL_CreateTextureFromSurface(renderer, buttonTextSurface);
        SDL_FreeSurface(buttonTextSurface);

        if (button.selected && !button.letter_used){
            textBox.text += button.text;
            button.letter_used = true;
        }

        }
        ++i;
    }
}

//Game function use to create buttons
void Game::createButtons()
{
    int buttonAreaWidth = BUTTON_COLS * BUTTON_WIDTH + (BUTTON_COLS - 1) * BUTTON_MARGIN;
    int buttonAreaHeight = BUTTON_ROWS * BUTTON_HEIGHT + (BUTTON_ROWS - 1) * BUTTON_MARGIN;

    int startX = (WINDOW_WIDTH - buttonAreaWidth) / 2;
    int startY = WINDOW_HEIGHT - buttonAreaHeight - BUTTON_MARGIN;


    Button del;
    del.x = startX + (BUTTON_WIDTH + BUTTON_MARGIN) * 4;
    del.y = startY + (BUTTON_HEIGHT + BUTTON_MARGIN) * 2;
    del.width = 100;
    del.height = 100;
    del.text = "Del";

    SDL_Surface* delbuttonTextSurface = TTF_RenderText_Solid(font, del.text.c_str(), { 255, 255, 255 });
    del.texture = SDL_CreateTextureFromSurface(renderer, delbuttonTextSurface);
    SDL_FreeSurface(delbuttonTextSurface);

    buttons.push_back(del);

    Button enter;
    enter.x = startX + (BUTTON_WIDTH + BUTTON_MARGIN) * 4;
    enter.y = startY + (BUTTON_HEIGHT + BUTTON_MARGIN) * 0;
    enter.width = 100;
    enter.height = 100;
    enter.text =   "ENTER";
    SDL_Surface* enterbuttonTextSurface = TTF_RenderText_Solid(font, enter.text.c_str(), { 255, 255, 255 });
    enter.texture = SDL_CreateTextureFromSurface(renderer, enterbuttonTextSurface);
    SDL_FreeSurface(enterbuttonTextSurface);
    buttons.push_back(enter);

    for (int row = 0; row < BUTTON_ROWS; row++) {
        for (int col = 0; col < BUTTON_COLS; col++) {
            Button button;
            button.x = startX + (BUTTON_WIDTH + BUTTON_MARGIN) * col;
            button.y = startY + (BUTTON_HEIGHT + BUTTON_MARGIN) * row;
            button.width = BUTTON_WIDTH;
            button.height = BUTTON_HEIGHT;

            buttons.push_back(button);
        }
    }
}

//Game function use to render in buttons
void Game::renderButton(Button& button)
{
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);  // Set the button color (red in this example)
    SDL_Rect buttonRect = { button.x, button.y, button.width, button.height };
    SDL_RenderFillRect(renderer, &buttonRect);  // Render the button rectangle


    SDL_Rect buttonTextRect;
    buttonTextRect.x = button.x + (button.width - button.text.length() * 10) / 2;
    buttonTextRect.y = button.y + (button.height - 24) / 2;
    buttonTextRect.w = button.text.length() * 10;
    buttonTextRect.h = 24;

    SDL_RenderCopy(renderer, button.texture, nullptr, &buttonTextRect);
}

//Game function use to create textbox for input 
void Game::createTextBox()
{
    textBox.x = (WINDOW_WIDTH - TEXTBOX_WIDTH ) / 2;
    textBox.y = WINDOW_HEIGHT - TEXTBOX_HEIGHT - TEXTBOX_MARGIN ;
    textBox.width = TEXTBOX_WIDTH;
    textBox.height = TEXTBOX_HEIGHT;
    textBox.text = "";
}

//Game function use to render the textbox
void Game::renderTextBox()
{
    SDL_Rect textBoxRect = { textBox.x, textBox.y, textBox.width, textBox.height };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &textBoxRect);

    // Render text
    SDL_Color textColor = { 0, 0, 0 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, textBox.text.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    int fontSize = 15;  
    int maxCharacters = 16;  
    int textWidth = fontSize * textBox.text.size() ;  

    int textHeight = fontSize*2;  

    SDL_Rect textRect = { textBox.x + 20, textBox.y + 10, textWidth, textHeight };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}



//main
int main(int argc, char* argv[])
{   

    try{
    Game* game = nullptr;
    Uint32 frameStart;
    int frameTime;
    std::string filename = "assets/dictionary.txt";
    RandomLetter randomlet = RandomLetter(filename);
    std::vector<std::string> dict = randomlet.getdict() ;

    std::vector<std::string> vecofranlet;
    for (int i = 0; i != MAX_LETTER;i++){
        vecofranlet.push_back("");
    }
    

    game = new Game(player,enemy1,enemy2,enemy3,enemy4);
    


    game->init("Grim", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, false,vecofranlet);
    

    while (game->isRunning())
    {   
        
        frameStart = SDL_GetTicks();

        game->handleEvents();
        
        game->update(vecofranlet,dict);
        game->render();
        randomlet.randomlt(vecofranlet);
        
        frameTime = SDL_GetTicks() -frameStart;
        if(frameDelay > frameTime){
            SDL_Delay(frameDelay-frameTime);
        }
    }

    game->clean();
    delete game;
    }
    catch(const std::bad_alloc& e){
        std::cerr << "Memory allocation failed: " << e.what() << std::endl;
    }

    return 0;
}
