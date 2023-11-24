#include <Router.h>
#include <MathHelper.h>

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

float Router::findLength(pair<int, int> pos, stack<Intersection*> route) {
    //start by making the route.
    Variables::Side prevSide = Variables::Side::END;
    vector<vector<pair<float, float>>> paths;
    while(route.size() > 1) {
        Intersection* current = route.top();
        //pop off the current intersection
        route.pop();
        pair<int,int> targetId = route.top()->getId();
        bool successFlag = false;
        for (int side = Variables::TOP; side != Variables::END; side++) {
            if (current->getNeighbor((Variables::Side)side).first == targetId.first && current->getNeighbor((Variables::Side)side).second == targetId.second) {
                //found the neighbor and the route...
                if (prevSide != Variables::Side::END) {
                    //not the first path, so we should add it.
                    //this means we came from another intersection and need to path through an intersection
                    paths.push_back(current->getSampledInternals(prevSide).at(side));
                    //now add the actual route
                    paths.push_back(current->getSampled().at(side));
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
        //check success flag.
        if (!successFlag) {
            return -1;
        }
    }

    
    float length = 0;
    //then go through paths and count the length.
    pair<float, float> start = {(float)pos.first, (float)pos.second};
    for (int i = 0; i < paths.size(); i++) {
        for (int i2 = 0; i2 < (paths.at(i).size()); i2++) {
            pair<float, float> orig;
            pair<float, float> dest;
            if (i2 == 0) {
                orig = start;
            }else {
                orig = paths.at(i).at(i2 - 1);
            }
            dest = paths.at(i).at(i2);
            //now use orig and dest to calculate magnitude
            Vector2 distance = {(dest - orig).first, (dest - orig).second};
            length += distance.Magnitude();
        }
        //set the start to the end of the previous path
        start = paths.at(i).at(paths.at(i).size() - 1);
    }
    return length;
}