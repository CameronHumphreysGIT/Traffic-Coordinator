#ifndef CAR_H
#define CAR_H
#include <utility>
#include <vector>
#include <queue>
#include <SDL.h>
#include <cstdint>

#include <MathHelper.h>

class Car {
    private:
        std::vector<std::vector<std::pair<float, float>>>* paths;
        //current path from the paths
        int currentPath;
        //each curved path is made up of waypoints, this is the current waypoint.
        int currentWaypoint;
        SDL_Rect* chassis;
        enum STATE { rest, redlight, moving, movetowait, waiting, end};
        STATE state;
        //time of last Update in seconds
        float lastUpdate;
        //speed
        float speed;
        //These variables will accumulate the sub pixel updates to position.
        std::pair<float, float> sums;
        //keeps track of the rotation of the car in degrees
        float rotation;
        //a vector to keep track of the indices of every path that is an internal road (within an intersection)
        std::queue<int> internals;
        //the car we are waiting behind, if any.
        Car* wait;
        //the last position of the car we are waiting on, we need to know if it changed
        pair<int, int> lastWaitPos;
        uint64_t start;
    public:
        Car(std::pair<int, int> , float);
        ~Car();
        SDL_Rect* getChassis();
        float* getRotation();
        std::pair<int,int> getPos();
        std::vector<std::vector<std::pair<float, float>>> getPaths();
        void addPath(std::vector<std::pair<float, float>>, bool );
        void update(float );
        void update(float , bool );
        void waitBehind(Car*);
        bool withinTwoCarlengths();
        uint64_t timeSinceEpochMillisec();
        void updatePos(float );
        void translate(std::pair<float, float> , Vector2 , float );
        void rotate(Vector2 );
        bool isNotRest();
        bool isInternal();
        bool isWaiting();
        bool isMoveToWait();
        bool isRedLight();
        bool isLeftTurning();
        bool isAtEnd();
};

#endif