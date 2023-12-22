#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <cstdint>
#include <iostream>
#include <System.h>
#include <chrono>


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
            CHECK((system->getCarHandler()->getCar(0)->getPos().first == 316 || system->getCarHandler()->getCar(0)->getPos().first == 315));
            CHECK(system->getCarHandler()->getCar(0)->getPos().second == 65);
        }
        SECTION("end run") {
            cout.clear();
            system->run(INT_MAX);
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
            CHECK(system->getCarHandler()->getCar(0)->getPos().first == 252);
            CHECK(system->getCarHandler()->getCar(0)->getPos().second == 356);

            CHECK(system->getCarHandler()->getCar(1)->getPos().first == 170);
            CHECK((system->getCarHandler()->getCar(1)->getPos().second == 465 || system->getCarHandler()->getCar(1)->getPos().second == 466));

            CHECK((system->getCarHandler()->getCar(2)->getPos().first == 352 || system->getCarHandler()->getCar(2)->getPos().first == 353));
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

            CHECK(system->getCarHandler()->getCar(1)->getPos().first == 233);
            CHECK((system->getCarHandler()->getCar(1)->getPos().second == 295 || system->getCarHandler()->getCar(1)->getPos().second == 294));

            CHECK((system->getCarHandler()->getCar(2)->getPos().first == 600 || system->getCarHandler()->getCar(1)->getPos().second == 599 || system->getCarHandler()->getCar(1)->getPos().second == 598));
            CHECK(system->getCarHandler()->getCar(2)->getPos().second == 383);

            CHECK(system->getCarHandler()->getCar(3)->getPos().first == 229);
            CHECK(system->getCarHandler()->getCar(3)->getPos().second == 535);
            
            CHECK((system->getCarHandler()->getCar(4)->getPos().first == 348 || system->getCarHandler()->getCar(1)->getPos().second == 349));
            CHECK(system->getCarHandler()->getCar(4)->getPos().second == 451);
        }
        SECTION("end run") {
            system->run(INT_MAX);
            CHECK(system->getCarHandler()->getCar(0)->getPos().first == 144);
            CHECK(system->getCarHandler()->getCar(0)->getPos().second == 354);

            CHECK(system->getCarHandler()->getCar(1)->getPos().first == 269);
            CHECK(system->getCarHandler()->getCar(1)->getPos().second == 206);

            CHECK(system->getCarHandler()->getCar(2)->getPos().first == 804);
            CHECK(system->getCarHandler()->getCar(2)->getPos().second == 451);

            CHECK(system->getCarHandler()->getCar(3)->getPos().first == 148);
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

            CHECK((system->getCarHandler()->getCar(7)->getPos().first == 460 || system->getCarHandler()->getCar(7)->getPos().first == 459));
            CHECK(system->getCarHandler()->getCar(7)->getPos().second == 92);
        }
        SECTION("end run") {
            system->run(INT_MAX);
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

TEST_CASE("AStar test") {
    cout.setstate(ios_base::failbit);
    System* system = new System();
    system->init();
    system->loadMedia(true);
    system->buildInfrastructure();
    system->scenario(4);

    stack<Intersection*> route1;
    route1.push(system->getIntersections()->at(8)->at(0));
    route1.push(system->getIntersections()->at(5)->at(0));
    route1.push(system->getIntersections()->at(5)->at(1));
    route1.push(system->getIntersections()->at(5)->at(2));
    route1.push(system->getIntersections()->at(5)->at(3));
    route1.push(system->getIntersections()->at(5)->at(4));
    route1.push(system->getIntersections()->at(5)->at(5));
    route1.push(system->getIntersections()->at(4)->at(0));
    route1.push(system->getIntersections()->at(4)->at(1));
    route1.push(system->getIntersections()->at(3)->at(1));
    route1.push(system->getIntersections()->at(3)->at(2));
    route1.push(system->getIntersections()->at(3)->at(3));


    stack<Intersection*> route2;
    route2.push(system->getIntersections()->at(5)->at(9));
    route2.push(system->getIntersections()->at(5)->at(8));
    route2.push(system->getIntersections()->at(4)->at(2));
    route2.push(system->getIntersections()->at(3)->at(2));
    route2.push(system->getIntersections()->at(3)->at(1));
    route2.push(system->getIntersections()->at(2)->at(1));
    route2.push(system->getIntersections()->at(1)->at(0));
    route2.push(system->getIntersections()->at(0)->at(2));
    route2.push(system->getIntersections()->at(0)->at(1));

    stack<Intersection*> route3;
    route3.push(system->getIntersections()->at(9)->at(7));
    route3.push(system->getIntersections()->at(9)->at(6));
    route3.push(system->getIntersections()->at(10)->at(6));
    route3.push(system->getIntersections()->at(10)->at(5));

    stack<Intersection*> route4;
    route4.push(system->getIntersections()->at(3)->at(3));
    route4.push(system->getIntersections()->at(2)->at(3));
    route4.push(system->getIntersections()->at(1)->at(2));
    route4.push(system->getIntersections()->at(0)->at(4));
    route4.push(system->getIntersections()->at(0)->at(3));
    route4.push(system->getIntersections()->at(0)->at(2));
    route4.push(system->getIntersections()->at(0)->at(1));

    stack<Intersection*> route5;
    route5.push(system->getIntersections()->at(10)->at(4));
    route5.push(system->getIntersections()->at(11)->at(10));
    route5.push(system->getIntersections()->at(11)->at(11));
    route5.push(system->getIntersections()->at(11)->at(12));

    stack<Intersection*> route6;
    route6.push(system->getIntersections()->at(7)->at(8));
    route6.push(system->getIntersections()->at(6)->at(8));
    route6.push(system->getIntersections()->at(5)->at(13));

    stack<Intersection*> route7;
    route7.push(system->getIntersections()->at(9)->at(2));
    route7.push(system->getIntersections()->at(10)->at(2));
    route7.push(system->getIntersections()->at(11)->at(8));

    stack<Intersection*> route8;
    route8.push(system->getIntersections()->at(9)->at(1));
    route8.push(system->getIntersections()->at(9)->at(0));
    route8.push(system->getIntersections()->at(10)->at(0));
    route8.push(system->getIntersections()->at(11)->at(6));
    route8.push(system->getIntersections()->at(12)->at(3));
    route8.push(system->getIntersections()->at(13)->at(0));
    route8.push(system->getIntersections()->at(14)->at(3));

    stack<Intersection*> route9;
    route9.push(system->getIntersections()->at(6)->at(8));
    route9.push(system->getIntersections()->at(6)->at(7));
    route9.push(system->getIntersections()->at(6)->at(6));

    stack<Intersection*> route10;
    route10.push(system->getIntersections()->at(1)->at(0));
    route10.push(system->getIntersections()->at(2)->at(1));

    vector<stack<Intersection*>> routes;
    routes.push_back((stack<Intersection*>)route1);
    routes.push_back((stack<Intersection*>)route2);
    routes.push_back((stack<Intersection*>)route3);
    routes.push_back((stack<Intersection*>)route4);
    routes.push_back((stack<Intersection*>)route5);
    routes.push_back((stack<Intersection*>)route6);
    routes.push_back((stack<Intersection*>)route7);
    routes.push_back((stack<Intersection*>)route8);
    routes.push_back((stack<Intersection*>)route9);
    routes.push_back((stack<Intersection*>)route10);

    //verify paths
    for (int i = 0; i < routes.size(); i++) {
        stack<Intersection*> realRoute = system->getCarHandler()->getRoute(i);
        int size = (int)realRoute.size();
        for (int i2 = 0; i2 < size; i2++) {
            CHECK(realRoute.top() == routes.at(i).top());
            realRoute.pop();
            routes.at(i).pop();
        }
        
    }

    system->close();
    //Delete all objects;
    delete system;
    cout.clear();
}

void takedown(System* system) {
    system->close();
    //Delete all objects;
    delete system;
};

System* setup(int num) {
    cout.setstate(ios_base::failbit);
    System* system = new System();

    system->init();
    system->loadMedia(true);
    system->buildInfrastructure();
    system->scenario(num);
    cout.clear();
    return system;
};

TEST_CASE("Benchmarking") {
    SECTION("Default") {
        double sum = 0;
        for (int i =0; i < 3; i++) {
            System* system = setup(2);
            uint64_t start, end;
            start = system->getCarHandler()->getCar(0)->timeSinceEpochMillisec();
            system->run(INT_MAX);
            end = system->getCarHandler()->getCar(0)->timeSinceEpochMillisec();
            takedown(system);
            sum += (end - start);
            cout<<"====================================================ENDRUN\n";
        }
        cout<<"Average time: "<<(sum/3)/1000<<"\n";
    }
    SECTION("Divert Left Turns") {
        double sum = 0;
        for (int i =0; i < 3; i++) {
            System* system = setup(6);
            uint64_t start, end;
            start = system->getCarHandler()->getCar(0)->timeSinceEpochMillisec();
            system->run(INT_MAX);
            end = system->getCarHandler()->getCar(0)->timeSinceEpochMillisec();
            takedown(system);
            sum += (end - start);
            cout<<"====================================================ENDRUN\n";
        }
        cout<<"Average time: "<<(sum/3)/1000<<"\n";
    }
    SECTION("Divert Stop Lights") {
        double sum = 0;
        for (int i =0; i < 3; i++) {
            System* system = setup(7);
            uint64_t start, end;
            start = system->getCarHandler()->getCar(0)->timeSinceEpochMillisec();
            system->run(INT_MAX);
            end = system->getCarHandler()->getCar(0)->timeSinceEpochMillisec();
            takedown(system);
            sum += (end - start);
            cout<<"====================================================ENDRUN\n";
        }
        cout<<"Average time: "<<(sum/3)/1000<<"\n";
    }
    SECTION("Divert All Turns") {
        double sum = 0;
        for (int i =0; i < 3; i++) {
            System* system = setup(8);
            uint64_t start, end;
            start = system->getCarHandler()->getCar(0)->timeSinceEpochMillisec();
            system->run(INT_MAX);
            end = system->getCarHandler()->getCar(0)->timeSinceEpochMillisec();
            takedown(system);
            sum += (end - start);
            cout<<"====================================================ENDRUN\n";
        }
        cout<<"Average time: "<<(sum/3)/1000<<"\n";
    }
    SECTION("Optimized") {
        double sum = 0;
        for (int i =0; i < 3; i++) {
            System* system = setup(9);
            uint64_t start, end;
            start = system->getCarHandler()->getCar(0)->timeSinceEpochMillisec();
            system->run(INT_MAX);
            end = system->getCarHandler()->getCar(0)->timeSinceEpochMillisec();
            takedown(system);
            sum += (end - start);
            cout<<"====================================================ENDRUN\n";
        }
        cout<<"Average time: "<<(sum/3)/1000<<"\n";
    }
}


