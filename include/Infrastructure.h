#ifndef INFRASTRUCTURE_H
#define INFRASTRUCTURE_H
#include <Intersection.h>
#include <vector>
#include <SDL2/SDL.h>
//TODO: consider making an infrastructure catagory for roads and intersections to inhheiret from, it would be a interface

using namespace std;

class Infrastructure {
    private:
        vector<Intersection> *intersections;
    public:
        Infrastructure();
        void addI(Intersection* i);
        bool removeI(Intersection* i);
        void buildIntersections(SDL_Surface*);
        void colourCorners(uint8_t*&, int, int);
        int isCorner(int, int, uint8_t *, int, int);
        bool isGreen(int, int, uint8_t *, int, int);
        void print();      
};


#endif