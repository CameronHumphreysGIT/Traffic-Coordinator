#include <Car.h>
#include <Variables.h>

using namespace std;

Car::Car(SDL_Rect r, Uint64 time) {
    prevMove = time;
    chassis = r;
    speed = Variables::DEFAULT_SPEED;
}
void Car::updatePos(Uint64 time) {
    Uint64 elapsed = prevMove - time;
    float distance = elapsed * speed;
    //we need the ration of x to y in the direction component:
    float ratio = direction.first/direction.second;
    //using some simple math, i know that:
    int y = sqrt(pow(distance, 2)/(pow(ratio, 2) + 1));
    int x = ratio * y;
    chassis.x = x;
    chassis.y = y;
}
SDL_Rect Car::getChassis() {
    return chassis;
}

void Car::setDirection(pair<int, int> dir) {
    direction = dir;
}