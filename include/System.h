#ifndef SYSTEM_H
#define SYSTEM_H
#include <SDL2/SDL.h>
#include <vector>
#include <Scene.h>
#include <Infrastructure.h>
#include <Car.h>

class System {
    private:
        SDL_Window* window;
        SDL_Surface* screenSurface;
        SDL_Renderer* renderer;
        SDL_Texture* backgroundTexture;
        SDL_Texture* carTexture;
        Infrastructure* infrastructure;
        std::vector<Car*>* cars;
        Scene* scene;
        //time elapsed since SDL library initialized
        Uint32 time;
    public:
        System();
        ~System();
        bool init();
        bool loadMedia(bool);
        void close();
        void buildInfrastructure();
        void testdraw();
        void run();
};


#endif