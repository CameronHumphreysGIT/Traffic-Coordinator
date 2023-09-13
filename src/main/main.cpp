#include <iostream>
#include <vector>
#include <string>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <mapBuild.h>

int main() {
    mapBuild builder = mapBuild();
    builder.init();
    builder.loadMedia();
    SDL_Event e;
    bool quit = false;
    while( quit == false )  {
        while( SDL_PollEvent( &e ) )    {
            if( e.type == SDL_QUIT ) quit = true; 
        }
    }

    return 0;
}