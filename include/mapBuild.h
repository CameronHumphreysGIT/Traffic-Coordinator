#ifndef MAPBUILD_H
#define MAPBUILD_H
#include <Infrastructure.h>

class mapBuild {
    private:
        SDL_Window* gWindow;
        SDL_Surface* gScreenSurface;
        SDL_Surface* gBackground;
        //helper functions
        int isCorner(int x, int y, uint8_t * pixels);
        bool isGreen(int x, int y, uint8_t * pixels);
    public:
        mapBuild();
        bool init();
        bool loadMedia();
        void close();
        void buildIntersections(Infrastructure * inf);
};


#endif