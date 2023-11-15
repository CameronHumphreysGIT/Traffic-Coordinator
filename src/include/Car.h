#ifndef CAR_H
#define CAR_H
#include <utility>
#include <vector>
#include <SDL.h>

#include <MathHelper.h>

class Car {
    private:
        std::vector<std::vector<std::pair<float, float>>>* paths;
        //current path from the paths
        int currentPath;
        //each curved path is made up of waypoints, this is the current waypoint.
        int currentWaypoint;
        SDL_Rect* chassis;
        enum STATE { rest, moving};
        STATE state;
        //time of last Update in seconds
        float lastUpdate;
        //speed
        float speed;
        //These variables will accumulate the sub pixel updates to position.
        std::pair<float, float> sums;
        //keeps track of the rotation of the car in degrees
        float rotation;
    public:
        Car(std::pair<float, float> , float);
        ~Car();
        SDL_Rect* getChassis();
        float* getRotation();
        void addPath(std::vector<std::pair<float, float>>);
        void update(float );
        void updatePos(float );
        void translate(std::pair<float, float> , Vector2 , float );
        void rotate(Vector2 );
        bool isNotRest();
};

#endif