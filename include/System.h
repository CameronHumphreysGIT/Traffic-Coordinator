#ifndef SYSTEM_H
#define SYSTEM_H
#include <SDL2/SDL.h>
#include <vector>
#include <Scene.h>
#include <Infrastructure.h>
#include <Car.h>
#include <CarHandler.h>

class System {
    private:
        SDL_Window* window;
        SDL_Surface* screenSurface;
        SDL_Renderer* renderer;
        SDL_Texture* backgroundTexture;
        SDL_Texture* carTexture;
        Infrastructure* infrastructure;
        CarHandler* carHandler;
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
        void scenario(int );
        void draw();
        void run();
};


#endif