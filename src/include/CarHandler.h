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
        std::vector<std::pair<int,int>>* lastInterId;
        //bool for whether or not we should destroy ended cars
        bool destroy;
        //the last intersection each car visited, as an intersection pointer.
        std::vector<Intersection*>* lastInter;
        //map of all of the intersection ID's to a boolean indicating if the accident is still active (not cleared)
        std::map<std::pair<int, int>, bool>* accidents;
        //helpers
        void handleStop(int );
        void handleGo(int , float);
        pair<Intersection*, Intersection*> getStartEnd(int );
    public:
        CarHandler();
        ~CarHandler();
        std::pair<std::vector<bool>, std::pair<std::vector<SDL_Rect*>,std::vector<float*>>> getData();
        Car* getCar(int );
        std::vector<std::vector<std::vector<std::pair<float, float>>>> getPaths();
        stack<Intersection*> getRoute(int );
        bool setRoute(int ,std::stack<Intersection*>* );
        void setDestroy(bool );
        void addCar(std::pair<int, int> , float );
        int size();
        pair<Intersection*, Intersection*> updateCar(int , float );
        bool isNotDone();
        bool detectCollisions();
        bool isClear(Intersection* , float );
        void handleAccident();
        bool isValidDest(Intersection* );
};


#endif