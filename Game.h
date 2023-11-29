#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

//Game Class
class Game {
public:
    Game();
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