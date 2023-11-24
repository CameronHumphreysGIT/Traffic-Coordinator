#ifndef TRAFFICCOORDINATOR_H
#define TRAFFICCOORDINATOR_H
#include <CarHandler.h>
#include <map>

class TrafficCoordinator{
    private:
        CarHandler* carHandler;
        //this is a map of all cars that are to be initiating a turn.
        map<int, Intersection*>* turning;
        std::vector<std::vector<Intersection*>*>* intersections;
        //maps an intersection to a map that maps the arrival(when the car enters the intersection) time to that intersection to a car
        map<Intersection*, map<float, int>*>* arrivals;
    public:
        TrafficCoordinator(CarHandler* , vector<vector<Intersection*>*>* );
        ~TrafficCoordinator();
        void recalculatePath(int , float );
        float findArrival(pair<int, int> , stack<Intersection*> );
        void updateArrivals(Intersection* , float , int );
};

#endif