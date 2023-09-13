#ifndef MAPBUILD_H
#define MAPBUILD_H
#include <SDL2/SDL.h>

class mapBuild {
    private:
        SDL_Window* gWindow;
        SDL_Surface* gScreenSurface;
        SDL_Surface* gBackground;
    public:
        mapBuild();
        bool init();
        bool loadMedia();
        void close();
        bool buildIntersections();
};


#endif