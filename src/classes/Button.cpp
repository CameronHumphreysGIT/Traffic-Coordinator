#include <Button.h>
#include <Variables.h>
#include <iostream>

using namespace std;

Button::Button(int x, int y, int w, int h) {
    borders = new SDL_Rect();
    borders->x = x;
    borders->y = y;
    borders->w = w;
    borders->h = h;
    colour = Variables::BUTTONCOLOUR;
    text = "Satallite Off";
}

Button::~Button() {  
    delete borders;
}

//changes the color of the button and hadles the button click
bool Button::isClicked(SDL_Event &e) {
    int x,y;
    SDL_GetMouseState(&x, &y);
    switch(e.type) {
        case SDL_MOUSEMOTION:
            if (isWithin(x,y)) {
                colour = Variables::HOVERCOLOUR;
            }else {
                colour = Variables::BUTTONCOLOUR;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            //mouse clicked
            if (isWithin(x,y)) {
                if (strcmp(text,"Satallite Off") == 0) {
                    text = "Satallite On";
                }else {
                    text = "Satallite Off";
                }
                return true;
            }
            break;
    }
    return false;
}

bool Button::isWithin(int x, int y) {
    if (x > borders->x && y > borders->y) {
        //beyond top right and left
        if (x < (borders->x + borders->w) && y < (borders->y + borders->h)) {
            //within
            return true;
        }
    }
    return false;
}

SDL_Rect* Button::getBorders() {
    return borders;
}

vector<int> Button::getColour() {
    return colour;
}

const char* Button::getText() {
    return text;
}