#include <Intersection.h>
#include <iostream>
#include <Variables.h>
using namespace std;



Intersection::Intersection() {

}

void Intersection::print() {
    cout<<"I am an Intersection "<<"corners: "<<topLeft.first<<","<<topLeft.second<<"  "<<topRight.first<<","<<topRight.second<<"  "<<bottomLeft.first<<","<<bottomLeft.second<<"  "<<bottomRight.first<<","<<bottomRight.second<<"\n";
}