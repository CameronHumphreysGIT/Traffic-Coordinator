#include <TrafficCoordinator.h>
#include <iostream>

TrafficCoordinator::TrafficCoordinator(CarHandler* cHandler, vector<vector<Intersection*>*>* inters) {
    carHandler = cHandler;
    intersections = inters;
    turning = new map<int, Intersection*>;
    arrivals = new map<Intersection*, map<float, int>*>;
}

TrafficCoordinator::~TrafficCoordinator() {
    delete turning;
    for (auto iter = arrivals->begin(); iter != arrivals->end(); iter++) {
        delete iter->second;
    }
    delete arrivals;
}

void TrafficCoordinator::recalculatePath(int index, float time) {
    //firstly, lets check that we should be recalculating...
    if (carHandler->getCar(index)->isInternal()) {
        //take note of this car and save the intersection it came from
        pair<int, int> lastId = carHandler->getLastInter(index);
        Intersection* prev = intersections->at(lastId.first)->at(lastId.second);
        turning->insert({index, prev});
    }else {
        auto inter = turning->find(index);
        if (inter != turning->end()) {
            //we want to recalculate for this car.
            //lastInter gets set as soon as a car moves into an intersection.
            pair<int, int> lastId = carHandler->getLastInter(index);
            Intersection* current = intersections->at(lastId.first)->at(lastId.second);
            //possible routes:
            for (int side = 0; side < Variables::Side::END; side++) {
                pair<int, int> id = current->getNeighbor((Variables::Side)side);
                if (id.first != -1 && id.second != -1 && id != inter->second->getId()) {
                    //then find a way to calculate the length of the route from just here to the destination intersection.
                    stack<Intersection*> hypothetical;
                    hypothetical.push(intersections->at(id.first)->at(id.second));
                    hypothetical.push(current);
                    hypothetical.push(inter->second);
                    Car* me = carHandler->getCar(index);
                    float arrival = findArrival(me->getPos(), hypothetical);
                    if (arrival != -1) {
                        //check when the time the last car went into the intersection before me was.
                        float prevArrival = inter->second->getWithinTime();
                        //function to update the arrivals map
                        updateArrivals(inter->second, arrival, index);
                        UNTESTED CODE, THIS probably doesn't work and is likely extremely buggy.
                    }
                }
            }
            //pop the car off of turning
            turning->erase(inter);
        }
    }
}

float TrafficCoordinator::findArrival(pair<int, int> pos, stack<Intersection*> hypothetical) {
    float length = carHandler->getRouter()->findLength(pos, hypothetical);
    return (length/Variables::DEFAULT_SPEED);
}

