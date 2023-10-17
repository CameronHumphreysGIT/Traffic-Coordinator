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
    public:
        Intersection();
        ~Intersection();
        void setTop(Road*);
        void setRight(Road*);
        void setBottom(Road*);
        void setLeft(Road*);
        vector<vector<pair<float, float>>> getSampled();
        void print();
};


#endif