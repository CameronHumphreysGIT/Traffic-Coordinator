#ifndef SCENE_H
#define SCENE_H
#include <SDL2/SDL.h>
#include <vector>

class Scene {
    private:
        SDL_Renderer* renderer;
        SDL_Texture* backgroundTexture;
        SDL_Rect* boundary;
    public:
        Scene(SDL_Renderer* &, SDL_Texture* );
        ~Scene();
        void draw(std::vector<SDL_Rect*> , std::vector<std::pair<float, float>> );
};


#endif