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

//overload opperators for use with pairs:
inline pair<int, int> operator +(const pair<int, int>& lhs, const pair<int, int>& rhs) {
    return {lhs.first+rhs.first, lhs.second+rhs.second};
}
inline pair<int, int> operator -(const pair<int, int>& lhs, const pair<int, int>& rhs) {
    return {lhs.first-rhs.first, lhs.second-rhs.second};
}
inline pair<int, int> operator *(const pair<int, int>& lhs, const pair<int, int>& rhs) {
    return {lhs.first*rhs.first, lhs.second*rhs.second};
}
inline pair<int, int> operator *(const pair<int, int>& lhs, const int& rhs) {
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
            return {0.0f,0.0f};
        }
        //vars for operation:
        //variable name with repitition is the square or cube of another variable
        float tt = t* t;
        float ttt = tt * t;
        float u = 1.0f - t;
        float uu = u * u;
        float uuu = uu * u;

        pair<float, float> point = (p1 * uuu) + (c1 * (3 * uu * t)) + (c2 * (3 * u * tt)) + (p2 * ttt);
        //round to closest int
        point.first = (float)round(point.first);
        point.second = (float)round(point.second);
        return point;
    }
};

//Vector 2 for car physics
struct Vector2 {

    float x;
    float y;

    Vector2(float _x = 0.0f, float _y = 0.0f)
        : x(_x), y(_y) {}

    //--------------------------------------------------------------------------------------------
    //Calculates the square of the magnitude (preferably used instead of Magnitude if possible)   
    //--------------------------------------------------------------------------------------------
    float MagnitudeSqr() {

        return x*x + y*y;
    }
    //-------------------------------------------
    //Calculates the magnitude of the vector     
    //-------------------------------------------
    float Magnitude() {

        return (float)sqrt(x*x + y*y);
    }

    //----------------------------------------------------------------------------------------
    //Returns a directional Vector2 with the same direction as the Vector2 but of length 1    
    // (Does not change the x and y values of the original vector)                                       
    //----------------------------------------------------------------------------------------
    Vector2 Normalized() {

        float mag = Magnitude();

        return Vector2(x / mag, y / mag);
    }

    //-------------------------------------------
    //Calculates the dot product of this vector and the input vector.   
    //-------------------------------------------
    float DotProduct(Vector2 rhs) {
        float first = x * rhs.x;
        float second = y * rhs.y;
        return (first + second);
    }
};

//Vector overrides:
inline Vector2 operator -(const Vector2& lhs, const Vector2& rhs) {
    return {lhs.x-rhs.x, lhs.y-rhs.y};
}
inline Vector2 operator *(const Vector2& lhs, const float& rhs) {
    return {lhs.x*rhs, lhs.y*rhs};
}

#endif