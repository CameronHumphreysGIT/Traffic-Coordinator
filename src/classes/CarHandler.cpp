#include <iostream>
#include <CarHandler.h>

using namespace std;

CarHandler::CarHandler() {
    cars = new vector<Car*>();
    router = new Router();
    routes = new vector<stack<Intersection*>*>;
    trafficRules = new TrafficRules();
    prevInters = new map<pair<int,int>, vector<Car*>*>;
    lastInter = new vector<pair<int,int>>;
}

CarHandler::~CarHandler() {
    for (int i = 0; i < cars->size(); i++) {
        delete cars->at(i);
        cars->at(i) = NULL;
        delete routes->at(i);
        routes->at(i) = NULL;
    }
    for (auto it = prevInters->begin(); it != prevInters->end(); it++) {
        delete it->second;
    }
    delete cars;
    delete lastInter;
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
    //check if we have added a vector of cars with this intersection origin
    if (prevInters->find(newRoute->top()->getId()) == prevInters->end()) {
        vector<Car*>* vec = new vector<Car*>();
        vec->push_back(cars->at(index));
        prevInters->insert({newRoute->top()->getId(), vec});
    }else {
        //there is a vector of cars for this intersection, push this car into that vector
        prevInters->at(newRoute->top()->getId())->push_back(cars->at(index));
    }
    //also keep track of the intersection id:
    lastInter->at(index) = newRoute->top()->getId();
    //pop the first intersection, since it's the starting point.
    newRoute->pop();
    routes->at(index) = newRoute;
    return router->setRoute((cars->at(index)), route);
}

void CarHandler::addCar(pair<int, int> start, float time) {
    Car* car = new Car(start, time);
    cars->push_back(car);
    //send in an error value, to be changed later.
    lastInter->push_back({-1,-1});
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
            handleStop(index);
        }else {
            cars->at(index)->update(time, false);
            //remove this car from the vector at the previous intersection id
            vector<Car*>* vec = prevInters->at(lastInter->at(index));
            for (auto it = vec->begin(); it != vec->end(); it++) {
                if (*it == cars->at(index)) {
                    vec->erase(it);
                    break;
                }
            }
            //set the new Intersection to prevInter and lastInter
            //check if we have added a vector of cars with this intersection origin
            if (prevInters->find(routes->at(index)->top()->getId()) == prevInters->end()) {
                vector<Car*>* vec = new vector<Car*>();
                vec->push_back(cars->at(index));
                prevInters->insert({routes->at(index)->top()->getId(), vec});
            }else {
                //there is a vector of cars for this intersection, push this car into that vector
                prevInters->at(routes->at(index)->top()->getId())->push_back(cars->at(index));
            }
            //also keep track of the intersection id:
            lastInter->at(index) = routes->at(index)->top()->getId();
            //now pop this intersection from routes
            routes->at(index)->pop();
        }
    }else if (cars->at(index)->isWaiting()) {
        pair<float, float> currentPos = cars->at(4)->getPos();
        handleStop(index);
        cars->at(index)->update(time);
        

    }else {
        cars->at(index)->update(time);
    }
}

void CarHandler::handleStop(int index) {
    //tell closest behind us that we are stopped.
    vector<Car*> vec = *(prevInters->at(lastInter->at(index)));
    Car* me = cars->at(index);
    Car* closest = NULL;
    pair<int, int> minDiff = {1000, 1000};
    for (auto it = vec.begin(); it != vec.end(); it++) {
        //check to make sure we are not redundantly updating, or updating the car that we are waiting for.
        if (*it != me && !(*it)->isMoveToWait() && !(*it)->isWaiting() && !(*it)->isRedLight()) {
            //difference between me and the other
            pair<int, int> diff = {abs(me->getPos().first - (*it)->getPos().first), abs(me->getPos().second - (*it)->getPos().second)};
            if ((diff.first + diff.second) < (minDiff.first + minDiff.second)) {
                closest = *it;
                minDiff = diff;
            }
        }
    }
    if (closest != NULL) {
        closest->waitBehind(me);
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