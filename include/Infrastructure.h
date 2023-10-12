#ifndef INFRASTRUCTURE_H
#define INFRASTRUCTURE_H
#include <Intersection.h>
#include <Road.h>
#include <vector>
#include <SDL2/SDL.h>

class Infrastructure {
    private:
        //2d vector of intersections from top to bottom
        std::vector<std::vector<Intersection*>*> *intersections;
        std::vector<Road*> *roads;
        void insertionSort(std::vector<Intersection*> * &);
        void mySwap(Intersection* &, Intersection* &);
        void colourCorners(uint8_t*&, int, int);
        int isCorner(int, int, uint8_t *, int, int);
        bool isGreen(int, int, uint8_t *, int, int);
        void findClosestXValue(std::vector<int> &, int *, int );
    public:
        Infrastructure();
        ~Infrastructure();
        void addI(Intersection*, int);
        bool removeI(Intersection*);
        void addR(Road*);
        bool removeR(Road*);
        Intersection* getI(int, int);
        void buildInfrastructure(SDL_Surface*);
        void print();      
};


#endif