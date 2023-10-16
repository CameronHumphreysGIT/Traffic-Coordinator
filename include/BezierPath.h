#ifndef BEZIERPATH_H
#define BEZIERPATH_H

#include "MathHelper.h"
#include <utility>
#include <vector>

class BezierPath {
    private:
        //path is made up of multiple curves
        std::vector<BezierCurve> curves;
        //each curve has a sample amount since it is made up of lines.
        std::vector<int> samples;
    public:
        BezierPath();
        ~BezierPath();
        void addCurve(BezierCurve , int );
        std::vector<pair<float, float>> sample();
        void clearPath();
};
#endif