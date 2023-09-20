#ifndef INFRASTRUCTURE_H
#define INFRASTRUCTURE_H
#include <Intersection.h>
#include <Road.h>
#include <vector>
#include <SDL2/SDL.h>
//TODO: consider making an infrastructure catagory for roads and intersections to inhheiret from, it would be a interface

class Infrastructure {
    private:
        std::vector<Intersection> *intersections;
        std::vector<Road> *roads;
    public:
        Infrastructure();
        void addI(Intersection*);
        bool removeI(Intersection*);
        void addR(Road*);
        bool removeR(Road*);
        void buildInfrastructure(SDL_Surface*);
        void checkAndSetInter(int, int *, std::vector<int> &, int , int );
        void checkAndSetRoad(int, int *, std::vector<int> &, int , int );
        void colourCorners(uint8_t*&, int, int);
        int isCorner(int, int, uint8_t *, int, int);
        bool isColour(int, int, uint8_t *, int, int, bool);
        void print();      
};


#endif