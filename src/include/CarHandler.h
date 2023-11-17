#ifndef CARHANDLER_H
#define CARHANDLER_H
#include <Car.h>
#include <Router.h>
#include <TrafficRules.h>
#include <stack>

class CarHandler {
    private:
        std::vector<Car*>* cars;
        std::vector<std::stack<Intersection*>*>* routes;
        Router* router;
        TrafficRules* trafficRules;
    public:
        CarHandler();
        ~CarHandler();
        std::pair<std::vector<SDL_Rect*>,std::vector<float*>> getData();
        Car* getCar(int );
        bool setRoute(int ,std::stack<Intersection*>* );
        void addCar(std::pair<int, int> , float );
        int size();
        void updateCar(int , float );
        bool isNotDone();

};


#endif