#ifndef GAMEPLAYER_H
#define GAMEPLAYER_H

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


#endif