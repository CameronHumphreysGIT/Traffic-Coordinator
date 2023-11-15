#ifndef CARHANDLER_H
#define CARHANDLER_H
#include <Car.h>
#include <Router.h>

class CarHandler {
    private:
        std::vector<Car*>* cars;
        Router* router;
    public:
        CarHandler();
        ~CarHandler();
        std::pair<std::vector<SDL_Rect*>,std::vector<float*>> getData();
        bool setRoute(int ,std::vector<Intersection*>* );
        void addCar(std::pair<float, float> , float );
        int size();
        void updateCar(int , float );
        bool isNotDone();

};


#endif