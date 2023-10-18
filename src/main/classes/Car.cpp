#include <Car.h>
#include <Variables.h>
#include <MathHelper.h>
#include <iostream>

using namespace std;

Car::Car(pair<float, float> start, float time) {
    chassis.x = start.first;
    chassis.y = start.second;
    chassis.w = Variables::CAR_WIDTH;
    chassis.h = Variables::CAR_HEIGHT;
    paths = new vector<vector<pair<float, float>>>;
    currentPath = 0;
    currentWaypoint = 0;
    state = rest;
    lastUpdate = time;
    speed = Variables::DEFAULT_SPEED;
}

Car::~Car() {
    delete[] paths;
    
    /*for (int i =0; i < paths->size(); i++) {
        for (int i2 = 0; i2 < paths->at(i).size(); i2++) {
            delete paths
        }
    }*/
}

SDL_Rect* Car::getChassis() {
    return &chassis;
}

void Car::addPath(vector<pair<float, float>> path) {
    paths->push_back(path);
    //start moving
    state = moving;
}

void Car::update(float time) {
    switch(state) {
        case rest:
            break;
        case moving:
            updatePos(time);
            break;
    }
    //update the update time
    lastUpdate = time;
}

void Car::updatePos(float time) {
    float deltaTime = time - lastUpdate;
    pair<float, float> oldPos = {chassis.x, chassis.y};
    pair<float, float> dir = paths->at(currentPath).at(currentWaypoint) - oldPos;

    //I used a vector2 here since it's more mathamatically analageous
    Vector2 direction = {dir.first, dir.second};
    //check if we have "reached" the currentWaypoint
    if (direction.MagnitudeSqr() < Variables::WAYPOINT_TOL) {
        currentWaypoint++;
    }
    //makesure haven't reached the end of the path
    if (currentWaypoint < paths->at(currentPath).size()) {
        //change direction:
        dir = paths->at(currentPath).at(currentWaypoint) - oldPos;
        direction = {dir.first, dir.second};
        //normalized vectors describe the direction in terms of a unit vector
        direction = direction.Normalized();
        //deltatime * speed will produce the distance in pixels traveled in the deltatime
        direction = direction * (deltaTime * speed);
        //change the pos
        chassis.x += direction.x;
        chassis.y += direction.y;
    }else {
        //we've reached the last waypoint of this path
        currentPath++;
        if (currentPath == paths->size()) {
            state = rest;
        }
    }
}