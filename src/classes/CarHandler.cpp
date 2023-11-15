#include <iostream>
#include <CarHandler.h>

using namespace std;

CarHandler::CarHandler() {
    cars = new vector<Car*>();
    router = new Router();
}

CarHandler::~CarHandler() {
    for (int i = 0; i < cars->size(); i++) {
        delete cars->at(i);
        cars->at(i) = NULL;
    }
    delete cars;
    delete router;
}

//This function returns a pair with a vector of SDL_Rect's and a vector of floats for the rotation of each, in degrees
pair<vector<SDL_Rect*>, vector<float*>>CarHandler::getData() {
    vector<SDL_Rect*> rects;
    vector<float*> rotations;
    for (int i = 0; i < cars->size(); i++) {
        rects.push_back(cars->at(i)->getChassis());
        rotations.push_back(cars->at(i)->getRotation());
    }
    return {rects, rotations};
}

//function used in Testing
Car* CarHandler::getCar(int index) {
    return cars->at(index);
}

bool CarHandler::setRoute(int index, vector<Intersection*>* path) {
    return router->setRoute((cars->at(index)), path);
}

void CarHandler::addCar(pair<float, float> start, float time) {
    Car* car = new Car(start, time);
    cars->push_back(car);
}

int CarHandler::size() {
    return cars->size();
}

void CarHandler::updateCar(int index, float time) {
    cars->at(index)->update(time);
}

//function for checking if the cars have all come to rest
bool CarHandler::isNotDone() {
    for (int i = 0; i < cars->size(); i++) {
        if (cars->at(i)->isNotRest()) {
            return true;
        }
    }
    return false;
}