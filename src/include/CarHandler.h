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
    public:
        CarHandler();
        ~CarHandler();
        std::pair<std::vector<SDL_Rect*>,std::vector<float*>> getData();
        Car* getCar(int );
        bool setRoute(int ,std::stack<Intersection*>* );
        void addCar(std::pair<int, int> , float );
        int size();
        void updateCar(int , float );
        void handleStop(int );
        bool isNotDone();

};


#endif