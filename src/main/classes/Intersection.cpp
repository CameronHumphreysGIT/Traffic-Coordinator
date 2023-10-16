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

vector<pair<float, float>> Intersection::getSampled() {
    vector<pair<float, float>> topLine = top->getSampled();
    vector<pair<float, float>> rightLine = right->getSampled();
    vector<pair<float, float>> bottomLine = bottom->getSampled();
    vector<pair<float, float>> leftLine = left->getSampled();
    //one big vector for drawing lines
    topLine.insert(topLine.end(), rightLine.begin(), rightLine.end());
    topLine.insert(topLine.end(), bottomLine.begin(), bottomLine.end());
    topLine.insert(topLine.end(), leftLine.begin(), leftLine.end());
    return topLine;
}

void Intersection::print() {
    cout<<"I am an Intersection "<<"corners: "<<topLeft.first<<","<<topLeft.second<<"  "<<topRight.first<<","<<topRight.second<<"  "<<bottomLeft.first<<","<<bottomLeft.second<<"  "<<bottomRight.first<<","<<bottomRight.second<<"\n";
}