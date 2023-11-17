#ifndef TRAFFICRULES_H
#define TRAFFICRULES_H
#include <Car.h>
#include <Intersection.h>

//This class exsists to determine how cars should follow traffic rules.
class TrafficRules {
    public:
        TrafficRules();
        ~TrafficRules();
        //TODO: consider not passing car by reference
        void updateCar(Car* &, Intersection*);
};

#endif