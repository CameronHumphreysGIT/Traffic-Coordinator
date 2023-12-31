#include <iostream>
#include <CarHandler.h>
#include <random>
#include <AStar.h>

using namespace std;

CarHandler::CarHandler() {
    cars = new vector<Car*>();
    router = new Router();
    routes = new vector<stack<Intersection*>*>;
    prevInters = new map<pair<int,int>, vector<Car*>*>;
    lastInterId = new vector<pair<int,int>>;
    lastInter = new vector<Intersection*>;
    destroy = true;
    accidents = new map<pair<int, int>, bool>;
    accidentTypes = new vector<pair<int, float>>;
    timeSum = 0;
    carCount = 0;
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
pair<vector<bool>, pair<vector<SDL_Rect*>, vector<float*>>> CarHandler::getData() {
    vector<SDL_Rect*> rects;
    vector<float*> rotations;
    vector<bool> accidents;
    for (int i = 0; i < cars->size(); i++) {
        if (!cars->at(i)->isAtEnd()) {
            rects.push_back(cars->at(i)->getChassis());
            rotations.push_back(cars->at(i)->getRotation());
            accidents.push_back(cars->at(i)->isAccident());
        }
    }
    return {accidents, {rects, rotations}};
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
    if (accidentTypes->at(index).first != -1) {
        float selection = route->size() * accidentTypes->at(index).second;
        accidentTypes->at(index) = {accidentTypes->at(index).first, selection};
    }
    bool reroute = false;
    Intersection* top = NULL;
    if (!(routes->at(index) == nullptr)) {
        reroute = true;
        if (!cars->at(index)->isWithin()) {
            //keep where they are going.
            top = routes->at(index)->top();
            newRoute->push(top);
        }
    }
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

    if (reroute) {
        newRoute->pop();
        routes->at(index) = newRoute;
        if (top != NULL) {
            //they need the intersection destination that this car is going to:
            route->push(top);
        }
        return router->reRoute((cars->at(index)), route);
    }else {
        //pop the first intersection, since it's the starting point.
        newRoute->pop();
        routes->at(index) = newRoute;
        return router->setRoute((cars->at(index)), route);
    }
}

void CarHandler::setDestroy(bool des) {
    destroy = des;
} 

void CarHandler::addCar(pair<int, int> start, float time) {
    Car* car = new Car(start, time);
    cars->push_back(car);
    //send in an error value, to be changed later.
    lastInterId->push_back({-1,-1});
    accidentTypes->push_back({-1, -1.0f});
    lastInter->push_back(NULL);
    stack<Intersection*>* empty = nullptr;
    routes->push_back(empty);
    carCount++;
}

void CarHandler::addCar(pair<int, int> start, float time, int accidentType) {
    Car* car = new Car(start, time);
    cars->push_back(car);
    //first, pick a random car:
    random_device rd;
    mt19937 generator(rd());
    uniform_real_distribution<float> distribution(0.0f,1.0f);
    accidentTypes->push_back({accidentType, distribution(generator)});
    //send in an error value, to be changed later.
    lastInterId->push_back({-1,-1});
    lastInter->push_back(NULL);
    stack<Intersection*>* empty = nullptr;
    routes->push_back(empty);
    carCount++;
}

int CarHandler::size() {
    return (int)cars->size();
}

pair<Intersection*, Intersection*> CarHandler::updateCar(int index, float time) {
    //random accident, happens no matter the location.
    if (accidentTypes->at(index).first == 0 && routes->at(index)->size() == (int)ceil(accidentTypes->at(index).second)) {
        //have an accident
        cars->at(index)->haveAccident();
    }
    //accident only happens outside of an intersection
    if (accidentTypes->at(index).first == 1 && !cars->at(index)->isWithin() && routes->at(index)->size() == (int)ceil(accidentTypes->at(index).second)) {
        //have an accident
        cars->at(index)->haveAccident();
    }
    if (destroy && cars->at(index)->isAtEnd()) {
        timeSum += cars->at(index)->getDuration();
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
        Car* behind = reference->getBehind();
        if (behind != NULL) {
            behind->nullWait();
        }
        Car* wait = reference->getWait();
        if (wait != NULL) {
            wait->nullBehind(reference);
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
        if (cars->at(index)->isAccident()) {
            //tell people behind to handle the Stop.
            handleStop(index);
            cars->at(index)->update(time);
            //tell the intersection, check if we are within using our waypoint.
            //pair<float, float> posFloat = {(float)cars->at(index)->getPos().first, (float)cars->at(index)->getPos().second};
            //pair<float, float> dir = (cars->at(index)->getWaypoint() - posFloat) * 0.5;//go halfway to the waypoint
            //dir = posFloat + dir;
            //pair<int, int> roundedWaypoint = {(int)ceil(dir.first), (int)ceil(dir.second)};
            
            if (cars->at(index)->isWithin()) {
                //guarantee the wanted behaviour.
                lastInter->at(index)->accident(lastInter->at(index)->getCenter());
                //add to the map of accidents.
                accidents->insert({lastInterId->at(index), true});
                //special return value allows System to handle this.
                return {lastInter->at(index), NULL};
            }else {
                bool isOutside = lastInter->at(index)->accident(cars->at(index)->getPos());
                if (!isOutside) {
                    //add to the accidents and use the return value.
                    accidents->insert({lastInterId->at(index), true});
                    //special return value allows System to handle this.
                    return {lastInter->at(index), NULL};
                }
            }
        } else if (cars->at(index)->isInternal()) {
            //check if the next path for this car is an internal road
            bool passable = routes->at(index)->top()->isPassable(cars->at(index)->getPos(), time);
            if (!passable) {
                cars->at(index)->update(time, true);
                handleStop(index);
            }else {
                //see if we are going into an intersection with an accident
                if (accidents->find(routes->at(index)->top()->getId()) != accidents->end()) {
                    //we should reroute this car immediately.
                    return getStartEnd(index);
                }
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
            cars->at(index)->accidentWait();
        }else if (cars->at(index)->isBehindAccident()) {
            //this car is waiting behind a car accident.
            //find opposing traffic, which will be coming from the next intersection that i'm going to.
            Intersection* target = routes->at(index)->top();
            auto iter = prevInters->find(target->getId());
            if (iter != prevInters->end()) {
                vector<Car*> vec = *(iter->second);
                float minDiff = 1000.0f;
                Car* opposing = NULL;
                vector<pair<float, float>> myPath = cars->at(index)->getPath();
                pair<float, float> myStart = myPath.at(0);
                for (auto it = vec.begin(); it != vec.end(); it++) {
                    vector<pair<float, float>> theirPath = (*it)->getPath();
                    pair<float, float> theirEnd = theirPath.at(theirPath.size() - 1);
                    pair<float, float> diff = {(theirEnd.first - myStart.first), (theirEnd.second - myStart.second)};
                    Vector2 distance = {diff.first, diff.second};
                    if (distance.Magnitude() < minDiff) {
                        opposing = *it;
                        minDiff = distance.Magnitude();
                    }
                }
                if (opposing != NULL && minDiff <= 8) {
                    //found a suitable opposition.
                    cars->at(index)->waitBehind(opposing, {-1.0f,-1.0f});
                    cars->at(index)->update(time);
                }else {
                    opposing = NULL;
                    //setwaitbehind to an error value.
                    cars->at(index)->waitBehind(opposing, {-1.0f,-1.0f});
                    cars->at(index)->update(time);
                }
            }else {
                //no opposing traffic
                Car* opposing = NULL;
                //setwaitbehind to an error value.
                cars->at(index)->waitBehind(opposing, {-1.0f,-1.0f});
                cars->at(index)->update(time);
            }
            //check if we are within the borders of the previous intersection we visited.
            Intersection* last = lastInter->at(index);
            if (last != NULL) {
                //basically set's the intersection to be impassible
                last->collidesWith(cars->at(index)->getPos(), time, lastInterId->at(index));
            }
            //check if the car is now moving...
            if (cars->at(index)->isMoving()) {
                return getStartEnd(index);
            }else {
                return {NULL, NULL};
            }
        }else {
            cars->at(index)->update(time);
        }
    }
    return {NULL, NULL};
}

void CarHandler::handleStop(int index) {
    Car* me = cars->at(index);
    //if we have already told a car behind to stop, our job is done.
    if (!(me->isBehind())) {
        //tell closest behind us that we are stopped.
        vector<Car*> vec = *(prevInters->at(lastInterId->at(index)));
        Car* closest = NULL;
        pair<int, int> minDiff = {1000, 1000};
        vector<pair<float, float>> path = me->getPath();
        pair<float, float> startPath = path.at(0);
        pair<float, float> endPath = path.at(path.size() - 1);
        for (auto it = vec.begin(); it != vec.end(); it++) {
            if (!(*it)->isAtEnd()) {
                //make sure they are heading the right way.
                vector<pair<float, float>> theirPath = (*it)->getPath();
                pair<float, float> theirEnd = theirPath.at(theirPath.size() - 1);
                //within a 2 pixel tolerance.
                int diff1 = (int)((startPath - theirEnd).first + (startPath - theirEnd).second);
                int diff2 = (int)((endPath - theirEnd).first + (endPath - theirEnd).second);
                if (abs(diff1) <= 2 || abs(diff2) <= 2) {
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
            }
        }
        if (closest != NULL && closest != me->getWait()) {
            if (me->isAccident()) {
                if (closest != me->getBehind()) {
                    pair<float, float> myWay = me->getWaypoint();
                   //if (me->isWithin()) {
                   //    //the distance doesn't matter, they must wait.
                   //    closest->waitBehind(me, me->getPos());
                   //}else {
                    closest->waitBehind(me, myWay);
                   // }
                    if (closest->getWait() == me) {
                        me->setBehind(closest);
                    } 
                }
            }else {
                pair<float, float> myWay = me->getWaypoint();
                closest->waitBehind(me, myWay);
                if (closest->getWait() == me) {
                    me->setBehind(closest);
                }
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
    bool flag = false;
    if (routes->at(index)->size() == 0) {
        flag = true;
    }
    //here we can check if we should be a car accident:
    if (accidentTypes->at(index).first == 2 && routes->at(index)->size() == (int)ceil(accidentTypes->at(index).second)) {
        //have an accident
        cars->at(index)->haveAccident();
    }
}

//function for checking if the cars have all come to rest
bool CarHandler::isNotDone() {
    for (int i = 0; i < cars->size(); i++) {
        if (!cars->at(i)->isAtEnd() && !cars->at(i)->isAccident()) {
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
    //check that no car is stopped at this intersection.
    for (int i = 0; i < routes->size(); i++) {
        if (routes->at(i)->top() == intersection) {
            if (cars->at(i)->isRedLight()) {
                //there's a red light 
                return false;
            }
        }
    }
    return true;
}

void CarHandler::handleAccident() {
    //first, pick a random car:
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<int> distribution(0,(int)(cars->size() - 1));
    Car* car = cars->at(distribution(generator));
    //only choose cars that are valid
    while (car->isAtEnd() || car->isAccident()) {
        //pick a different car
        car = cars->at(distribution(generator));
    }
    //set into an accident.
    car->haveAccident();
    carCount--;
}

/// @brief helper function for accident handling. finds the start and end points that the car needs to be rerouted with.
/// @param index index of the car getting rerouted
/// @return the start, end of the route that is to be generated.
pair<Intersection*, Intersection*> CarHandler::getStartEnd(int index) {
    //we need to reroute this car...
    //find the end of their current route
    stack<Intersection*> route = *(routes->at(index));
    //we want to backtrack...
    Intersection* start = lastInter->at(index);
    while (!(route.size() == 1)) {
        route.pop();
    }
    Intersection* end = route.top();
    //remove self from prevInters
    vector<Car*>* vec = prevInters->at(lastInterId->at(index));
    for (auto it = vec->begin(); it != vec->end(); it++) {
        if (*it == cars->at(index)) {
            vec->erase(it);
            break;
        }
    }
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<int> distribution(0,(int)(routes->size() - 1));
    while (!isValidDest(end) || start == end) {
        //select a new dest.

        int randRow = distribution(generator);
        end = routes->at(randRow)->top();
    }
    return {start, end};
}

/// @brief Way to check if a destination is reachable ie: if there are no accidents in that intersection
/// @param dest Destination candidate to be checked, as an Intersection pointer
/// @return Whether or not destination is reachable.
bool CarHandler::isValidDest(Intersection* dest) {
    //accidents is a map, which makes this easy:
    auto iter = accidents->find(dest->getId());
    return (iter == accidents->end());
}

void CarHandler::dataReport() {
    cout<<"DATAREPORT=============================Cars: "<<carCount<<" Total Time (seconds): "<<(timeSum * 0.001f)<<" Average Time (seconds): "<<(timeSum/carCount)*0.001f<<" ==================================\n";
}