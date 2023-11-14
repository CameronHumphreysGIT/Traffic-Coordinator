#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <utility>
#include <vector>
#include <Variables.h>

class Rectangle {
   protected:
        //coords of the corners
        std::pair<int,int> topLeft;
        std::pair<int,int> topRight;
        std::pair<int,int> bottomRight;
        std::pair<int,int> bottomLeft;
    public:
        virtual ~Rectangle() {};
        virtual void print() = 0;
        std::vector<std::pair<int,int>> getCorners() {
                std::vector<std::pair<int,int>> corners = {topLeft, topRight, bottomLeft, bottomRight};
                return corners;
        };
        void setCorner(enum Variables::Corner corner, int x, int y) {
                std::pair<int, int> coords = {x, y};
                switch(corner) {
                    case Variables::TOPLEFT:
                        topLeft = coords;
                        break;
                    case Variables::TOPRIGHT:
                        topRight = coords;
                        break;
                    case Variables::BOTTOMLEFT:
                        bottomLeft = coords;
                        break;
                    case Variables::BOTTOMRIGHT:
                        bottomRight = coords;
                        break;
                }
        };
        void setCorners(int x, int y) {
                std::pair<int, int> coords = {x, y};
                topLeft = coords;
                topRight = {coords.first + Variables::INTERSECTION_DIMS, coords.second};
                bottomLeft = {coords.first, coords.second + Variables::INTERSECTION_DIMS};
                bottomRight = {coords.first + Variables::INTERSECTION_DIMS, coords.second + Variables::INTERSECTION_DIMS};
        };
};

#endif