void TrafficCoordinator::updateArrivals(Intersection* intersection, float arrival, int index) {
    //function to update the arrivals for a given intersection
    //arrivals for this intersection, if they exist
    map<float, int>* thisArrivals = arrivals->find(intersection)->second;
    if (thisArrivals == arrivals->end()->second) {
        thisArrivals = new map<float, int>;
    }else {
        //delete all arrivals that happen before the time of the most recent car to get within this intersection
        float withinTime = intersection->getWithinTime();
        if (withinTime != -1) {
            for (auto iter = thisArrivals->begin(); iter != thisArrivals->end(); iter++) {
                if (iter->first <= withinTime) {
                    thisArrivals->erase(iter);
                }
            }
        }
    }
    //cars that have this intersection as their current destination.
    vector<int> carIndices = carHandler->getDestIds(intersection->getId());
    //FIXME this is a test, remove it maybe
    if (carIndices.empty()) {
        cout<<"--------------------------------------------------------------------------------NOITHING BROKEN AHHH IT HURTS\n";
    }
    //insert the arrival of my car:
    thisArrivals->insert({arrival, index});
    pair<int, int> id;
    Intersection* prev;
    stack<Intersection*> path;
    //now cycle through cars with this destination, and set arrivals as the time that they will get to the intersection, but not necessarily when they are in the intersection.
    for (int i = 0; i < carIndices.size(); i++) {
        //check that we haven't already calculated for this id.
        if (thisArrivals->find(carIndices.at(i)) == thisArrivals->end()) {
            id = carHandler->getLastInter(carIndices.at(i));
            prev = intersections->at(id.first)->at(id.second);
            //we know they are going to our current intersection.
            path.push(intersection);
            path.push(prev);
            //calculate the time
            float time = findArrival(carHandler->getCar(carIndices.at(i))->getPos(), path);
            //put it into thisArrivals:
            thisArrivals->insert({time, carIndices.at(i)});
        }
    }
    //go through and update times according to traffic rules. 
    float latestWithinTime = NULL;
    bool isWithinLeft;
    pair<int, int> latestWithinOrig;
    pair<int, int> latestWithinDest;
    int prevInter;
    for (int i = 0; i < thisArrivals->size(); i++) {
        auto iter = thisArrivals->begin();
        advance(iter, i);
        if (iter->second != prevInter) {
            float waitTime;
            if (latestWithinTime == NULL) {
                //get the time tillpass, to see if we are at a redlight
                waitTime = intersection->timeTillPass(carHandler->getLastInter(iter->second), iter->first);
                if (waitTime <= Variables::CLEARTIME) {
                    //we are waiting for the within to clear or not waiting at all, we are the next within.
                    latestWithinTime = (iter->first + waitTime);
                    latestWithinOrig = carHandler->getLastInter(iter->second);
                    //top here will be the current intersection.
                    stack<Intersection*> stuff = carHandler->getRoute(iter->second);
                    stuff.pop();
                    //this will get the destination, needed so we know if left turning.
                    latestWithinDest = stuff.top()->getId();
                }
            }else {
                //get the time tillpass, to see if we are at a redlight
                waitTime = intersection->timeTillPass(carHandler->getLastInter(iter->second), iter->first, latestWithinTime, latestWithinOrig, latestWithinDest);
                if (iter->first <= latestWithinTime) {
                    //adjust for addedTime in the timeTillPass function
                    if ((waitTime - (latestWithinTime - iter->first)) <= Variables::CLEARTIME) {
                        //we are waiting for the within to clear or not waiting at all, we are the next within.
                        latestWithinTime = (iter->first + waitTime);
                        latestWithinOrig = carHandler->getLastInter(iter->second);
                        //top here will be the current intersection.
                        stack<Intersection*> stuff = carHandler->getRoute(iter->second);
                        stuff.pop();
                        //this will get the destination, needed so we know if left turning.
                        latestWithinDest = stuff.top()->getId();
                    }
                }else {
                    if (waitTime <= Variables::CLEARTIME) {
                        //we are waiting for the within to clear or not waiting at all, we are the next within.
                        latestWithinTime = (iter->first + waitTime);
                        latestWithinOrig = carHandler->getLastInter(iter->second);
                        //top here will be the current intersection.
                        stack<Intersection*> stuff = carHandler->getRoute(iter->second);
                        stuff.pop();
                        //this will get the destination, needed so we know if left turning.
                        latestWithinDest = stuff.top()->getId();
                    }
                }
            }
            thisArrivals->insert({iter->first + waitTime, iter->second});
            thisArrivals->erase(iter);
            //we want to go back to this same position, which may be a new value
            i--;
        }
        prevInter = iter->second;
    }
    //TODO: consider cycle through adjacent intersections and updateArrivals for them??
}

//bool TrafficCoordinator::isFirst(int index, Intersection* intersection, float arrival) {
//    //first lets find cars that have this intersection as their next destination:
//    vector<int> cars = carHandler->getDestInters()->at(intersection->getId());
//    //map on arrival time to the car that is arriving at that time
//    map<float, int> arrivals;
//    for (int i =0; i < cars.size(); i++) {
//        
//        //calculate the time
//        float thisArrival = findArrival(carHandler->getLastInter())
//    }
//    //cycle through the adjacent intersections
//}