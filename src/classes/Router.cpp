#include <Router.h>
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
                    car->addPath(current->getSampled().at(side));
                }else {
                    //this means we came from another intersection and need to path through an intersection
                    car->addPath(current->getSampledInternals(prevSide).at(side));
                    //now add the actual route
                    car->addPath(current->getSampled().at(side));
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