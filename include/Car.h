#ifndef CAR_H
#define CAR_H
#include <utility>
#include <vector>
#include <SDL2/SDL.h>

class Car {
    private:
        std::vector<std::vector<std::pair<float, float>>>* paths;
        //current path from the paths
        int currentPath;
        //each curved path is made up of waypoints, this is the current waypoint.
        int currentWaypoint;
        SDL_Rect chassis;
        enum STATE { rest, moving};
        STATE state;
        //time of last Update in seconds
        float lastUpdate;
        //speed
        float speed;
        //These variables will accumulate the sub pixel updates to position.
        std::pair<float, float> sums;
    public:
        Car(std::pair<float, float> , float);
        ~Car();
        SDL_Rect* getChassis();
        void addPath(std::vector<std::pair<float, float>>);
        void update(float );
        void updatePos(float );

};

#endif