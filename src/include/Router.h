#ifndef ROUTER_H
#define ROUTER_H
#include <stack>
#include <Intersection.h>
#include <Car.h>

class Router {
    public:
        Router();
        ~Router();
        bool setRoute(Car* &, std::stack<Intersection*>* );
        bool reRoute(Car* & car, stack<Intersection*>* route);
};

#endif