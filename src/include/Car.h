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
        enum STATE { rest, redlight, moving, movetowait, waiting, end, accident, behindAccident};
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
        //Car* for the car previously waiting behind this car, if any.
        Car* prevBehind;
        //true if currently has a car behind this one.
        bool behind;
    public:
        Car(std::pair<int, int> , float);
        ~Car();
        SDL_Rect* getChassis();
        float* getRotation();
        std::pair<int,int> getPos();
        std::vector<std::vector<std::pair<float, float>>> getPaths();
        pair<float, float> getWaypoint();
        Car* getBehind();
        vector<pair<float, float>> getPath();
        Car* getWait();
        void nullWait();
        void nullBehind(Car* );
        bool isBehind();
        void addPath(std::vector<std::pair<float, float>>, bool );
        void setBehind(Car* );
        void update(float );
        void update(float , bool );
        void waitBehind(Car*&, pair<float, float>);
        bool withinTwoCarlengths();
        uint64_t timeSinceEpochMillisec();
        void updatePos(float );
        void translate(std::pair<float, float> , Vector2 , float );
        void rotate(Vector2 );
        bool pathClear();
        void reset();
        void accidentWait();
        void haveAccident();
        bool isMoving();
        bool isBehindAccident();
        bool isAccident();
        bool isNotRest();
        bool isInternal();
        bool isWaiting();
        bool isMoveToWait();
        bool isRedLight();
        bool isLeftTurning();
        bool isAtEnd();
};

#endif