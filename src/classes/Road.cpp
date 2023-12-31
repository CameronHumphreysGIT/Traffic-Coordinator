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

//here we will set the path to a curved path between two points
void Road::setPath(pair<float, float> start, pair<float, float> cp1, pair<float, float> end, pair<float, float> cp2) {
    path->clearPath();
    path->addCurve({{start.first, start.second}, {cp1.first, cp1.second}, {end.first, end.second}, {cp2.first, cp2.second}}, Variables::CURVED_SAMPLES);
}

//here we will set the path to a curved path between two points when we want a lower sampling rate
void Road::setShortPath(pair<float, float> start, pair<float, float> cp1, pair<float, float> end, pair<float, float> cp2) {
    path->clearPath();
    path->addCurve({{start.first, start.second}, {cp1.first, cp1.second}, {end.first, end.second}, {cp2.first, cp2.second}}, Variables::CURVED_SAMPLES/2);
}

vector<pair<float, float>> Road::getSampled() {
    return path->sample();
}

void Road::print() {

}