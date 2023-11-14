#include "BezierPath.h"
#include <iostream>


BezierPath::BezierPath(){}
BezierPath::~BezierPath(){}

void BezierPath::addCurve(BezierCurve curve, int sample) {
    //add curve to the curves:
    curves.push_back(curve);
    samples.push_back(sample);
}

std::vector<pair<float, float>> BezierPath::sample() {
    std::vector<pair<float, float>> sampledPath;
    //We want to go through each path, sample it into a series of points, and then return the path as a long series of points.
    for (int i = 0; i < curves.size(); i++) {
        //this loop will produce sample points along the path.
        for (float t = 0; t <= 1.0f; t += (1.0f / samples[i])) {
            sampledPath.push_back(curves[i].CalculateCurvePoint(t));
        }
        //push in the end of the path
        sampledPath.push_back({curves[i].p2});
    }
    return sampledPath;
}

void BezierPath::clearPath() {
    curves.clear();
    samples.clear();
}