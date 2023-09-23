#ifndef CAR_H
#define CAR_H
#include <utility>
#include <SDL2/SDL.h>

class Car {
    private:
        std::pair<int, int> direction;
        float speed;
        SDL_Rect chassis;
        //the time we last moved
        Uint64 prevMove;
    public:
        Car(SDL_Rect, Uint64);
        void updatePos(Uint64 );
        SDL_Rect getChassis();
        void setDirection(std::pair<int, int>);
};

#endif