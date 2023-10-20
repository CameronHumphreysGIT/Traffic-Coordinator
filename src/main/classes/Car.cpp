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
    sums = {0,0};
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
    cout<<paths->size();
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
        
        if (dir.first != 0 || dir.second != 0) {
            direction = {dir.first, dir.second};
            //normalized vectors describe the direction in terms of a unit vector
            direction = direction.Normalized();
            //deltatime * speed will produce the distance in pixels traveled in the deltatime
            direction = direction * (deltaTime * speed);
            //chassis only holds int positions, so we accumulate the sub pixel position and change as we move.
            //this also allows for a higher frame rate or lower car speed.
            sums = {(sums.first + direction.x), (sums.second + direction.y)};
            //change the pos
            if (sums.first < 0) {
                //need to do this for the sake of negative numbers
                chassis.x += ceil(sums.first);
            }else {
                chassis.x += sums.first;
            }
            if (sums.second < 0) {
                //need to do this for the sake of negative numbers
                chassis.y += ceil(sums.second);
            }else {
                chassis.y += sums.second;
            }
            sums.first -= (int)sums.first;
            sums.second -= (int)sums.second;
        }

        
    }else {
        //we've reached the last waypoint of this path
        currentPath++;
        //reset to first waypoint
        currentWaypoint = 0;
        if (currentPath == paths->size()) {
            state = rest;
        }
    }
}