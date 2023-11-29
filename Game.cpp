#include "Game.h"
#include "GameObject.h"
#include "Player.h"
#include "algorithm"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define FULL_PLAYERHPS 150

//const ดีกว่า
#define BUTTON_ROWS 4
#define BUTTON_COLS 4
#define BUTTON_WIDTH 60
#define BUTTON_HEIGHT 50
#define BUTTON_MARGIN 10
#define TEXTBOX_HEIGHT BUTTON_HEIGHT
#define TEXTBOX_WIDTH (BUTTON_WIDTH + BUTTON_MARGIN) * BUTTON_COLS
#define TEXTBOX_MARGIN BUTTON_MARGIN * 25


//Declare Renderer and Background
GameObject* mainMenubackground;
GameObject* background;
GameObject* playerheart;
GameObject* enemyheart;
GameObject* Gameover;
SDL_Renderer* Game::renderer = nullptr;

//Declare Players
Player* player;
Player* enemy1;
Player* enemy2;
Player* enemy3;
Player* enemy4;

//Time Delay before Quit by GameOver
int delay = 0;




Game::Game()
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

