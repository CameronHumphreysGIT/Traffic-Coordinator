#include <iostream>
#include <vector>
#include <string>
#define SDL_MAIN_HANDLED
#include <System.h>
#include <Car.h>
#include <utility>

using namespace std;

int main() {
    System* system = new System();
    system->init();
    system->loadMedia(true);
    system->buildInfrastructure();
    SDL_Event e;
    bool quit = false;
    while( quit == false )  {
        //Render the screen;
        system->testdraw();
        while( SDL_PollEvent( &e ) )    {
            if( e.type == SDL_QUIT ) quit = true;
        }
    }
    system->close();
    //Delete all objects;
    delete system;

    return 0;
}