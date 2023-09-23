#include <iostream>
#include <vector>
#include <string>
#define SDL_MAIN_HANDLED
#include <MapBuild.h>
#include <Car.h>
#include <utility>

using namespace std;

int main() {
    mapBuild builder = mapBuild();
    builder.init();
    builder.loadMedia();
    Infrastructure *infrastructure = new Infrastructure();
    builder.buildIntersections(infrastructure);
    infrastructure->print();
    SDL_Event e;
    bool quit = false;
    //get the top right of the first intersection
    int startX = infrastructure->getI(0)->getCorners().at(1).first;
    int startY = infrastructure->getI(0)->getCorners().at(1).second;
    SDL_Rect* rect = new SDL_Rect{startX,startY,10,20};
    Car *c = new Car(*rect, SDL_GetTicks64());
    pair<int, int> p = {100, 0};
    c->setDirection(p);
    while( quit == false )  {
        //Render the screen;
        c->updatePos(SDL_GetTicks64());
        SDL_Rect temp = c->getChassis();
        builder.testRectangle(rect);
        while( SDL_PollEvent( &e ) )    {
            if( e.type == SDL_QUIT ) quit = true;
        }

    }
    builder.close();

    return 0;
}