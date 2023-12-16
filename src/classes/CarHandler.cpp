#include <iostream>
#include <CarHandler.h>

using namespace std;

CarHandler::CarHandler() {
    cars = new vector<Car*>();
    router = new Router();
    routes = new vector<stack<Intersection*>*>;
    prevInters = new map<pair<int,int>, vector<Car*>*>;
    lastInterId = new vector<pair<int,int>>;
    lastInter = new vector<Intersection*>;
    destroy = true;
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
    delete lastInterId;
    delete lastInter;
    delete router;
    delete routes;
}

//This function returns a pair with a vector of SDL_Rect's and a vector of floats for the rotation of each, in degrees
pair<vector<SDL_Rect*>, vector<float*>>CarHandler::getData() {
    vector<SDL_Rect*> rects;
    vector<float*> rotations;
    for (int i = 0; i < cars->size(); i++) {
        if (!cars->at(i)->isAtEnd()) {
            rects.push_back(cars->at(i)->getChassis());
            rotations.push_back(cars->at(i)->getRotation());
        }
    }
    return {rects, rotations};
}

//function used in Testing
Car* CarHandler::getCar(int index) {
    return cars->at(index);
}

vector<vector<vector<pair<float, float>>>> CarHandler::getPaths() {
    vector<vector<vector<pair<float, float>>>> paths;
    for (int i =0; i < cars->size(); i++) {
        if (!cars->at(i)->isAtEnd()) {
            paths.push_back(cars->at(i)->getPaths());
        }
    }
    return paths;
}

stack<Intersection*> CarHandler::getRoute(int index) {
    return *(routes->at(index));
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
    lastInterId->at(index) = newRoute->top()->getId();
    lastInter->at(index) = newRoute->top();
    //pop the first intersection, since it's the starting point.
    newRoute->pop();
    routes->at(index) = newRoute;
    return router->setRoute((cars->at(index)), route);
}

void CarHandler::setDestroy(bool des) {
    destroy = des;
} 

void CarHandler::addCar(pair<int, int> start, float time) {
    Car* car = new Car(start, time);
    cars->push_back(car);
    //send in an error value, to be changed later.
    lastInterId->push_back({-1,-1});
    lastInter->push_back(NULL);
    stack<Intersection*> empty;
    routes->push_back(&empty);
}

int CarHandler::size() {
    return (int)cars->size();
}

void CarHandler::updateCar(int index, float time) {
    if (destroy && cars->at(index)->isAtEnd()) {
        //handle prevIntersections
        vector<Car*>* vec = prevInters->at(lastInterId->at(index));
        for (auto it = vec->begin(); it != vec->end(); it++) {
            if (*it == cars->at(index)) {
                vec->erase(it);
                break;
            }
        }
        //keep a ref so we can delete it
        Car* reference = cars->at(index);
        if (reference->isBehind()) {
            Car* behind = reference->getBehind();
            behind->nullWait();
        }
        //handle cars:
        cars->erase(cars->begin() + index);
        //handle routes
        routes->erase(routes->begin() + index);
        //handle lastInterId
        lastInterId->erase(lastInterId->begin() + index);
        lastInter->erase(lastInter->begin() + index);
        delete reference;
        reference = nullptr;
    }else {
        //check if the next path for this car is an internal road
        if (cars->at(index)->isInternal()) {
            bool passable = routes->at(index)->top()->isPassable(cars->at(index)->getPos(), time);
            if (!passable) {
                cars->at(index)->update(time, true);
                handleStop(index);
            }else {
                float withinIntersectionTime = routes->at(index)->top()->getWithinTime();
                pair<int, int> withinIntersectionOrigin = routes->at(index)->top()->getWithinOrigin();
                bool withinIntersectionLeft = routes->at(index)->top()->isWithinLeft();
                //check if this car was stopped, if so, reset the withinIntersection variables to default values.
                if (cars->at(index)->isLeftTurning()) {
                    //check if the car should go:
                    if ((withinIntersectionTime == -1 && withinIntersectionOrigin.first == -1 && withinIntersectionOrigin.second == -1) || 
                        (withinIntersectionOrigin == lastInterId->at(index) || (time - withinIntersectionTime) > Variables::CLEARTIME) ) {
                        routes->at(index)->top()->setWithin(time, lastInterId->at(index), true);
                        cars->at(index)->update(time, false);
                        handleGo(index, time);
                    }else {
                        //we are stopped
                        cars->at(index)->update(time, true);
                        handleStop(index);
                    }
                }else {
                    //check if we should wait for a left turner:
                    if (withinIntersectionLeft && (withinIntersectionOrigin != lastInterId->at(index) && (time - withinIntersectionTime) < Variables::CLEARTIME)) {
                        //we are stopped
                        cars->at(index)->update(time, true);
                        handleStop(index);
                    }else if (withinIntersectionOrigin == lastInterId->at(index) && (time - withinIntersectionTime) < (Variables::CLEARTIME/4)) {
                        //we are waiting for a bit so they can get further into the intersection
                        cars->at(index)->update(time, true);
                        handleStop(index);
                    }else {
                        //set the withinIntersection paramaters
                        routes->at(index)->top()->setWithin(time, lastInterId->at(index), false);
                        cars->at(index)->update(time, false);
                        handleGo(index, time);
                    }
                }
            }
        }else if (cars->at(index)->isWaiting()) {
            //update first
            cars->at(index)->update(time);
            //check if we are still waiting
            if (cars->at(index)->isWaiting()) {
                handleStop(index);
            }
            //check if we are within the borders of the previous intersection we visited.
            Intersection* last = lastInter->at(index);
            if (last != NULL) {
                //basically set's the intersection to be impassible
                last->collidesWith(cars->at(index)->getPos(), time, lastInterId->at(index));
            }
        }else {
            //bool thing = cars->at(index)->isLeftTurning();
            cars->at(index)->update(time);
        }
    }

}

