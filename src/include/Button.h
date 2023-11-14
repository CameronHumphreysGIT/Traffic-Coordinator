#ifndef BUTTON_H
#define BUTTON_H
#include <SDL.h>
#include <vector>

class Button {
    private:
        SDL_Rect* borders;
        std::vector<int> colour;
        const char* text;
    public:
        Button(int , int , int , int );
        ~Button();
        bool isClicked(SDL_Event &e);
        bool isWithin(int , int );
        SDL_Rect* getBorders();
        std::vector<int> getColour();
        const char* getText();
};


#endif