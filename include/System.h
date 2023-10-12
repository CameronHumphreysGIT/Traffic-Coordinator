#ifndef SYSTEM_H
#define SYSTEM_H
#include <SDL2/SDL.h>
#include <vector>
#include <Scene.h>
#include <Infrastructure.h>

class System {
    private:
        SDL_Window* window;
        SDL_Surface* screenSurface;
        SDL_Renderer* renderer;
        SDL_Texture* backgroundTexture;
        Infrastructure* infrastructure;
        Scene* scene;
    public:
        System();
        ~System();
        bool init();
        bool loadMedia(bool);
        void close();
        void buildInfrastructure();
        void testdraw();
};


#endif