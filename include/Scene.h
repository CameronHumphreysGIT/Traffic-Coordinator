#ifndef SCENE_H
#define SCENE_H
#include <SDL2/SDL.h>
#include <vector>

class Scene {
    private:
        SDL_Renderer* renderer;
        SDL_Rect* boundary;
    public:
        Scene(SDL_Renderer* &);
        ~Scene();
        void drawBackground(SDL_Texture *);
        void drawRoads(std::vector<std::vector<std::vector<std::pair<float, float>>>> );
        void drawCars(std::vector<SDL_Rect*> , SDL_Texture* , std::vector<float*> );
        void present();
};


#endif