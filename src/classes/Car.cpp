#include <Car.h>
#include <Variables.h>

#include <iostream>
#include <chrono>

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
    sums = {0.0f,0.0f};
    rotation = 0;
    internals = {};
    wait = NULL;
    lastWaitPos = {-1, -1};
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

vector<vector<pair<float, float>>> Car::getPaths() {
    //only want remaining paths
    vector<vector<pair<float, float>>> vec;
    if (currentPath + 1 < paths->size()) {
        //we draw differently if our current path is curved.
        if (paths->at(currentPath).size() == 1) {
            vec = vector<vector<pair<float, float>>>(paths->begin() + (currentPath + 1), paths->end());
            //insert the next waypoint, but draw it from current position
            vec.insert((vec.begin() + 1), {{(float)chassis->x, (float)chassis->y},paths->at(currentPath).at(currentWaypoint)});
        }else {
            vec = vector<vector<pair<float, float>>>(paths->begin() + (currentPath + 1), paths->end());
            //remove prior waypoints from current path, then draw from position to current waypoint
            vector<pair<float, float>> current = vector<pair<float, float>>(paths->at(currentPath).begin() + (currentWaypoint), paths->at(currentPath).end());
            current.insert((current.begin() + 1), {{(float)chassis->x, (float)chassis->y}, paths->at(currentPath).at(currentWaypoint)});
            vec.insert((vec.begin() + 1), current);
        }

    }else {
        vec.push_back({{(float)chassis->x, (float)chassis->y},paths->at(currentPath).at(currentWaypoint)});
    }
    return vec;
}

void Car::addPath(vector<pair<float, float>> path, bool isInternal) {
    if (isInternal) {
        internals.push((int)paths->size());
    }
    paths->push_back(path);
    //start moving
    state = moving;
    start = timeSinceEpochMillisec();
}

void Car::update(float time) {
    switch(state) {
        case rest:
            break;
        case redlight:
            break;
        case moving:
            updatePos(time);
            break;
        case movetowait:
            if (withinTwoCarlengths()) {
                state = waiting;
                break;
            }else if ((lastWaitPos != pair<int, int>({-1,-1})) && (lastWaitPos != wait->getPos())) {
                //the guy we are waiting on has started to move.
                state = moving;
                wait = NULL;
                lastWaitPos = {-1, -1};
                updatePos(time);
                break;
            }else {
                updatePos(time);
            }
        case waiting:
            if (!withinTwoCarlengths()) {
                state = moving;
                wait = NULL;
                lastWaitPos = {-1, -1};
                updatePos(time);
            }
            break;
    }
    //update the update time
    lastUpdate = time;
}

//update function, that stops the car, or starts it again
void Car::update(float time, bool isStopped) {
    if (isStopped) {
        state = redlight;
    }else {
        //this means we are going to start moving
        state = moving;
        //get rid of top internal road
        internals.pop();
    }
    switch(state) {
        case rest:
            break;
        case redlight:
            break;
        case moving:
            updatePos(time);
            break;
    }
    //update the update time
    lastUpdate = time;
}

//function to wait behind a given car.
void Car::waitBehind(Car* c) {
    //save the car object and that way we can check their position.
    wait = c;
    state = movetowait;
}

bool Car::withinTwoCarlengths() {
    if (wait == NULL) {
        return false;
    }
    pair<float, float> carpos = {(float)wait->getPos().first, (float)wait->getPos().second};
    //get direction
    pair<float, float> currentPos = {(float)chassis->x, (float)chassis->y};
    pair<float, float> dir = carpos - currentPos;
    Vector2 direction = {dir.first, dir.second};
    lastWaitPos = wait->getPos();
    return (direction.Magnitude() <= (Variables::CAR_HEIGHT * 2));
}

//helper function found on stackoverflow.
uint64_t Car::timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void Car::updatePos(float time) {
    float deltaTime = time - lastUpdate;
    pair<float, float> oldPos = {(float)chassis->x, (float)chassis->y};
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
            state = end;
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
            int mul = (int)(rotation / 360);
            rotation -= 360.0f * mul;
        }else if (rotation < 0.0f) {
            int mul = (int)((rotation / 360) - 1);
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

//function to tell if current path is an internal road
bool Car::isInternal() {
    if (internals.empty()) {
        return false;
    }
    return (currentPath == internals.front());
}
//TODO consider changing these to a return value, downside: need to give enum to other classes.
bool Car::isWaiting() {
    return state == waiting;
}

bool Car::isMoveToWait() {
    return state == movetowait;
}

bool Car::isRedLight() {
    return state == redlight;
}

bool Car::isLeftTurning() {
    //first check that we are on an internal road:
    if (!isInternal()) {
        return false;
    }else {
        //we are on an internal path
        return (paths->at(currentPath).size() > 3);
    }
    return state == redlight;
}

bool Car::isAtEnd() {
    return (state == end);
}