#include <Intersection.h>
#include <iostream>
#include <Variables.h>
using namespace std;



Intersection::Intersection() {
    top = new Road;
    right = new Road;
    bottom = new Road;
    left = new Road;
    //loop through each side, and set internals
    for (int side = Variables::TOP; side != Variables::END; side++) {
        for (int side2 = Variables::TOP; side2 != Variables::END; side2++) {
            internals[side][side2] = new Road();
        }
    }
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

void Intersection::setTop(Road* r) {
    top = r;
    setInternal(Variables::TOP);
}

void Intersection::setRight(Road* r) {
    right = r;
    setInternal(Variables::RIGHT);
}

void Intersection::setBottom(Road* r) {
    bottom = r;
    setInternal(Variables::BOTTOM);
}

void Intersection::setLeft(Road* r) {
    left = r;
    setInternal(Variables::LEFT);
}

void Intersection::setInternal(Variables::Side side) {
    //these four points are the four endpoints of the internal paths
    pair<int, int> topPoint = this->topRight;
    pair<int, int> leftPoint = this->topLeft;
    pair<int, int> rightPoint = this->bottomRight;
    pair<int, int> bottomPoint = this->bottomLeft;

    topPoint.first -= (Variables::INTERSECTION_DIMS / 4);
    leftPoint.second += (Variables::INTERSECTION_DIMS / 4);
    rightPoint.second -= (Variables::INTERSECTION_DIMS / 4);
    bottomPoint.first += (Variables::INTERSECTION_DIMS / 4);
    //we won't curve right turns since they are so close.
    switch(side) {
        case Variables::TOP:
            //redefine toppoint to the insert point
            topPoint = this->topLeft;
            topPoint.first += (Variables::INTERSECTION_DIMS / 4);
            internals[side][Variables::RIGHT]->setPath(topPoint,{topPoint.first, (topPoint.second + (Variables::INTERSECTION_DIMS / 2))}, rightPoint, {(rightPoint.first - (Variables::INTERSECTION_DIMS / 2)), rightPoint.second});
            internals[side][Variables::BOTTOM]->setPath(topPoint,topPoint,bottomPoint, bottomPoint);
            internals[side][Variables::LEFT]->setPath(topPoint,topPoint,leftPoint,leftPoint);
            break;
        case Variables::RIGHT:
            //redefine rightpoint to the insert point
            rightPoint = this->topRight;
            rightPoint.second += (Variables::INTERSECTION_DIMS / 4);
            internals[side][Variables::TOP]->setPath(rightPoint,rightPoint,topPoint,topPoint);
            internals[side][Variables::BOTTOM]->setPath(rightPoint,{(rightPoint.first - (Variables::INTERSECTION_DIMS / 2)), rightPoint.second}, bottomPoint, {bottomPoint.first, (bottomPoint.second - (Variables::INTERSECTION_DIMS / 2))});
            internals[side][Variables::LEFT]->setPath(rightPoint,rightPoint,leftPoint,leftPoint);
            break;
        case Variables::BOTTOM:
            //redefine bottompoint to the insert point
            bottomPoint = this->bottomRight;
            bottomPoint.first -= (Variables::INTERSECTION_DIMS / 4);
            internals[side][Variables::TOP]->setPath(bottomPoint,bottomPoint,topPoint,topPoint);
            internals[side][Variables::RIGHT]->setPath(bottomPoint,bottomPoint,rightPoint,rightPoint);
            internals[side][Variables::LEFT]->setPath(bottomPoint,{bottomPoint.first, (bottomPoint.second - (Variables::INTERSECTION_DIMS / 2))}, leftPoint, {(leftPoint.first + (Variables::INTERSECTION_DIMS / 2)), leftPoint.second});
            break;
        case Variables::LEFT:
            //redefine leftpoint to the insert point
            leftPoint = this->bottomLeft;
            leftPoint.second -= (Variables::INTERSECTION_DIMS / 4);
            internals[side][Variables::TOP]->setPath(leftPoint,{(leftPoint.first + (Variables::INTERSECTION_DIMS / 2)), leftPoint.second}, topPoint, {topPoint.first, (topPoint.second + (Variables::INTERSECTION_DIMS / 2))});
            internals[side][Variables::RIGHT]->setPath(leftPoint,leftPoint,rightPoint,rightPoint);
            internals[side][Variables::BOTTOM]->setPath(leftPoint,leftPoint,bottomPoint,bottomPoint);
            break;
    }

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

void Intersection::print() {
    cout<<"I am an Intersection "<<"corners: "<<topLeft.first<<","<<topLeft.second<<"  "<<topRight.first<<","<<topRight.second<<"  "<<bottomLeft.first<<","<<bottomLeft.second<<"  "<<bottomRight.first<<","<<bottomRight.second<<"\n";
}