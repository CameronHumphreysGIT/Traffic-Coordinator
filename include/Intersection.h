#ifndef INTERSECTION_H
#define INTERSECTION_H
#include <utility>
#include <vector>
#include <Variables.h>
#include <Rectangle.h>

class Intersection : public Rectangle {
    private:
        int id;
        bool isSet;
    public:
        Intersection();
        void print();
};


#endif