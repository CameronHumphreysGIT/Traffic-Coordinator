#include <iostream>
#include <CarHandler.h>

using namespace std;

CarHandler::CarHandler() {
    cars = new vector<Car*>();
    router = new Router();
    routes = new vector<stack<Intersection*>*>;
    trafficRules = new TrafficRules();
}

CarHandler::~CarHandler() {
    for (int i = 0; i < cars->size(); i++) {
        delete cars->at(i);
        cars->at(i) = NULL;
        delete routes->at(i);
        routes->at(i) = NULL;
    }
    delete cars;
    delete router;
    delete routes;
    delete trafficRules;
}

//This function returns a pair with a vector of SDL_Rect's and a vector of floats for the rotation of each, in degrees
pair<vector<SDL_Rect*>, vector<float*>>CarHandler::getData() {
    vector<SDL_Rect*> rects;
    vector<float*> rotations;
    for (int i = 0; i < cars->size(); i++) {
        rects.push_back(cars->at(i)->getChassis());
        rotations.push_back(cars->at(i)->getRotation());
    }
    return {rects, rotations};
}

//function used in Testing
Car* CarHandler::getCar(int index) {
    return cars->at(index);
}

bool CarHandler::setRoute(int index, stack<Intersection*>* route) {
    stack<Intersection*>* newRoute = new stack<Intersection*>(*route);
    //pop the first intersection, since it's the starting point.
    newRoute->pop();
    routes->at(index) = newRoute;
    return router->setRoute((cars->at(index)), route);
}

void CarHandler::addCar(pair<int, int> start, float time) {
    Car* car = new Car(start, time);
    cars->push_back(car);
    stack<Intersection*> empty;
    routes->push_back(&empty);
}

int CarHandler::size() {
    return (int)cars->size();
}

void CarHandler::updateCar(int index, float time) {
    //check if the next path for this car is an internal road
    if (cars->at(index)->isInternal()) {
        bool passable = routes->at(index)->top()->isPassable(cars->at(index)->getPos(), time);
        if (!passable) {
            cars->at(index)->update(time, true);
        }else {
            cars->at(index)->update(time, false);
            routes->at(index)->pop();
        }
    }else {
        cars->at(index)->update(time);
    }
}

//function for checking if the cars have all come to rest
bool CarHandler::isNotDone() {
    for (int i = 0; i < cars->size(); i++) {
        if (cars->at(i)->isNotRest()) {
            return true;
        }
    }
    return false;
}