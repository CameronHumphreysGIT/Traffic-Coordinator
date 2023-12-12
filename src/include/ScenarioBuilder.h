#ifndef SCENARIOBUILDER_H
#define SCENARIOBUILDER_H
#include <CarHandler.h>
#include <Infrastructure.h>

class ScenarioBuilder {
    public:
        ScenarioBuilder();
        ~ScenarioBuilder();
        bool scenario(int , Uint32 , CarHandler* & , Infrastructure* );
};

#endif