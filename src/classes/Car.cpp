#include <Car.h>
#include <Variables.h>

#include <iostream>

using namespace std;

Car::Car(pair<int, int> start, float time) {
    chassis = new SDL_Rect();
    chassis->x = start.first;
    chassis->y = start.second;
    chassis->w = Variables::CAR_WIDTH;
    chassis->h = Variables::CAR_HEIGHT;
    paths = new vector<vector<pair<float, float>>>();
    currentPath = 0;
    currentWaypoint = 0;
    state = rest;
    lastUpdate = time;
    speed = Variables::DEFAULT_SPEED;
    sums = {0,0};
    rotation = 0;
}

Car::~Car() {
    delete chassis;
    delete paths;
}

SDL_Rect* Car::getChassis() {
    return chassis;
}

float* Car::getRotation() {
    return &rotation;
}

pair<int,int> Car::getPos() {
    return {chassis->x, chassis->y};
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
    pair<float, float> oldPos = {chassis->x, chassis->y};
    pair<float, float> dir = paths->at(currentPath).at(currentWaypoint) - oldPos;
    //I used a vector2 here since it's more mathamatically analageous
    Vector2 direction = {dir.first, dir.second};
    bool waypointFlag = false;
    //check if we have "reached" the currentWaypoint
    if (direction.MagnitudeSqr() < Variables::WAYPOINT_TOL) {
        currentWaypoint++;
        waypointFlag = true;
    }
    //makesure haven't reached the end of the path
    if (currentWaypoint < paths->at(currentPath).size()) {
        //redefine dir and direction:
        dir = paths->at(currentPath).at(currentWaypoint) - oldPos;
        direction = {dir.first, dir.second};
        //use a function since we need to also change rotation, and this shouldn't be too bulky
        translate(dir, direction, deltaTime);
        //rotate only once per waypoint:
        if (waypointFlag || currentWaypoint == 0) {
            rotate(direction);
            waypointFlag = false;
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

void Car::translate(pair<float, float> dir, Vector2 direction, float deltaTime) {
    if (dir.first != 0 || dir.second != 0) {
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
            chassis->x += (int)ceil(sums.first);
        }else {
            chassis->x += (int)sums.first;
        }
        if (sums.second < 0) {
            //need to do this for the sake of negative numbers
            chassis->y += (int)ceil(sums.second);
        }else {
            chassis->y += (int)sums.second;
        }
        sums.first -= (int)sums.first;
        sums.second -= (int)sums.second;
    }
}

void Car::rotate(Vector2 direction) {
    //check nonzero:
    if (direction.x != 0 || direction.y != 0) {
        //we can do this since the direction is a vector
        rotation = (atan2(direction.y, direction.x) * Variables::RAD_TO_DEG) - 90.0f;

        //Wraps the angle between 0 and 360 degrees, addition and subtraction is sed to avoid snapping
        //Updated to deal with degrees higher than 360 and -360
        if (rotation > 360.0f) {
            int mul = rotation / 360;
            rotation -= 360.0f * mul;
        }else if (rotation < 0.0f) {
            int mul = (rotation / 360) - 1;
            rotation -= 360.0f * mul;
        }
    }
}

bool Car::isNotRest() {
    if (state == rest) {
        return false;
    }
    return true;
}