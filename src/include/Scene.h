#ifndef SCENE_H
#define SCENE_H
#include <SDL.h>
#include <vector>
#include <SDL_ttf.h>

class Scene {
    private:
        SDL_Renderer* renderer;
        SDL_Rect* boundary;
        TTF_Font* font;
    public:
        Scene(SDL_Renderer* &, TTF_Font* &);
        ~Scene();
        void clear();
        void drawBackground(SDL_Texture *);
        void drawRoads(std::vector<std::vector<std::vector<std::pair<float, float>>>> );
        void drawCars(std::vector<SDL_Rect*> , SDL_Texture* , std::vector<float*> );
        void drawLights(std::vector<std::vector<std::vector<std::vector<std::pair<float, float>>>>> );
        void drawButton(SDL_Rect* , std::vector<int> , const char *);
        void present();
};


#endif