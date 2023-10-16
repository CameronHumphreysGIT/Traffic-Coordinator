#include <Intersection.h>
#include <iostream>
#include <Variables.h>
using namespace std;



Intersection::Intersection() {
    top = new Road;
    right = new Road;
    bottom = new Road;
    left = new Road;
}

Intersection::~Intersection() {
    //delete collection
    delete top;
    delete right;
    delete bottom;
    delete left;
}

void Intersection::setTop(Road* r) {
    top = r;
}

void Intersection::setRight(Road* r) {
    right = r;
}

void Intersection::setBottom(Road* r) {
    bottom = r;
}

void Intersection::setLeft(Road* r) {
    left = r;
}

vector<vector<pair<float, float>>> Intersection::getSampled() {
    return {top->getSampled(), right->getSampled(), bottom->getSampled(), left->getSampled()};
}

void Intersection::print() {
    cout<<"I am an Intersection "<<"corners: "<<topLeft.first<<","<<topLeft.second<<"  "<<topRight.first<<","<<topRight.second<<"  "<<bottomLeft.first<<","<<bottomLeft.second<<"  "<<bottomRight.first<<","<<bottomRight.second<<"\n";
}