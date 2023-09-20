#ifndef ROAD_H
#define ROAD_H
#include <utility>
#include <vector>
#include <Variables.h>
#include <Rectangle.h>

class Road : public Rectangle {
    private:
        int id;
    public:
        Road();
        void print();
};

#endif