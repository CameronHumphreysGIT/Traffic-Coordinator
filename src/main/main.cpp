#include <iostream>
#include <vector>
#include <string>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <MapBuild.h>

int main() {
    mapBuild builder = mapBuild();
    builder.init();
    builder.loadMedia();
    Infrastructure *infrastructure = new Infrastructure();
    builder.buildIntersections(infrastructure);
    infrastructure->print();
    SDL_Event e;
    bool quit = false;
    while( quit == false )  {
        while( SDL_PollEvent( &e ) )    {
            if( e.type == SDL_QUIT ) quit = true;
        }
    }


    return 0;
}