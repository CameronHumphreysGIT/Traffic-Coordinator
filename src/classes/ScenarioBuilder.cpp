#include <ScenarioBuilder.h>
#include <cassert>
#include <Astar.h>
#include <time.h>
#include <queue>
#include <random>
#include <iostream>

ScenarioBuilder::ScenarioBuilder(){
    origins = new map<Intersection*, queue<pair<int, int>>*>;
    lastSpawn = 0.0f;
}
ScenarioBuilder::~ScenarioBuilder() {
    for (auto it = origins->begin(); it != origins->end(); it++) {
        delete it->second;
    }
    delete origins;
}
/// @brief Function that chooses random start point on the right edge of the map. any time a Intersection start point is repeated, the origins object stores the pair start point into the queue for that intersection in the origins map. 
/// @param carHandler This paramater is used in the cases when the random intersection start point is novel, in this case, the car is added to the carHandler, and a random destination on the opposite(left) side is generated. the a star algorithm object creates the Intersection path between these two points, and is aded to the car's routes.
/// @param intersections This parameter is used to generate the random origins and destinations, and to push the proper queue and queue elements into the origins map.
/// @param amount This is the amount of random cars to spawn
/// @param startTime The time that these random cars are spawned, in seconds.
void ScenarioBuilder::spawnRandom(CarHandler* & carHandler, vector<vector<Intersection*>*>* intersections, int amount, float startTime) {
    lastSpawn = startTime;
    AStar* algo = new AStar();
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<int> distribution(0,(int)(intersections->size() - 1));
    srand((unsigned)time(0));
    for (int i = 0; i < amount; i++) {
        //random row id origin
        int randRow = distribution(generator);
        int size = (int)intersections->at(randRow)->size();
        //randomely determine the side of the map this car comes from:
        int coin = (rand()%2)+1;
        Intersection* origin;
        if (coin == 1) {
            origin = intersections->at(randRow)->at(size - 1);
        }else {
            origin = intersections->at(randRow)->at(0);
        }
        //default start point is center of the intersection origin
        pair<int, int> startPoint = origin->getCenter();
        //have we already spawned a car here?
        auto it = origins->find(origin);
        if (it != origins->end()) {
            //add to it's queue
            it->second->push(startPoint);
        }else {
            queue<pair<int, int>>* addQueue = new queue<pair<int, int>>();
            origins->insert({origin, addQueue});
            carHandler->addCar(startPoint, startTime);
            //now choose a destination.
            int randRow = distribution(generator);
            Intersection* dest = intersections->at(randRow)->at(0);
            if (dest->getCenter() == origin->getCenter()) {
                int size = (int)intersections->at(randRow)->size();
                dest = intersections->at(randRow)->at(size - 1);
            }
            stack<Intersection*> stack = algo->findRoute(origin, dest, intersections);
            assert(carHandler->setRoute((carHandler->size() - 1), &stack));
        }
    }
}
/// @brief This function spawns in more cars according to the queue for each intersection from origins. 
/// @param time the time that spawning occurs, in seconds
/// @param carHandler the carHandler from system, passed as a reference so new cars can be created and routes can be set.
/// @param intersections the intersections 2d list, used to find the intersection destination.
/// @return whether or not anything was spawned when this function was called.
bool ScenarioBuilder::spawnMore(float time, CarHandler*& carHandler, vector<vector<Intersection*>*>* intersections) {
    AStar* algo = new AStar();
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<int> distribution(0,(int)(intersections->size() - 1));
    if ((time - lastSpawn) > Variables::SPAWNDIFF) {
        bool spawned = false;
        //now spawn all vehicles in the queue
        for (auto iter = origins->begin(); iter != origins->end(); iter++) {
            if (!iter->second->empty() && carHandler->isClear(iter->first, time)) {
                pair<int, int> startPoint = iter->second->front();
                iter->second->pop();
                carHandler->addCar(startPoint, time);
                //now choose a destination.
                int randRow = distribution(generator);
                Intersection* dest = intersections->at(randRow)->at(0);
                if (dest == iter->first) {
                    int size = (int)intersections->at(randRow)->size();
                    dest = intersections->at(randRow)->at(size - 1);
                }
                stack<Intersection*> stack = algo->findRoute(iter->first, dest, intersections);
                assert(carHandler->setRoute((carHandler->size() - 1), &stack));
                spawned = true;
            }
        }
        lastSpawn = time;
        return spawned;
    }
    return false;
}

