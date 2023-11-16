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

    SECTION("Scenario 1") {
        System* system = new System();
        //mute the console:
        
        system->init();
        system->loadMedia(true);
        system->buildInfrastructure();
        system->scenario(1);
        SECTION("5 second run") {
            system->run(5);
            CHECK(system->getCarHandler()->getCar(0)->getPos().first == 216);
            CHECK(system->getCarHandler()->getCar(0)->getPos().second == 358);

            CHECK(system->getCarHandler()->getCar(1)->getPos().first == 170);
            CHECK((system->getCarHandler()->getCar(1)->getPos().second == 465 || system->getCarHandler()->getCar(1)->getPos().second == 466));

            CHECK((system->getCarHandler()->getCar(2)->getPos().first == 254 || system->getCarHandler()->getCar(2)->getPos().first == 253));
            CHECK(system->getCarHandler()->getCar(2)->getPos().second == 366);

            CHECK(system->getCarHandler()->getCar(3)->getPos().first == 262);
            CHECK(system->getCarHandler()->getCar(3)->getPos().second == 335);

            CHECK(system->getCarHandler()->getCar(4)->getPos().first == 595);
            CHECK(system->getCarHandler()->getCar(4)->getPos().second == 453);
        }
        SECTION("15 second run") {
            system->run(15);
            CHECK(system->getCarHandler()->getCar(0)->getPos().first == 144);
            CHECK(system->getCarHandler()->getCar(0)->getPos().second == 354);

            CHECK(system->getCarHandler()->getCar(1)->getPos().first == 254);
            CHECK(system->getCarHandler()->getCar(1)->getPos().second == 270);

            CHECK((system->getCarHandler()->getCar(2)->getPos().first == 485 || system->getCarHandler()->getCar(2)->getPos().first == 486));
            CHECK(system->getCarHandler()->getCar(2)->getPos().second == 378);

            CHECK((system->getCarHandler()->getCar(3)->getPos().first == 201 || system->getCarHandler()->getCar(3)->getPos().first == 202));
            CHECK(system->getCarHandler()->getCar(3)->getPos().second == 550);
            
            CHECK(system->getCarHandler()->getCar(4)->getPos().first == 348);
            CHECK(system->getCarHandler()->getCar(4)->getPos().second == 451);
        }
        SECTION("end run") {
            system->run(1000);
            CHECK(system->getCarHandler()->getCar(0)->getPos().first == 144);
            CHECK(system->getCarHandler()->getCar(0)->getPos().second == 354);

            CHECK(system->getCarHandler()->getCar(1)->getPos().first == 269);
            CHECK(system->getCarHandler()->getCar(1)->getPos().second == 206);

            CHECK(system->getCarHandler()->getCar(2)->getPos().first == 804);
            CHECK(system->getCarHandler()->getCar(2)->getPos().second == 451);

            CHECK(system->getCarHandler()->getCar(3)->getPos().first == 147);
            CHECK(system->getCarHandler()->getCar(3)->getPos().second == 577);
            
            CHECK(system->getCarHandler()->getCar(4)->getPos().first == 99);
            CHECK(system->getCarHandler()->getCar(4)->getPos().second == 350);
        }

        system->close();
        //Delete all objects;
        delete system;  
    }

   cout.clear();
}