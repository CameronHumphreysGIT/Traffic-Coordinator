#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <System.h>


TEST_CASE("System Initialization") {
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
}