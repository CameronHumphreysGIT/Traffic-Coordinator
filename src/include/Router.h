#ifndef ROUTER_H
#define ROUTER_H
#include <vector>
#include <Intersection.h>
#include <Car.h>

class Router {
    public:
        Router();
        ~Router();
        bool setRoute(Car* &, std::vector<Intersection*>*);
};

#endif