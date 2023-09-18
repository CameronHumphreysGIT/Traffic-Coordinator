#include <Intersection.h>
#include <iostream>

using namespace std;



Intersection::Intersection() {

}

void Intersection::setCorner(enum Corner corner, int x, int y) {
    pair<int, int> coords = {x, y};
    switch(corner) {
        case TOPLEFT:
            topLeft = coords;
            break;
        case TOPRIGHT:
            topRight = coords;
            break;
        case BOTTOMLEFT:
            bottomLeft = coords;
            break;
        case BOTTOMRIGHT:
            bottomRight = coords;
            break;
    }
}

vector<pair<int, int>> Intersection::getCorners() {
    vector<pair<int,int>> corners = {topLeft, topRight, bottomRight, bottomLeft};
    return corners;
}

void Intersection::print() {
    cout<<"I am an Intersection "<<"corners: "<<topLeft.first<<","<<topLeft.second<<"  "<<topRight.first<<","<<topRight.second<<"  "<<bottomLeft.first<<","<<bottomLeft.second<<"  "<<bottomRight.first<<","<<bottomRight.second;
}