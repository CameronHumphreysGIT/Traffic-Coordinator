#ifndef INFRASTRUCTURE_H
#define INFRASTRUCTURE_H
#include <Intersection.h>
#include <Road.h>
#include <vector>
#include <SDL2/SDL.h>

class Infrastructure {
    private:
        std::vector<Intersection> *intersections;
        std::vector<Road> *roads;
        void colourCorners(uint8_t*&, int, int);
        int isCorner(int, int, uint8_t *, int, int);
        bool isColour(int, int, uint8_t *, int, int, bool);
        void findClosestXValue(std::vector<int> &, int *, int );
    public:
        Infrastructure();
        void addI(Intersection*);
        bool removeI(Intersection*);
        void addR(Road*);
        bool removeR(Road*);
        Intersection* getI(int );
        void buildInfrastructure(SDL_Surface*);
        void print();      
};


#endif