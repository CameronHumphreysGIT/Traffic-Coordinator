#include <Intersection.h>
#include <iostream>
#include <Variables.h>
using namespace std;

//enum trafficDirs {Vertical = 1, Horizontal = 0};
Intersection::Intersection() {
    top = new Road;
    right = new Road;
    bottom = new Road;
    left = new Road;
    isVerticalGreen = false;
    //loop through each side, and set internals
    for (int side = Variables::TOP; side != Variables::END; side++) {
        for (int side2 = Variables::TOP; side2 != Variables::END; side2++) {
            internals[side][side2] = new Road();
        }
    }
    lastChange = 0;
    withinIntersectionTime = -1;
    withinIntersectionOrigin = {-1,-1};
    withinIntersectionLeft = false;
}

Intersection::~Intersection() {
    //delete collection
    delete top;
    delete right;
    delete bottom;
    delete left;
    for (int side = Variables::TOP; side != Variables::END; side++) {
        for (int side2 = Variables::TOP; side2 != Variables::END; side2++) {
            delete internals[side][side2];
        }
    }
}

void Intersection::setCorner(enum Variables::Corner corner, int x, int y) {
    std::pair<int, int> coords = {x, y};
    switch(corner) {
        case Variables::TOPLEFT:
            topLeft = coords;
            break;
        case Variables::TOPRIGHT:
            topRight = coords;
            break;
        case Variables::BOTTOMLEFT:
            bottomLeft = coords;
            break;
        case Variables::BOTTOMRIGHT:
            bottomRight = coords;
            break;
    }
}

void Intersection::setCorners(int x, int y) {
    std::pair<int, int> coords = {x, y};
    topLeft = coords;
    topRight = {coords.first + Variables::INTERSECTION_DIMS, coords.second};
    bottomLeft = {coords.first, coords.second + Variables::INTERSECTION_DIMS};
    bottomRight = {coords.first + Variables::INTERSECTION_DIMS, coords.second + Variables::INTERSECTION_DIMS};
}

void Intersection::setTop(Road* r, pair<int,int> i) {
    //save id of neigbor
    neighbors[Variables::TOP] = i;
    top = r;
    setInternal(Variables::TOP);
}

void Intersection::setRight(Road* r, pair<int,int> i) {
    //save id of neigbor
    neighbors[Variables::RIGHT] = i;
    right = r;
    setInternal(Variables::RIGHT);
}

void Intersection::setBottom(Road* r, pair<int,int> i) {
    //save id of neigbor
    neighbors[Variables::BOTTOM] = i;
    bottom = r;
    setInternal(Variables::BOTTOM);
}

void Intersection::setLeft(Road* r, pair<int,int> i) {
    //save id of neigbor
    neighbors[Variables::LEFT] = i;
    left = r;
    setInternal(Variables::LEFT);
}

