#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <iostream>
#include <System.h>


TEST_CASE("System Initialization") {
    cout.setstate(ios_base::failbit);
    System* system = new System();
    REQUIRE(system->init());
    REQUIRE(system->loadMedia(true));
    system->buildInfrastructure();
    //verify infrastructure dimentions and intersections:
    vector<vector<Intersection*>*>* inters = system->getIntersections();
    REQUIRE(inters->size() == 19);
    vector<int> expectedSizes = {9,7,8,8,8,14,9,9,6,9,9,15,12,9,7,6,2,11,14};
    for (int i =0; i < expectedSizes.size(); i++) {
        CHECK(inters->at(i)->size() == expectedSizes.at(i));
    }

    system->close();
    //Delete all objects;
    delete system;
    cout.clear();
}

TEST_CASE("Scenarios") {
    cout.setstate(ios_base::failbit);
    SECTION("Scenario 0") {
        System* system = new System();
        //mute the console:
        
        system->init();
        system->loadMedia(true);
        system->buildInfrastructure();
        system->scenario(0);
        SECTION("5 second run") {
            system->run(5);
            CHECK(system->getCarHandler()->getCar(0)->getPos().first == 100);
            CHECK(system->getCarHandler()->getCar(0)->getPos().second == 74);
        }
        SECTION("15 second run") {
            system->run(15);
            CHECK(system->getCarHandler()->getCar(0)->getPos().first == 316);
            CHECK(system->getCarHandler()->getCar(0)->getPos().second == 65);
        }
        SECTION("end run") {
            system->run(1000);
            CHECK(system->getCarHandler()->getCar(0)->getPos().first == 455);
            CHECK(system->getCarHandler()->getCar(0)->getPos().second == 114);
        }

        system->close();
        //Delete all objects;
        delete system;  
    }

   cout.clear();
}