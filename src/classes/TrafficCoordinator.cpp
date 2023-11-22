#include <TrafficCoordinator.h>

TrafficCoordinator::TrafficCoordinator(CarHandler* cHandler) {
    carHandler = cHandler;
}

TrafficCoordinator::~TrafficCoordinator() {

}

void TrafficCoordinator::recalculatePath(int index, float time) {
    //firstly, lets check that we should be recalculating...
    
}