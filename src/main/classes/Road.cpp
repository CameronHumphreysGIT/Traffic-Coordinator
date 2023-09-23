#include <Road.h>
#include <iostream>
#include <Variables.h>

using namespace std;

Road::Road() {
    
}

void Road::print() {
    cout<<"I am an Road "<<"corners: "<<topLeft.first<<","<<topLeft.second<<"  "<<topRight.first<<","<<topRight.second<<"  "<<bottomLeft.first<<","<<bottomLeft.second<<"  "<<bottomRight.first<<","<<bottomRight.second<<"\n";
}