void Intersection::setInternal(Variables::Side side) {
    //these four points are the four endpoints of the internal paths
    pair<float, float> topPoint = {(float)topRight.first, (float)topRight.second};
    pair<float, float> leftPoint = {(float)topLeft.first, (float)topLeft.second};
    pair<float, float> rightPoint = {(float)bottomRight.first, (float)bottomRight.second};
    pair<float, float> bottomPoint = {(float)bottomLeft.first, (float)bottomLeft.second};

    topPoint.first -= (Variables::INTERSECTION_DIMS / 4);
    leftPoint.second += (Variables::INTERSECTION_DIMS / 4);
    rightPoint.second -= (Variables::INTERSECTION_DIMS / 4);
    bottomPoint.first += (Variables::INTERSECTION_DIMS / 4);
    //we won't curve right turns since they are so close.
    switch(side) {
        case Variables::TOP:
            //redefine toppoint to the insert point
            topPoint = {(float)topLeft.first, (float)topLeft.second};
            topPoint.first += (Variables::INTERSECTION_DIMS / 4);
            internals[side][Variables::RIGHT]->setShortPath(topPoint,{topPoint.first, (topPoint.second + (Variables::INTERSECTION_DIMS / 2))}, rightPoint, {(rightPoint.first - (Variables::INTERSECTION_DIMS / 2)), rightPoint.second});
            internals[side][Variables::BOTTOM]->setPath(topPoint,bottomPoint);
            internals[side][Variables::LEFT]->setPath(topPoint,leftPoint);
            break;
        case Variables::RIGHT:
            //redefine rightpoint to the insert point
            rightPoint = {(float)topRight.first, (float)topRight.second};
            rightPoint.second += (Variables::INTERSECTION_DIMS / 4);
            internals[side][Variables::TOP]->setPath(rightPoint,topPoint);
            internals[side][Variables::BOTTOM]->setShortPath(rightPoint,{(rightPoint.first - (Variables::INTERSECTION_DIMS / 2)), rightPoint.second}, bottomPoint, {bottomPoint.first, (bottomPoint.second - (Variables::INTERSECTION_DIMS / 2))});
            internals[side][Variables::LEFT]->setPath(rightPoint,leftPoint);
            break;
        case Variables::BOTTOM:
            //redefine bottompoint to the insert point
            bottomPoint = {(float)bottomRight.first, (float)bottomRight.second};
            bottomPoint.first -= (Variables::INTERSECTION_DIMS / 4);
            internals[side][Variables::TOP]->setPath(bottomPoint,topPoint);
            internals[side][Variables::RIGHT]->setPath(bottomPoint,rightPoint);
            internals[side][Variables::LEFT]->setShortPath(bottomPoint,{bottomPoint.first, (bottomPoint.second - (Variables::INTERSECTION_DIMS / 2))}, leftPoint, {(leftPoint.first + (Variables::INTERSECTION_DIMS / 2)), leftPoint.second});
            break;
        case Variables::LEFT:
            //redefine leftpoint to the insert point
            leftPoint = {(float)bottomLeft.first, (float)bottomLeft.second};
            leftPoint.second -= (Variables::INTERSECTION_DIMS / 4);
            internals[side][Variables::TOP]->setShortPath(leftPoint,{(leftPoint.first + (Variables::INTERSECTION_DIMS / 2)), leftPoint.second}, topPoint, {topPoint.first, (topPoint.second + (Variables::INTERSECTION_DIMS / 2))});
            internals[side][Variables::RIGHT]->setPath(leftPoint,rightPoint);
            internals[side][Variables::BOTTOM]->setPath(leftPoint,bottomPoint);
            break;
    }
}

void Intersection::setId(pair<int,int> i) {
    id = i;
}

void Intersection::setWithin(float time,std::pair<int,int> origin, bool left) {
    withinIntersectionTime = time;
    withinIntersectionOrigin = origin;
    withinIntersectionLeft = left;
}

vector<pair<int,int>> Intersection::getCorners() {
    std::vector<std::pair<int,int>> corners = {topLeft, topRight, bottomLeft, bottomRight};
    return corners;
}

pair<int,int> Intersection::getId() {
    return id;
}

pair<int,int> Intersection::getNeighbor(Variables::Side side) {
    return neighbors[side];
}

vector<vector<pair<float, float>>> Intersection::getSampled() {
    vector<vector<pair<float, float>>> vec = {top->getSampled(), right->getSampled(), bottom->getSampled(), left->getSampled()};
    return vec;
}

vector<vector<pair<float, float>>> Intersection::getSampledInternals(Variables::Side side) {
    //note: this function is to test that the internals are properly set.
    //use in conjunction with
    //sampledRoads.push_back(intersections->at(1)->at(1)->getSampledInternals(Variables::BOTTOM));
    vector<vector<pair<float, float>>> vec = {internals[side][Variables::TOP]->getSampled(),internals[side][Variables::RIGHT]->getSampled(),internals[side][Variables::BOTTOM]->getSampled(),internals[side][Variables::LEFT]->getSampled()};
    return vec;
}

