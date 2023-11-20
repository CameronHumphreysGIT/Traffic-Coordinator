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
            CHECK(system->getCarHandler()->getCar(0)->getPos().first == 251);
            CHECK(system->getCarHandler()->getCar(0)->getPos().second == 356);

            CHECK(system->getCarHandler()->getCar(1)->getPos().first == 170);
            CHECK(system->getCarHandler()->getCar(1)->getPos().second == 465);

            CHECK(system->getCarHandler()->getCar(2)->getPos().first == 353);
            CHECK(system->getCarHandler()->getCar(2)->getPos().second == 378);

            CHECK(system->getCarHandler()->getCar(3)->getPos().first == 262);
            CHECK(system->getCarHandler()->getCar(3)->getPos().second == 335);

            CHECK(system->getCarHandler()->getCar(4)->getPos().first == 595);
            CHECK(system->getCarHandler()->getCar(4)->getPos().second == 453);
        }
        SECTION("15 second run") {
            system->run(15);
            CHECK(system->getCarHandler()->getCar(0)->getPos().first == 144);
            CHECK(system->getCarHandler()->getCar(0)->getPos().second == 354);

            CHECK(system->getCarHandler()->getCar(1)->getPos().first == 234);
            CHECK(system->getCarHandler()->getCar(1)->getPos().second == 293);

            CHECK(system->getCarHandler()->getCar(2)->getPos().first == 600);
            CHECK(system->getCarHandler()->getCar(2)->getPos().second == 383);

            CHECK(system->getCarHandler()->getCar(3)->getPos().first == 208);
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

    SECTION("Scenario 2") {
        System* system = new System();
        //mute the console:
        
        system->init();
        system->loadMedia(true);
        system->buildInfrastructure();
        system->scenario(2);
        SECTION("2 second run") {
            system->run(2);
            CHECK(system->getCarHandler()->getCar(0)->getPos().first == 496);
            CHECK(system->getCarHandler()->getCar(0)->getPos().second == 108);

            CHECK(system->getCarHandler()->getCar(1)->getPos().first == 515);
            CHECK(system->getCarHandler()->getCar(1)->getPos().second == 108);

            CHECK(system->getCarHandler()->getCar(2)->getPos().first == 466);
            CHECK(system->getCarHandler()->getCar(2)->getPos().second == 101);

            CHECK(system->getCarHandler()->getCar(3)->getPos().first == 468);
            CHECK(system->getCarHandler()->getCar(3)->getPos().second == 116);

            CHECK(system->getCarHandler()->getCar(4)->getPos().first == 438);
            CHECK(system->getCarHandler()->getCar(4)->getPos().second == 114);

            CHECK(system->getCarHandler()->getCar(5)->getPos().first == 419);
            CHECK(system->getCarHandler()->getCar(5)->getPos().second == 114);

            CHECK(system->getCarHandler()->getCar(6)->getPos().first == 460);
            CHECK(system->getCarHandler()->getCar(6)->getPos().second == 103);

            CHECK(system->getCarHandler()->getCar(7)->getPos().first == 459);
            CHECK(system->getCarHandler()->getCar(7)->getPos().second == 92);
        }
        SECTION("end run") {
            system->run(1000);
            CHECK(system->getCarHandler()->getCar(0)->getPos().first == 466);
            CHECK(system->getCarHandler()->getCar(0)->getPos().second == 83);

            CHECK(system->getCarHandler()->getCar(1)->getPos().first == 405);
            CHECK(system->getCarHandler()->getCar(1)->getPos().second == 108);

            CHECK(system->getCarHandler()->getCar(2)->getPos().first == 466);
            CHECK(system->getCarHandler()->getCar(2)->getPos().second == 83);

            CHECK(system->getCarHandler()->getCar(3)->getPos().first == 542);
            CHECK(system->getCarHandler()->getCar(3)->getPos().second == 114);
            
            CHECK(system->getCarHandler()->getCar(4)->getPos().first == 466);
            CHECK(system->getCarHandler()->getCar(4)->getPos().second == 83);
            
            CHECK(system->getCarHandler()->getCar(5)->getPos().first == 542);
            CHECK(system->getCarHandler()->getCar(5)->getPos().second == 114);

            CHECK(system->getCarHandler()->getCar(6)->getPos().first == 542);
            CHECK(system->getCarHandler()->getCar(6)->getPos().second == 114);

            CHECK(system->getCarHandler()->getCar(7)->getPos().first == 405);
            CHECK(system->getCarHandler()->getCar(7)->getPos().second == 108);
        }

        system->close();
        //Delete all objects;
        delete system;  
    }

   cout.clear();
}