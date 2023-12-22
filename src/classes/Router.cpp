#include <Router.h>
#include <iostream>
using namespace std;

Router::Router() {

}

Router::~Router() {

}

bool Router::setRoute(Car* &car, stack<Intersection*>* route) {
    Variables::Side prevSide = Variables::Side::END;
    while(route->size() > 1) {
        Intersection* current = route->top();
        //pop off the current intersection
        route->pop();
        pair<int,int> targetId = route->top()->getId();
        bool successFlag = false;
        for (int side = Variables::TOP; side != Variables::END; side++) {
            if (current->getNeighbor((Variables::Side)side).first == targetId.first && current->getNeighbor((Variables::Side)side).second == targetId.second) {
                //found the neighbor and the route...
                if (prevSide == Variables::Side::END) {
                    //first route, don't worry about internals
                    //add sampled path at the side that the neigbor was found on
                    car->addPath(current->getSampled().at(side), false);
                }else {
                    //this means we came from another intersection and need to path through an intersection
                    car->addPath(current->getSampledInternals(prevSide).at(side), true);
                    //now add the actual route
                    car->addPath(current->getSampled().at(side), false);
                    prevSide = (Variables::Side)side;
                }
                //nextside is adjacent to previous side
                if (side == Variables::Side::LEFT) {
                    prevSide = Variables::Side::RIGHT;
                }else if (side == Variables::Side::BOTTOM) {
                    prevSide = Variables::Side::TOP;
                }else {
                    prevSide = (Variables::Side)(side + 2);
                }
                
                successFlag = true;
            }
        }
        //means we didn't find the next intersection
        if (!successFlag) {
            return successFlag;
        }
    }
    return true;
}

bool Router::reRoute(Car* & car, stack<Intersection*>* route) {
    pair<float, float> waypoint1 = car->getWaypoint();
    Intersection* orig = route->top();
    route->pop();
    Intersection* dest = route->top();
    route->pop();
    //check here if the car is inside an intersection...
    if (car->isWithin()) {
        //car is within an intersection.
        for (int side = Variables::TOP; side != Variables::END; side++) {
            if (orig->getNeighbor((Variables::Side)side).first == dest->getId().first && orig->getNeighbor((Variables::Side)side).second == dest->getId().second) {
                ///this side is the side that we want to go to, but which side did we come from??
                pair<float, float> origin = car->getPath().at(0);
                //search through each side.
                bool flag = false;
                for(int side2 = Variables::TOP; side2 != Variables::END; side2++) {
                    vector<pair<float, float>> path = orig->getSampledInternals((Variables::Side)side2).at(side);
                    if (path.size() > 0 && path.at(0) == origin) {
                        //tell the car it has been reset.
                        car->reset();
                        //add the path
                        car->addPath(path, true);
                        flag = true;
                        break;
                    }
                }
                //put dest back onto the stack.
                route->push(dest);
                //put orig back onto the stack.
                route->push(orig);
                break;
            }
        }
    }else {
        //tell the car it has been reset.
        car->reset();
        //this method ignores the disabled array, and returns the side id's as originally designed.
        Variables::Side side = orig->getNeighbor(dest->getId());
        //The goal of this starting part is to get the closest point on the road to the current car position. this point, along with the end point of the road will make up the first path for the car.
        //make a direction vector along the path
        vector<pair<float, float>> path = orig->getSampled().at(side);
        //path has two points
        pair<float, float> dir = path.at(1) - path.at(0);
        Vector2 road = {dir.first, dir.second};
        //find closest point from pos to vector.
        pair<float, float> pos = {(float)car->getPos().first, (float)car->getPos().second};
        dir = pos - path.at(0);
        Vector2 road2Car = {dir.first, dir.second};
        road = road.Normalized();
        //projection of car pos onto the road
        float projRoad = road.DotProduct(road2Car);
        //vector from road start to pos projection
        Vector2 proj = (road*(projRoad));
        //above, but as a pair
        pair<float, float> projPair = {proj.x, proj.y};
        //finally, the start pos for the car.
        pair<float, float> start = path.at(0) + projPair;
        pair<float, float> end = path.at(1);
        vector<pair<float, float>> addPath = {start, end};
        car->addPath(addPath, false);
        //now add the internal road start with the side that is at the entrance of the intersection
        Variables::Side prevSide;
        //nextside is adjacent to previous side
        if (side == Variables::Side::LEFT) {
            prevSide = Variables::Side::RIGHT;
        }else if (side == Variables::Side::BOTTOM) {
            prevSide = Variables::Side::TOP;
        }else {
            prevSide = (Variables::Side)(side + 2);
        }
        //now find the side that goes to the next intersection.
        for (int side2 = Variables::TOP; side2 != Variables::END; side2++) {
            if (dest->getNeighbor((Variables::Side)side2).first == route->top()->getId().first && dest->getNeighbor((Variables::Side)side2).second == route->top()->getId().second) {
                //found the spot. now we can add in the proper path:
                //make sure there is a path:

                car->addPath(dest->getSampledInternals(prevSide).at(side2), true);
                break;
            }
        }
        //put dest back onto the stack.
        route->push(dest);
    }
    pair<float, float> waypoint2 = car->getWaypoint();

    //set the route.
    bool success = setRoute(car, route);
    return success;
}