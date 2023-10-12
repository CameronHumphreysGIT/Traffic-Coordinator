#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <utility>
#include <vector>
#include <math.h>
using namespace std;

//overload opperators for use with pairs:
inline pair<float, float> operator +(const pair<float, float>& lhs, const pair<float, float>& rhs) {
    return {lhs.first+rhs.first, lhs.second+rhs.second};
}
inline pair<float, float> operator -(const pair<float, float>& lhs, const pair<float, float>& rhs) {
    return {lhs.first-rhs.first, lhs.second-rhs.second};
}
inline pair<float, float> operator *(const pair<float, float>& lhs, const pair<float, float>& rhs) {
    return {lhs.first*rhs.first, lhs.second*rhs.second};
}
inline pair<float, float> operator *(const pair<float, float>& lhs, const float& rhs) {
    return {lhs.first*rhs, lhs.second*rhs};
}


//bezier curve struct:
struct BezierCurve {
    //p1 and p2 are start and end of the curve
    pair<float,float> p1;
    //c1 and c2 are the control points of the curve.
    pair<float,float> c1;
    pair<float,float> p2;
    pair<float,float> c2;

    //t is between 1 and 0, 0 being the start, 1 being the end, and between being between.
    pair<float, float> CalculateCurvePoint(float t) {
        //error case
        if (t > 1 || t < 0) {
            return {0,0};
        }
        //vars for operation:
        //variable name with repitition is the square or cube of another variable
        float tt = t* t;
        float ttt = tt * t;
        float u = 1.0f - t;
        float uu = u * u;
        float uuu = uu * u;

        pair<float, float> point = (p1 * uuu) + (c1 * (3 * uu * t)) + (p2 * (3 * u * tt)) + (c2 * ttt);
        //round to closest int
        point.first = round(point.first);
        point.second = round(point.second);
        return point;
    }
};

#endif