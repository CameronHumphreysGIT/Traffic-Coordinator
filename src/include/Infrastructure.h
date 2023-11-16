#ifndef INFRASTRUCTURE_H
#define INFRASTRUCTURE_H
#include <Intersection.h>
#include <Road.h>
#include <vector>
#include <SDL.h>

class Infrastructure {
    private:
        //2d vector of intersections from top to bottom
        std::vector<std::vector<Intersection*>*>* intersections;
        std::vector<std::vector<std::vector<std::pair<float, float>>>> sampledRoads;
        void insertionSort(std::vector<Intersection*> * &);
        void mySwap(Intersection* &, Intersection* &);
        void colourCorners(uint8_t*&, int, int);
        bool isCorner(int, int, uint8_t *, int, int);
        bool isGreen(int, int, uint8_t *, int, int);
        void findClosestXValue(std::vector<int> &, int *, int );
        Intersection* findBelow(int , int );
        Intersection* getClosest(Intersection* , Intersection* , int );
    public:
        Infrastructure();
        ~Infrastructure();
        void addI(Intersection*, int);
        bool removeI(Intersection*);
        Intersection* getI(int, int);
        std::vector<std::vector<Intersection*>*>* getIntersections();
        std::vector<std::vector<std::vector<std::pair<float, float>>>> getSampled();
        std::vector<std::vector<std::vector<std::vector<std::pair<float, float>>>>> getLights();
        void buildInfrastructure(SDL_Surface*);
        void buildRoads();
        void print();      
};


#endif