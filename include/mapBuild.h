#ifndef MAPBUILD_H
#define MAPBUILD_H
#include <Infrastructure.h>

class MapBuild {
    private:
        SDL_Window* window;
        SDL_Surface* screenSurface;
        SDL_Surface* background;
        SDL_Renderer* renderer;
        SDL_Texture* screenTexture;
        SDL_Rect* boundary;
        //helper functions
        int isCorner(int x, int y, uint8_t * pixels);
        bool isGreen(int x, int y, uint8_t * pixels);
    public:
        MapBuild();
        ~MapBuild();
        bool init();
        bool loadMedia();
        void close();
        void buildIntersections(Infrastructure * inf);
        void testRectangle();
        void renderBackground();
};


#endif