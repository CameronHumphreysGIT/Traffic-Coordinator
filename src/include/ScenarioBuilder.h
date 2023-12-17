#ifndef SCENARIOBUILDER_H
#define SCENARIOBUILDER_H
#include <CarHandler.h>
#include <Infrastructure.h>

class ScenarioBuilder {
    private:
        //a map from the intersection to a queue of vehicles looking to spawn at that intersection.
        map<Intersection*, queue<pair<int, int>>*>* origins;
        //the time that the last vehicles where spawned in
        float lastSpawn;
    public:
        ScenarioBuilder();
        ~ScenarioBuilder();
        bool scenario(int , Uint32 , CarHandler* & , Infrastructure* );
        void spawnRandom(CarHandler* & , vector<vector<Intersection*>*>* , int , float );
        int spawnMore(float , CarHandler*& , vector<vector<Intersection*>*>* );
        void addToQueue(CarHandler*& , vector<vector<Intersection*>*>* );
};

#endif