void CarHandler::handleStop(int index) {
    Car* me = cars->at(index);
    //if we have already told a car behind to stop, our job is done.
    if (!(me->isBehind())) {
        //tell closest behind us that we are stopped.
        vector<Car*> vec = *(prevInters->at(lastInterId->at(index)));
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
        if (closest != NULL && !closest->isAtEnd()) {
            pair<float, float> myWay = me->getWaypoint();
            pair<float, float> theirWay = closest->getWaypoint();
            int diff = (int)((myWay - theirWay).first + (myWay - theirWay).second);
            if (abs(diff) <= 2) {
                closest->waitBehind(me);
                me->setBehind(closest);
            }
        }
    }

}

void CarHandler::handleGo(int index, float time) {
    //remove this car from the vector at the previous intersection id
    vector<Car*>* vec = prevInters->at(lastInterId->at(index));
    for (auto it = vec->begin(); it != vec->end(); it++) {
        if (*it == cars->at(index)) {
            vec->erase(it);
            break;
        }
    }
    //set the new Intersection to prevInter and lastInterId
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
    lastInterId->at(index) = routes->at(index)->top()->getId();
    lastInter->at(index) = routes->at(index)->top();
    //now pop this intersection from routes
    routes->at(index)->pop();
}

//function for checking if the cars have all come to rest
bool CarHandler::isNotDone() {
    for (int i = 0; i < cars->size(); i++) {
        if (!cars->at(i)->isAtEnd()) {
            return true;
        }
    }
    return false;
}

bool CarHandler::detectCollisions() {
    bool collision = false;
    //go through prevInters, and detect collisions among the cars with the same previous intersections.
    for (auto it = prevInters->begin(); it != prevInters->end(); it++) {
        vector<Car*> checklist = *(it->second);
        while(!checklist.empty()) {
            SDL_Rect* first = checklist.at(0)->getChassis();
            //pop the front
            checklist.erase(checklist.begin());
            //loop through the checklist
            for (auto iter = checklist.begin(); iter != checklist.end(); iter++) {
                SDL_Rect* compare = (*iter)->getChassis();
                //check each point of compare and see if any point is in first.
                SDL_Point topLeft = {(compare->x), (compare->y)};
                SDL_Point topRight = {(compare->x + compare->w), (compare->y)};
                SDL_Point bottomLeft = {(compare->x), (compare->y + compare->h)};
                SDL_Point bottomRight = {(compare->x + compare->w), (compare->y + compare->h)};
                SDL_Point points[4] = {topLeft, topRight, bottomLeft, bottomRight};
                for (int i = 0; i < 4; i++) {
                    collision = SDL_PointInRect(&points[i], first);
                }
            }
        }
    }
    return collision;
}

bool CarHandler::isClear(Intersection* intersection, float time) {
    //check that no car is currently inside the intersection.
    if (time - intersection->getWithinTime() < Variables::CLEARTIME) {
        return false;
    }
    return true;
}