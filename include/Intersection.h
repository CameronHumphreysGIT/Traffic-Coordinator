#ifndef INTERSECTION_H
#define INTERSECTION_H
#include <Variables.h>
#include <Rectangle.h>
#include <Road.h>

class Intersection : public Rectangle {
    private:
        int id;
        Road* top;
        Road* right;
        Road* bottom;
        Road* left;
        //internals is a 2d array of sides
        Road* internals[4][4];
    public:
        Intersection();
        ~Intersection();
        void setTop(Road*);
        void setRight(Road*);
        void setBottom(Road*);
        void setLeft(Road*);
        void setInternal(Variables::Side);
        vector<vector<pair<float, float>>> getSampled();
        vector<vector<pair<float, float>>> getSampledInternals(Variables::Side);
        void print();
};


#endif