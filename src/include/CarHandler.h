#ifndef CARHANDLER_H
#define CARHANDLER_H
#include <Car.h>
#include <Router.h>
#include <TrafficRules.h>
#include <stack>
#include <map>

class CarHandler {
    private:
        std::vector<Car*>* cars;
        std::vector<std::stack<Intersection*>*>* routes;
        Router* router;
        TrafficRules* trafficRules;
        //maps the id of the intersection that theses cars came from
        std::map<std::pair<int,int>, std::vector<Car*>*>* prevInters;
        //uses the same indeces as cars, and has the id of the last intersection that car visited
        std::vector<std::pair<int,int>>* lastInter;
        //the time at which the last car to enter the intersection entered the intersection
        float withinIntersectionTime;
        //the origin (previous intersection) from which the last car to enter the intersection came.
        std::pair<int,int> withinIntersectionOrigin;
        //boolean to indicate if the car within the Intersection is turning left
        bool withinIntersectionLeft;
        //helpers
        void handleStop(int );
        void handleGo(int , float);
    public:
        CarHandler();
        ~CarHandler();
        std::pair<std::vector<SDL_Rect*>,std::vector<float*>> getData();
        Car* getCar(int );
        std::vector<std::vector<std::vector<std::pair<float, float>>>> getPaths();
        bool setRoute(int ,std::stack<Intersection*>* );
        void addCar(std::pair<int, int> , float );
        int size();
        void updateCar(int , float );
        bool isNotDone();

};


#endif