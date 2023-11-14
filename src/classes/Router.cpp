#include <Router.h>
using namespace std;

Router::Router() {

}

Router::~Router() {

}

bool Router::setRoute(Car* &car, vector<Intersection*>* route) {
    Variables::Side prevSide = Variables::Side::END;
    for (int i = 0; i < (route->size() - 1); i++) {
        pair<int,int> targetId = route->at(i+1)->getId();
        bool successFlag = false;
        for (int side = Variables::TOP; side != Variables::END; side++) {
            if (route->at(i)->getNeighbor((Variables::Side)side).first == targetId.first && route->at(i)->getNeighbor((Variables::Side)side).second == targetId.second) {
                //found the neighbor and the route...
                if (prevSide == Variables::Side::END) {
                    //first route, don't worry about internals
                    //add sampled path at the side that the neigbor was found on
                    car->addPath(route->at(i)->getSampled().at(side));
                }else {
                    //this means we came from another intersection and need to path through an intersection
                    car->addPath(route->at(i)->getSampledInternals(prevSide).at(side));
                    //now add the actual route
                    car->addPath(route->at(i)->getSampled().at(side));
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