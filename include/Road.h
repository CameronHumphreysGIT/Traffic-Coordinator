#ifndef ROAD_H
#define ROAD_H
#include <utility>
#include <Variables.h>
#include <BezierPath.h>

class Road {
    private:
        int id;
        BezierPath* path;
    public:
        Road();
        ~Road();
        void setPath(pair<float, float>, pair<float, float>);
        void setPath(pair<float, float>, pair<float, float>, pair<float, float>, pair<float, float>);
        BezierPath getPath();
        vector<pair<float, float>> getSampled();
        void print();
};

#endif