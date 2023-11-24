#ifndef CARHANDLER_H
#define CARHANDLER_H
#include <Car.h>
#include <Router.h>
#include <stack>
#include <map>

class CarHandler {
    private:
        std::vector<Car*>* cars;
        std::vector<std::stack<Intersection*>*>* routes;
        Router* router;
        //maps the id of the intersection that theses cars came from
        std::map<std::pair<int,int>, std::vector<Car*>*>* prevInters;
        //uses the same indeces as cars, and has the id of the last intersection that car visited
        std::vector<std::pair<int,int>>* lastInter;
        //maps the id of an intersection to a vector of cars with the intersection as their destination
        std::map<std::pair<int, int>, std::vector<int>*>* destInters;
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
        std::pair<int, int> getLastInter(int );
        Router* getRouter();
        std::vector<int> getDestIds(std::pair<int, int> );
        bool setRoute(int ,std::stack<Intersection*>* );
        void addCar(std::pair<int, int> , float );
        int size();
        void updateCar(int , float );
        bool isNotDone();

};


#endif