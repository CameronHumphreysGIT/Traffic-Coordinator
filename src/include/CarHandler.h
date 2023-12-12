#ifndef CARHANDLER_H
#define CARHANDLER_H
#include <Car.h>
#include <Router.h>
#include <stack>
#include <map>
#include <set>

class CarHandler {
    private:
        std::vector<Car*>* cars;
        std::vector<std::stack<Intersection*>*>* routes;
        Router* router;
        //maps the id of the intersection that theses cars came from
        std::map<std::pair<int,int>, std::vector<Car*>*>* prevInters;
        //uses the same indeces as cars, and has the id of the last intersection that car visited
        std::vector<std::pair<int,int>>* lastInter;
        //bool for whether or not we should destroy ended cars
        bool destroy;
        //helpers
        void handleStop(int );
        void handleGo(int , float);
    public:
        CarHandler();
        ~CarHandler();
        std::pair<std::vector<SDL_Rect*>,std::vector<float*>> getData();
        Car* getCar(int );
        std::vector<std::vector<std::vector<std::pair<float, float>>>> getPaths();
        stack<Intersection*> getRoute(int );
        bool setRoute(int ,std::stack<Intersection*>* );
        void setDestroy(bool );
        void addCar(std::pair<int, int> , float );
        int size();
        void updateCar(int , float );
        bool isNotDone();

};


#endif