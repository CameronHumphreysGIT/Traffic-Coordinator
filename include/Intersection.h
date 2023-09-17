#ifndef INTERSECTION_H
#define INTERSECTION_H
#include <Intersection.h>
#include <utility>

using namespace std;

//FIXME: 
enum Corner { TOPLEFT = 1, TOPRIGHT = 2, BOTTOMLEFT = 3, BOTTOMRIGHT = 4, NOT = 0};

class Intersection {
    private:
        int id;
        bool isSet;
        pair<int,int> topLeft;
        pair<int,int> topRight;
        pair<int,int> bottomRight;
        pair<int,int> bottomLeft;
    public:
        Intersection();
        void setCorner(enum Corner corner, int x, int y);
        void setTopLeft(int x, int y);
        void setTopRight(int x, int y);
        void setBottomRight(int x, int y);
        void setBottomLeft(int x, int y);
        void print();
};


#endif