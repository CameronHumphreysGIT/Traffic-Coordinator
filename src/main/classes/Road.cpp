#include <Road.h>
#include <iostream>
#include <Variables.h>

using namespace std;

Road::Road() {
    path = new BezierPath();
}

Road::~Road() {
    delete path;
}

//here we will set the bath to a straightline path between two points
void Road::setPath(pair<float, float> start, pair<float, float> end) {
    path->clearPath();
    //sampling of 1 for a straight line
    path->addCurve({{start.first, start.second}, {start.first, start.second}, {end.first, end.second}, {end.first, end.second}}, 1);
}

vector<pair<float, float>> Road::getSampled() {
    return path->sample();
}

void Road::print() {

}