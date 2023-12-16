#ifndef INTERSECTION_H
#define INTERSECTION_H
#include <Variables.h>
#include <Road.h>

class Intersection {
    private:
        //coords of the corners
        pair<int,int> topLeft;
        pair<int,int> topRight;
        pair<int,int> bottomRight;
        pair<int,int> bottomLeft;
        pair<int,int> id;
        //error values to indicate the selected id hasn't been added
        pair<int,int> neighbors[4] = {{-1,-1}, {-1,-1}, {-1,-1}, {-1,-1}};
        Road* top;
        Road* right;
        Road* bottom;
        Road* left;
        //boolean to represent if the vertical pathways are green
        bool isVerticalGreen;
        //internals is a 2d array of sides
        Road* internals[4][4];
        //float for the time that the last intersection change was made
        float lastChange;
        //the time at which the last car to enter the intersection entered the intersection
        float withinIntersectionTime;
        //the origin (previous intersection) from which the last car to enter the intersection came.
        pair<int,int> withinIntersectionOrigin;
        //boolean to indicate if the car within the Intersection is turning left
        bool withinIntersectionLeft;
    public:
        Intersection();
        ~Intersection();
        void setCorner(enum Variables::Corner corner, int x, int y);
        void setCorners(int x, int y);
        void setTop(Road*, pair<int,int>);
        void setRight(Road*, pair<int,int>);
        void setBottom(Road*, pair<int,int>);
        void setLeft(Road*, pair<int,int>);
        void setInternal(Variables::Side);
        void setId(pair<int,int> );
        void setWithin(float ,std::pair<int,int> ,bool );
        vector<pair<int,int>> getCorners();
        pair<int,int> getId();
        pair<int,int> getNeighbor(Variables::Side);
        vector<vector<pair<float, float>>> getSampled();
        vector<vector<pair<float, float>>> getSampledInternals(Variables::Side);
        vector<vector<vector<pair<float, float>>>> getLights();
        float getWithinTime();
        pair<int,int> getWithinOrigin();
        pair<int,int> getCenter();
        bool isWithinLeft();
        bool isPassable(pair<int, int> , float );
        void collidesWith(pair<int, int> , float , pair<int, int> );
        void print();
};


#endif