//using vectors instead of pairs to make this more readable, vec.at(0) is greenlights, vec.at(1) are redlights
vector<vector<vector<pair<float, float>>>> Intersection::getLights() {
    vector<vector<vector<pair<float, float>>>> vec = {{},{}};
    pair<int,int> noNeighbor = {-1,-1};
    for (int i = 0; i < 4; i++) {
        //check we have a neighbor at the location
        if (neighbors[i] != noNeighbor) {
            //are the vertical pathways green
            if (isVerticalGreen) {
                //greenlights
                if ((i == Variables::TOP)) {
                    vec.at(0).push_back({{(float)topLeft.first, (float)topLeft.second},{(float)topRight.first, (float)topRight.second}});
                }
                if ((i == Variables::BOTTOM)) {
                    vec.at(0).push_back({{(float)bottomLeft.first, (float)bottomLeft.second},{(float)bottomRight.first, (float)bottomRight.second}});
                }
                //setReds
                if ((i == Variables::LEFT)) {
                    vec.at(1).push_back({{(float)topLeft.first, (float)topLeft.second},{(float)bottomLeft.first, (float)bottomLeft.second}});
                }
                if ((i == Variables::RIGHT)) {
                    vec.at(1).push_back({{(float)topRight.first, (float)topRight.second},{(float)bottomRight.first, (float)bottomRight.second}});
                }
            }else {
                //horizontal pathways green
                if ((i == Variables::LEFT)) {
                    vec.at(0).push_back({{(float)topLeft.first, (float)topLeft.second},{(float)bottomLeft.first, (float)bottomLeft.second}});
                }
                if ((i == Variables::RIGHT)) {
                    vec.at(0).push_back({{(float)topRight.first, (float)topRight.second},{(float)bottomRight.first, (float)bottomRight.second}});
                }
                //set redlights:
                if ((i == Variables::TOP)) {
                    vec.at(1).push_back({{(float)topLeft.first, (float)topLeft.second},{(float)topRight.first, (float)topRight.second}});
                }
                if ((i == Variables::BOTTOM)) {
                    vec.at(1).push_back({{(float)bottomLeft.first, (float)bottomLeft.second},{(float)bottomRight.first, (float)bottomRight.second}});
                }
            }
        }
    }
    return vec;
}

float Intersection::getWithinTime() {
    return withinIntersectionTime;
}

pair<int,int> Intersection::getWithinOrigin() {
    return withinIntersectionOrigin;
}

pair<int,int> Intersection::getCenter() {
    return {(topLeft.first + Variables::INTERSECTION_DIMS/2),(topLeft.second + Variables::INTERSECTION_DIMS/2)};
}

bool Intersection::isWithinLeft() {
    return withinIntersectionLeft;
}

//function to determine if a car can pass a side of the intersection
bool Intersection::isPassable(pair<int, int> pos, float time) {
    //reset isWithin if enough time has passed
    //check if we should change the lights
    bool lightsChange = false;
    if (lastChange == 0 || (time - lastChange) > Variables::LIGHTTIME) {
        lightsChange = true;
        lastChange = time;
    }
    //check if passable
    bool passable;
    if (pos.second <= topLeft.second) {
        //is above
        passable = isVerticalGreen;
    }
    if (pos.second >= bottomRight.second) {
        //is below
        passable = isVerticalGreen;
    }
    if (pos.first <= topLeft.first) {
        //is left
        passable = !isVerticalGreen;
    }
    if (pos.first >= bottomRight.first) {
        //is right
        passable = !isVerticalGreen;
    }
    //if we are supposed to change the lights, and the car can't pass
    if (lightsChange && !passable) {
        //change the lights
        isVerticalGreen = !isVerticalGreen;
        //it now is passable
        passable = !passable;
    }
    //check if the car has time to clear the intersection:
    if (lastChange != time && (Variables::LIGHTTIME - (time - lastChange)) < Variables::CLEARTIME) {
        passable = false;
    }
    return passable;
}

void Intersection::print() {
    cout<<"I am an Intersection "<<"corners: "<<topLeft.first<<","<<topLeft.second<<"  "<<topRight.first<<","<<topRight.second<<"  "<<bottomLeft.first<<","<<bottomLeft.second<<"  "<<bottomRight.first<<","<<bottomRight.second<<"\n";
}