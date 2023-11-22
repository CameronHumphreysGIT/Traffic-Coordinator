#ifndef TRAFFICCOORDINATOR_H
#define TRAFFICCOORDINATOR_H
#include <CarHandler.h>

class TrafficCoordinator{
    private:
        CarHandler* carHandler;
    public:
        TrafficCoordinator(CarHandler* );
        ~TrafficCoordinator();
        void recalculatePath(int , float );
};

#endif