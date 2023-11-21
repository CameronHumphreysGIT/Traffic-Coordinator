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
    system->scenario(3);

    system->run();

    system->close();
    //Delete all objects;
    delete system;

    return 0;
}