bool ScenarioBuilder::scenario(int scenario, Uint32 time, CarHandler* & carHandler, Infrastructure* infrastructure) {
    //declare all the variables:
    Intersection* i1;
    Intersection* i2;
    Intersection* i3;
    Intersection* i4;
    Intersection* i5;
    Intersection* i6;
    Intersection* i7;
    Intersection* i8;
    Intersection* i9;
    Intersection* i10;
    Intersection* i11;
    Intersection* i12;
    Intersection* i13;
    Intersection* i14;
    stack<Intersection*> stack = {};
    AStar* algo = new AStar();
    //using an if statement instead of switch to avoid redefinition issues
    if (scenario == 0) {
        //This function exists to create a car and a path for the car.
        //update the time
        time = SDL_GetTicks();
        //Car wants time in seconds, the carHandler will make the car
        carHandler->addCar({0,0}, (time * 0.001f));
        i1 = infrastructure->getI(0,0);
        i2 = infrastructure->getI(0,1);
        i3 = infrastructure->getI(0,2);
        i4 = infrastructure->getI(1,0);
        i5 = infrastructure->getI(1,1);
        stack.push(i5);
        stack.push(i4);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(0, &stack));
        stack = {};
        delete algo;
        return true;
    }
    if (scenario == 1) {
        //five cars random start random end.
        //update the time
        time = SDL_GetTicks();
        //Car wants time in seconds, the carHandler will make the car
        carHandler->addCar({319,369}, (time * 0.001f));//orig(9,1) //dest (8,2)
        i1 = infrastructure->getI(9,1);
        i2 = infrastructure->getI(9,0);
        i3 = infrastructure->getI(8,5);
        i4 = infrastructure->getI(8,4);
        i5 = infrastructure->getI(8,3);
        i6 = infrastructure->getI(8,2);
        stack.push(i6);
        stack.push(i5);
        stack.push(i4);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(0, &stack));
        stack = {};
        carHandler->addCar({144,579}, (time * 0.001f));//orig(16,0) //dest (3,0)
        i1 = infrastructure->getI(16,0);
        i2 = infrastructure->getI(14,1);
        i3 = infrastructure->getI(11,4);
        i4 = infrastructure->getI(8,3);
        i5 = infrastructure->getI(8,4);
        i6 = infrastructure->getI(5,4);
        i7 = infrastructure->getI(5,5);
        i8 = infrastructure->getI(4,0);
        i9 = infrastructure->getI(3,0);
        stack.push(i9);
        stack.push(i8);
        stack.push(i7);
        stack.push(i6);
        stack.push(i5);
        stack.push(i4);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(1, &stack));
        stack = {};
        carHandler->addCar({254,353}, (time * 0.001f));//orig(8,5) //dest (11,13)
        i1 = infrastructure->getI(8,5);
        i2 = infrastructure->getI(9,0);
        i3 = infrastructure->getI(9,1);
        i4 = infrastructure->getI(9,2);
        i5 = infrastructure->getI(9,3);
        i6 = infrastructure->getI(9,4);
        i7 = infrastructure->getI(9,5);
        i8 = infrastructure->getI(9,6);
        i9 = infrastructure->getI(9,7);
        i10 = infrastructure->getI(10,7);
        i11 = infrastructure->getI(11,13);
        stack.push(i11);
        stack.push(i10);
        stack.push(i9);
        stack.push(i8);
        stack.push(i7);
        stack.push(i6);
        stack.push(i5);
        stack.push(i4);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(2, &stack));
        stack = {};
        carHandler->addCar({273,215}, (time * 0.001f));//orig(4,0) //dest (16,0)
        i1 = infrastructure->getI(4,0);
        i2 = infrastructure->getI(5,5);
        i3 = infrastructure->getI(6,0);
        i4 = infrastructure->getI(7,0);
        i5 = infrastructure->getI(8,5);
        i6 = infrastructure->getI(9,0);
        i7 = infrastructure->getI(10,0);
        i8 = infrastructure->getI(11,6);
        i9 = infrastructure->getI(12,3);
        i10 = infrastructure->getI(13,0);
        i11 = infrastructure->getI(14,3);
        i12 = infrastructure->getI(14,2);
        i13 = infrastructure->getI(14,1);
        i14 = infrastructure->getI(16,0);
        stack.push(i14);
        stack.push(i13);
        stack.push(i12);
        stack.push(i11);
        stack.push(i10);
        stack.push(i9);
        stack.push(i8);
        stack.push(i7);
        stack.push(i6);
        stack.push(i5);
        stack.push(i4);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(3, &stack));
        stack = {};
        carHandler->addCar({717,453}, (time * 0.001f));//orig(11,12) //dest (8,1)
        i1 = infrastructure->getI(11,12);
        i2 = infrastructure->getI(11,11);
        i3 = infrastructure->getI(11,10);
        i4 = infrastructure->getI(11,9);
        i5 = infrastructure->getI(11,8);
        i6 = infrastructure->getI(11,7);
        i7 = infrastructure->getI(11,6);
        i8 = infrastructure->getI(10,0);
        i9 = infrastructure->getI(9,0);
        i10 = infrastructure->getI(8,5);
        i11 = infrastructure->getI(8,4);
        i12 = infrastructure->getI(8,3);
        i13 = infrastructure->getI(8,2);
        i14 = infrastructure->getI(8,1);
        stack.push(i14);
        stack.push(i13);
        stack.push(i12);
        stack.push(i11);
        stack.push(i10);
        stack.push(i9);
        stack.push(i8);
        stack.push(i7);
        stack.push(i6);
        stack.push(i5);
        stack.push(i4);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(4, &stack));
        stack = {};
        delete algo;
        return true;
    }
    if (scenario == 2) {
        //6 cars around 1 intersection to test traffic
        //update the time
        time = SDL_GetTicks();
        //Car wants time in seconds, the carHandler will make the car
        carHandler->addCar({544,105}, (time * 0.001f));//orig(1,2) //dest (0,3)
        i1 = infrastructure->getI(1,2);
        i2 = infrastructure->getI(1,1);
        i3 = infrastructure->getI(0,3);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(0, &stack));
        stack = {};

        carHandler->addCar({564,105}, (time * 0.001f));//orig(1,2) //dest (1,0)
        i1 = infrastructure->getI(1,2);
        i2 = infrastructure->getI(1,1);
        i3 = infrastructure->getI(1,0);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(1, &stack));
        stack = {};

        carHandler->addCar({457,142}, (time * 0.001f));//orig(2,2) //dest (0,3)
        i1 = infrastructure->getI(2,2);
        i2 = infrastructure->getI(1,1);
        i3 = infrastructure->getI(0,3);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(2, &stack));
        stack = {};

        carHandler->addCar({457,162}, (time * 0.001f));//orig(2,2) //dest (1,2)
        i1 = infrastructure->getI(2,2);
        i2 = infrastructure->getI(1,1);
        i3 = infrastructure->getI(1,2);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(3, &stack));
        stack = {};

        carHandler->addCar({391,105}, (time * 0.001f));//orig(1,0) //dest (0,3)
        i1 = infrastructure->getI(1,0);
        i2 = infrastructure->getI(1,1);
        i3 = infrastructure->getI(0,3);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(4, &stack));
        stack = {};

        carHandler->addCar({371,105}, (time * 0.001f));//orig(1,0) //dest (1,2)
        i1 = infrastructure->getI(1,0);
        i2 = infrastructure->getI(1,1);
        i3 = infrastructure->getI(1,2);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(5, &stack));
        stack = {};

        carHandler->addCar({457,69}, (time * 0.001f));//orig(0,3) //dest (1,2)
        i1 = infrastructure->getI(0,3);
        i2 = infrastructure->getI(1,1);
        i3 = infrastructure->getI(1,2);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(6, &stack));
        stack = {};

        carHandler->addCar({437,69}, (time * 0.001f));//orig(0,3) //dest (1,0)
        i1 = infrastructure->getI(0,3);
        i2 = infrastructure->getI(1,1);
        i3 = infrastructure->getI(1,0);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(7, &stack));
        stack = {};
        delete algo;
        return true;
    }
    if (scenario == 3) {
        //This function exists to create a car and a path for the car, testing the A* algorithm
        //update the time
        time = SDL_GetTicks();
        //Car wants time in seconds, the carHandler will make the car
        carHandler->addCar({0,0}, (time * 0.001f));
        
        i1 = infrastructure->getI(0,0);
        i2 = infrastructure->getI(1,1);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(0, &stack));
        stack = {};
        delete algo;
        return true;
    }
    if (scenario == 4) {
        //update the time
        time = SDL_GetTicks();
        
        //Car wants time in seconds, the carHandler will make the car
        carHandler->addCar({633,175}, (time * 0.001f));
        i1 = infrastructure->getI(3,4);
        i2 = infrastructure->getI(8,0);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(0, &stack));
        stack = {};

        carHandler->addCar({83,69}, (time * 0.001f));
        i1 = infrastructure->getI(0,0);
        i2 = infrastructure->getI(5,9);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(1, &stack));
        stack = {};

        carHandler->addCar({633,453}, (time * 0.001f));
        i1 = infrastructure->getI(11,11);
        i2 = infrastructure->getI(9,7);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(2, &stack));
        stack = {};

        carHandler->addCar({93,69}, (time * 0.001f));
        i1 = infrastructure->getI(0,0);
        i2 = infrastructure->getI(3,3);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(3, &stack));
        stack = {};

        carHandler->addCar({717,491}, (time * 0.001f));
        i1 = infrastructure->getI(12,9);
        i2 = infrastructure->getI(10,4);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(4, &stack));
        stack = {};

        carHandler->addCar({890,212}, (time * 0.001f));
        i1 = infrastructure->getI(4,7);
        i2 = infrastructure->getI(7,8);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(5, &stack));
        stack = {};

        carHandler->addCar({387,489}, (time * 0.001f));
        i1 = infrastructure->getI(12,5);
        i2 = infrastructure->getI(9,2);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(6, &stack));
        stack = {};

        carHandler->addCar({219,566}, (time * 0.001f));
        i1 = infrastructure->getI(15,0);
        i2 = infrastructure->getI(9,1);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(7, &stack));
        stack = {};

        carHandler->addCar({717,261}, (time * 0.001f));
        i1 = infrastructure->getI(5,11);
        i2 = infrastructure->getI(6,8);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(8, &stack));
        stack = {};

        carHandler->addCar({287,141}, (time * 0.001f));
        i1 = infrastructure->getI(2,0);
        i2 = infrastructure->getI(1,0);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(9, &stack));
        stack = {};
        delete algo;
        return true;
    }
    if (scenario == 5) {
        //30 car scenario for testing the algorithm
        //update the time
        time = SDL_GetTicks();
        
        //Car wants time in seconds, the carHandler will make the car
        carHandler->addCar({633,175}, (time * 0.001f));
        i1 = infrastructure->getI(3,4);
        i2 = infrastructure->getI(8,0);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(0, &stack));
        stack = {};

        carHandler->addCar({83,69}, (time * 0.001f));
        i1 = infrastructure->getI(0,0);
        i2 = infrastructure->getI(5,9);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(1, &stack));
        stack = {};

        carHandler->addCar({633,453}, (time * 0.001f));
        i1 = infrastructure->getI(11,11);
        i2 = infrastructure->getI(9,7);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(2, &stack));
        stack = {};

        carHandler->addCar({93,69}, (time * 0.001f));
        i1 = infrastructure->getI(0,0);
        i2 = infrastructure->getI(3,3);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(3, &stack));
        stack = {};

        carHandler->addCar({717,491}, (time * 0.001f));
        i1 = infrastructure->getI(12,9);
        i2 = infrastructure->getI(10,4);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(4, &stack));
        stack = {};

        carHandler->addCar({890,212}, (time * 0.001f));
        i1 = infrastructure->getI(4,7);
        i2 = infrastructure->getI(7,8);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(5, &stack));
        stack = {};

        carHandler->addCar({387,489}, (time * 0.001f));
        i1 = infrastructure->getI(12,5);
        i2 = infrastructure->getI(9,2);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(6, &stack));
        stack = {};

        carHandler->addCar({219,566}, (time * 0.001f));
        i1 = infrastructure->getI(15,0);
        i2 = infrastructure->getI(9,1);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(7, &stack));
        stack = {};

        carHandler->addCar({717,261}, (time * 0.001f));
        i1 = infrastructure->getI(5,11);
        i2 = infrastructure->getI(6,8);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(8, &stack));
        stack = {};

        carHandler->addCar({287,141}, (time * 0.001f));
        i1 = infrastructure->getI(2,0);
        i2 = infrastructure->getI(1,0);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(9, &stack));
        stack = {};

        carHandler->addCar({880,212}, (time * 0.001f));
        i1 = infrastructure->getI(4,7);
        i2 = infrastructure->getI(8,3);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(10, &stack));
        stack = {};

        carHandler->addCar({855,64}, (time * 0.001f));
        i1 = infrastructure->getI(0,8);
        i2 = infrastructure->getI(3,7);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(11, &stack));
        stack = {};

        carHandler->addCar({885,493}, (time * 0.001f));
        i1 = infrastructure->getI(12,11);
        i2 = infrastructure->getI(5,6);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(12, &stack));
        stack = {};

        carHandler->addCar({73,69}, (time * 0.001f));
        i1 = infrastructure->getI(0,0);
        i2 = infrastructure->getI(9,1);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(13, &stack));
        stack = {};

        carHandler->addCar({633,570}, (time * 0.001f));
        i1 = infrastructure->getI(15,5);
        i2 = infrastructure->getI(5,7);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(14, &stack));
        stack = {};

        carHandler->addCar({457,178}, (time * 0.001f));
        i1 = infrastructure->getI(3,2);
        i2 = infrastructure->getI(7,5);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(15, &stack));
        stack = {};
        
        carHandler->addCar({391,105}, (time * 0.001f));
        i1 = infrastructure->getI(1,0);
        i2 = infrastructure->getI(18,13);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(16, &stack));
        stack = {};
                
        carHandler->addCar({391,410}, (time * 0.001f));
        i1 = infrastructure->getI(10,2);
        i2 = infrastructure->getI(11,3);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(17, &stack));
        stack = {};

        carHandler->addCar({250,370}, (time * 0.001f));
        i1 = infrastructure->getI(9,0);
        i2 = infrastructure->getI(18,13);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(18, &stack));
        stack = {};

        carHandler->addCar({633,212}, (time * 0.001f));
        i1 = infrastructure->getI(4,4);
        i2 = infrastructure->getI(11,6);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(19, &stack));
        stack = {};

        carHandler->addCar({801,554}, (time * 0.001f));
        i1 = infrastructure->getI(14,6);
        i2 = infrastructure->getI(10,7);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(20, &stack));
        stack = {};

        carHandler->addCar({85,347}, (time * 0.001f));
        i1 = infrastructure->getI(8,1);
        i2 = infrastructure->getI(15,1);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(21, &stack));
        stack = {};

        carHandler->addCar({456,606}, (time * 0.001f));
        i1 = infrastructure->getI(17,5);
        i2 = infrastructure->getI(12,8);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(22, &stack));
        stack = {};

        carHandler->addCar({387,608}, (time * 0.001f));
        i1 = infrastructure->getI(17,4);
        i2 = infrastructure->getI(17,0);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(23, &stack));
        stack = {};
        
        carHandler->addCar({377,489}, (time * 0.001f));
        i1 = infrastructure->getI(12,5);
        i2 = infrastructure->getI(5,0);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(24, &stack));
        stack = {};

        carHandler->addCar({144,579}, (time * 0.001f));
        i1 = infrastructure->getI(16,0);
        i2 = infrastructure->getI(2,7);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(25, &stack));
        stack = {};
        
        carHandler->addCar({457,69}, (time * 0.001f));
        i1 = infrastructure->getI(0,3);
        i2 = infrastructure->getI(13,5);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(26, &stack));
        stack = {};
                
        carHandler->addCar({172,646}, (time * 0.001f));
        i1 = infrastructure->getI(18,4);
        i2 = infrastructure->getI(13,8);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(27, &stack));
        stack = {};
        
        carHandler->addCar({391,178}, (time * 0.001f));
        i1 = infrastructure->getI(3,1);
        i2 = infrastructure->getI(11,9);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(28, &stack));
        stack = {};
    
        carHandler->addCar({717,103}, (time * 0.001f));
        i1 = infrastructure->getI(1,4);
        i2 = infrastructure->getI(3,3);
        stack = algo->findRoute(i1, i2, infrastructure->getIntersections());
        assert(carHandler->setRoute(29, &stack));
        stack = {};
        delete algo;
        return true;
    }
    if (scenario == 6) {
        //6 cars around 1 intersection to test traffic with diversions of left turning cars
        //update the time
        time = SDL_GetTicks();
        //Car wants time in seconds, the carHandler will make the car
        carHandler->addCar({544,105}, (time * 0.001f));//orig(1,2) //dest (0,3)
        i1 = infrastructure->getI(1,2);
        i2 = infrastructure->getI(1,1);
        i3 = infrastructure->getI(0,3);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(0, &stack));
        stack = {};

        carHandler->addCar({564,105}, (time * 0.001f));//orig(1,2) //dest (1,0)
        i1 = infrastructure->getI(1,2);
        i2 = infrastructure->getI(1,1);
        i3 = infrastructure->getI(1,0);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(1, &stack));
        stack = {};

        carHandler->addCar({457,142}, (time * 0.001f));//orig(2,2) //dest (0,3)
        i1 = infrastructure->getI(2,2);
        i2 = infrastructure->getI(1,1);
        i3 = infrastructure->getI(0,3);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(2, &stack));
        stack = {};

        carHandler->addCar({457,162}, (time * 0.001f));//orig(2,2) //dest (1,2)
        i1 = infrastructure->getI(2,2);
        i2 = infrastructure->getI(1,1);
        i3 = infrastructure->getI(1,2);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(3, &stack));
        stack = {};

        carHandler->addCar({391,105}, (time * 0.001f));//orig(1,0) //dest (0,3) //divert
        i1 = infrastructure->getI(1,0);
        i2 = infrastructure->getI(0,2);
        i3 = infrastructure->getI(0,3);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(4, &stack));
        stack = {};

        carHandler->addCar({371,105}, (time * 0.001f));//orig(1,0) //dest (1,2)
        i1 = infrastructure->getI(1,0);
        i2 = infrastructure->getI(1,1);
        i3 = infrastructure->getI(1,2);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(5, &stack));
        stack = {};

        carHandler->addCar({457,69}, (time * 0.001f));//orig(0,3) //dest (1,2) //divert
        i1 = infrastructure->getI(0,3);
        i2 = infrastructure->getI(0,4);
        i3 = infrastructure->getI(1,2);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(6, &stack));
        stack = {};

        carHandler->addCar({437,69}, (time * 0.001f));//orig(0,3) //dest (1,0)
        i1 = infrastructure->getI(0,3);
        i2 = infrastructure->getI(1,1);
        i3 = infrastructure->getI(1,0);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(7, &stack));
        stack = {};
        delete algo;
        return true;
    }
    if (scenario == 7) {
        //8 cars around 1 intersection to test traffic with diversions around red lights
        //update the time
        time = SDL_GetTicks();
        //Car wants time in seconds, the carHandler will make the car
        carHandler->addCar({554,105}, (time * 0.001f));//orig(1,2) //dest (0,3) //divert
        i1 = infrastructure->getI(1,2);
        i2 = infrastructure->getI(0,4);
        i3 = infrastructure->getI(0,3);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(0, &stack));
        stack = {};

        carHandler->addCar({564,105}, (time * 0.001f));//orig(1,2) //dest (1,0) //divert above
        i1 = infrastructure->getI(1,2);
        i2 = infrastructure->getI(0,4);
        i3 = infrastructure->getI(0,3);
        i4 = infrastructure->getI(0,2);
        i5 = infrastructure->getI(1,0);
        stack.push(i5);
        stack.push(i4);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(1, &stack));
        stack = {};

        carHandler->addCar({457,142}, (time * 0.001f));//orig(2,2) //dest (0,3)
        i1 = infrastructure->getI(2,2);
        i2 = infrastructure->getI(1,1);
        i3 = infrastructure->getI(0,3);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(2, &stack));
        stack = {};

        carHandler->addCar({457,162}, (time * 0.001f));//orig(2,2) //dest (1,2)
        i1 = infrastructure->getI(2,2);
        i2 = infrastructure->getI(1,1);
        i3 = infrastructure->getI(1,2);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(3, &stack));
        stack = {};

        carHandler->addCar({391,105}, (time * 0.001f));//orig(1,0) //dest (0,3) //divert
        i1 = infrastructure->getI(1,0);
        i2 = infrastructure->getI(0,2);
        i3 = infrastructure->getI(0,3);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(4, &stack));
        stack = {};

        carHandler->addCar({371,105}, (time * 0.001f));//orig(1,0) //dest (1,2) //divert above
        i1 = infrastructure->getI(1,0);
        i2 = infrastructure->getI(0,2);
        i3 = infrastructure->getI(0,3);
        i4 = infrastructure->getI(0,4);
        i5 = infrastructure->getI(1,2);
        stack.push(i5);
        stack.push(i4);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(5, &stack));
        stack = {};

        carHandler->addCar({457,69}, (time * 0.001f));//orig(0,3) //dest (1,2)
        i1 = infrastructure->getI(0,3);
        i2 = infrastructure->getI(1,1);
        i3 = infrastructure->getI(1,2);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(6, &stack));
        stack = {};

        carHandler->addCar({437,69}, (time * 0.001f));//orig(0,3) //dest (1,0)
        i1 = infrastructure->getI(0,3);
        i2 = infrastructure->getI(1,1);
        i3 = infrastructure->getI(1,0);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(7, &stack));
        stack = {};
        delete algo;
        return true;
    }
    if (scenario == 8) {
        //6 cars around 1 intersection to test traffic with diversions of left and right turning cars
        //update the time
        time = SDL_GetTicks();
        //Car wants time in seconds, the carHandler will make the car
        carHandler->addCar({544,105}, (time * 0.001f));//orig(1,2) //dest (0,3) //divert
        i1 = infrastructure->getI(1,2);
        i2 = infrastructure->getI(0,4);
        i3 = infrastructure->getI(0,3);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(0, &stack));
        stack = {};

        carHandler->addCar({564,105}, (time * 0.001f));//orig(1,2) //dest (1,0)
        i1 = infrastructure->getI(1,2);
        i2 = infrastructure->getI(1,1);
        i3 = infrastructure->getI(1,0);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(1, &stack));
        stack = {};

        carHandler->addCar({457,142}, (time * 0.001f));//orig(2,2) //dest (0,3)
        i1 = infrastructure->getI(2,2);
        i2 = infrastructure->getI(1,1);
        i3 = infrastructure->getI(0,3);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(2, &stack));
        stack = {};

        carHandler->addCar({457,162}, (time * 0.001f));//orig(2,2) //dest (1,2) //divert
        i1 = infrastructure->getI(2,2);
        i2 = infrastructure->getI(2,3);
        i3 = infrastructure->getI(1,2);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(3, &stack));
        stack = {};

        carHandler->addCar({391,105}, (time * 0.001f));//orig(1,0) //dest (0,3) //divert
        i1 = infrastructure->getI(1,0);
        i2 = infrastructure->getI(0,2);
        i3 = infrastructure->getI(0,3);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(4, &stack));
        stack = {};

        carHandler->addCar({371,105}, (time * 0.001f));//orig(1,0) //dest (1,2)
        i1 = infrastructure->getI(1,0);
        i2 = infrastructure->getI(1,1);
        i3 = infrastructure->getI(1,2);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(5, &stack));
        stack = {};

        carHandler->addCar({457,69}, (time * 0.001f));//orig(0,3) //dest (1,2) //divert
        i1 = infrastructure->getI(0,3);
        i2 = infrastructure->getI(0,4);
        i3 = infrastructure->getI(1,2);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(6, &stack));
        stack = {};

        carHandler->addCar({437,69}, (time * 0.001f));//orig(0,3) //dest (1,0) //divert
        i1 = infrastructure->getI(0,3);
        i2 = infrastructure->getI(0,2);
        i3 = infrastructure->getI(1,0);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(7, &stack));
        stack = {};
        delete algo;
        return true;
    }
    if (scenario == 9) {
        //6 cars around 1 intersection to test traffic with diversions of left and right turning cars
        //update the time
        time = SDL_GetTicks();
        //Car wants time in seconds, the carHandler will make the car
        carHandler->addCar({544,105}, (time * 0.001f));//orig(1,2) //dest (0,3) //divert
        i1 = infrastructure->getI(1,2);
        i2 = infrastructure->getI(0,4);
        i3 = infrastructure->getI(0,3);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(0, &stack));
        stack = {};

        carHandler->addCar({564,105}, (time * 0.001f));//orig(1,2) //dest (1,0)
        i1 = infrastructure->getI(1,2);
        i2 = infrastructure->getI(1,1);
        i3 = infrastructure->getI(1,0);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(1, &stack));
        stack = {};

        carHandler->addCar({457,142}, (time * 0.001f));//orig(2,2) //dest (0,3)
        i1 = infrastructure->getI(2,2);
        i2 = infrastructure->getI(1,1);
        i3 = infrastructure->getI(0,3);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(2, &stack));
        stack = {};

        carHandler->addCar({457,162}, (time * 0.001f));//orig(2,2) //dest (1,2) //divert
        i1 = infrastructure->getI(2,2);
        i2 = infrastructure->getI(2,3);
        i3 = infrastructure->getI(1,2);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(3, &stack));
        stack = {};

        carHandler->addCar({391,105}, (time * 0.001f));//orig(1,0) //dest (0,3) //divert
        i1 = infrastructure->getI(1,0);
        i2 = infrastructure->getI(0,2);
        i3 = infrastructure->getI(0,3);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(4, &stack));
        stack = {};

        carHandler->addCar({371,105}, (time * 0.001f));//orig(1,0) //dest (1,2)
        i1 = infrastructure->getI(1,0);
        i2 = infrastructure->getI(1,1);
        i3 = infrastructure->getI(1,2);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(5, &stack));
        stack = {};

        carHandler->addCar({457,69}, (time * 0.001f));//orig(0,3) //dest (1,2) //divert 
        i1 = infrastructure->getI(0,3);
        i2 = infrastructure->getI(0,4);
        i3 = infrastructure->getI(1,2);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(6, &stack));
        stack = {};

        carHandler->addCar({437,69}, (time * 0.001f));//orig(0,3) //dest (1,0)
        i1 = infrastructure->getI(0,3);
        i2 = infrastructure->getI(1,1);
        i3 = infrastructure->getI(1,0);
        stack.push(i3);
        stack.push(i2);
        stack.push(i1);
        assert(carHandler->setRoute(7, &stack));
        stack = {};
        delete algo;
        return true;
    }
    if (scenario == 10) {
        spawnRandom(carHandler, infrastructure->getIntersections(), 50, (time * 0.001f));
        delete algo;
        return true;
    }
    delete algo;
    return false;
}