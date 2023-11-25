#ifndef SYSTEM_H
#define SYSTEM_H
#include <SDL.h>
#include <vector>
#include <Scene.h>
#include <Infrastructure.h>
#include <Car.h>
#include <CarHandler.h>
#include <Button.h>

class System {
    private:
        SDL_Window* window;
        SDL_Surface* screenSurface;
        SDL_Renderer* renderer;
        SDL_Texture* backgroundTexture;
        SDL_Texture* intersectionsBackground;
        SDL_Texture* satalliteBackground;
        SDL_Texture* carTexture;
        Infrastructure* infrastructure;
        CarHandler* carHandler;
        Scene* scene;
        //time elapsed since SDL library initialized
        Uint32 time;
        Button* toggleBackground;
        TTF_Font* font;
    public:
        System();
        ~System();
        bool init();
        bool loadMedia(bool);
        void buildInfrastructure();
        void swapBackground();
        std::vector<std::vector<Intersection*>*>* getIntersections();
        CarHandler* getCarHandler();
        void scenario(int );
        void draw();
        void run();
        void run(int );
        void close();

};


#endif