#include <Infrastructure.h>
#include <list>
#include <iostream>
#include <Variables.h>


using namespace std;

Infrastructure::Infrastructure() {
    intersections = new vector<vector<Intersection*>*>();
}

Infrastructure::~Infrastructure() {
    //delete all intersections:
    for (int i = 0; i < intersections->size(); i++) {
        for (int i2 = 0; i2 < (intersections->at(i)->size()); i2++) {
            delete intersections->at(i)->at(i2);
        }
    }
    //delete the collections inside the intersections array:
    for (int i = 0 ; i < intersections->size(); i++) {
        delete intersections->at(i);
    }
    //then, delete the collections:
    delete intersections;
}

void Infrastructure::addI(Intersection* i, int row) {
    intersections->at(row)->push_back(i);
}

Intersection* Infrastructure::getI(int row, int column) {
    return intersections->at(row)->at(column);
}

void Infrastructure::buildInfrastructure(SDL_Surface* screenSurface) {
    //surface must be locked for modification
    SDL_LockSurface(screenSurface);
    //The for loops require a const paramater for their bounds.
    const int height = screenSurface->h;
    const int width = screenSurface->w;
    //we need to modify the pixels, 
    uint8_t* pixelArray = (uint8_t*)screenSurface->pixels;
    int pitch = screenSurface->pitch;
    int bytes = screenSurface->format->BytesPerPixel;
    //Index of the row we are adding members to
    int rowNum = 0;
    //the amount of fully realized intersections in our current row.
    //as we create intersections, we add to this number, as we complete intersections, we remove from this number
    intersections->push_back(new vector<Intersection*>);
    int tolerances[19] = {70,12,12,20,12,12,12,12,16,12,12,12,20,12,12,12,8,30,20};
    for(int y = 0; y < height; y++)  {
        for(int x = 0; x < width; x++)  {
            if (isCorner(x, y, pixelArray, pitch, bytes)) {
                if (intersections->at(rowNum)->size() > 0 && (y - intersections->at(rowNum)->at(0)->getCorners().at(0).second) > tolerances[rowNum]) {
                    //we've completed all that is in this row
                    rowNum++;
                    //make a new row
                    intersections->push_back(new vector<Intersection*>);
                }
                //make an intersection and set it's top left corner
                Intersection* intersection = new Intersection();
                //Add to the infrastructure collection
                addI(intersection, rowNum);
                intersections->at(rowNum)->at(intersections->at(rowNum)->size() - 1)->setCorners(x, y);
            }
        }
    }
    //sort the arrays by x value of the top left corner:
    for (int i = 0 ; i < intersections->size(); i++) {
        insertionSort(intersections->at(i));
        //now that they are sorted, we should set the ids:
        for (int i2 = 0; i2 < intersections->at(i)->size(); i2++) {
            //id is a pair representing the row and column of this intersection
            pair<int, int> id = {i,i2};
            intersections->at(i)->at(i2)->setId(id);
        }
    }
    //finally, colour the corners:
    colourCorners(pixelArray, pitch, bytes);
    buildRoads();
    SDL_UnlockSurface(screenSurface);
}

vector<vector<vector<pair<float, float>>>> Infrastructure::getSampled() {
    sampledRoads.push_back(intersections->at(0)->at(2)->getSampledInternals(Variables::LEFT));
    return sampledRoads;
}

void Infrastructure::buildRoads() {
    //loop through the intersections.
    for (int i = 0; i < intersections->size(); i++) {
        for (int i2 = 0; i2 < (intersections->at(i)->size()); i2++) {
            //since the intersections are sorted, we want to build roads to the intersection on the right, and the intersection bellow.
            Intersection* me = intersections->at(i)->at(i2);
            if (i < (intersections->size() - 1)) {
                Intersection* bottom = findBelow(i, i2);
                //search for the intersection in the rows below that has the closest x value to us:
                if (bottom != NULL) {
                    //has space for a road below
                    //all intersections are 12x12 squares, we want the roads to go in three pixels away from the corners.
                    //incoming road from bottom.
                    Road* bottom2Me = new Road();
                    bottom2Me->setPath({(bottom->getCorners().at(1).first - 3),(bottom->getCorners().at(1).second - 1)}, {(me->getCorners().at(3).first - 3),(me->getCorners().at(3).second + 1)});
                    //the path from the node on my bottom to me is the path on its bottom side.
                    bottom->setTop(bottom2Me, me->getId());
                    //outgoing
                    Road* me2Bottom = new Road();
                    me2Bottom->setPath({(me->getCorners().at(2).first + 3),(me->getCorners().at(2).second + 1)}, {(bottom->getCorners().at(0).first + 3),(bottom->getCorners().at(0).second - 1)});
                    me->setBottom(me2Bottom, bottom->getId());
                }
            }
            if (i2 < (intersections->at(i)->size()) - 1 && !((i == 17 && i2 == 2) || (i == 17 && i2 == 6) || (i == 14 && i2 == 3) || (i == 12 && i2 == 2))) {
                //has space for a road to the right
                //intersection to the right
                Intersection* right = intersections->at(i)->at(i2 + 1);
                //all intersections are 12x12 squares, we want the roads to go in three pixels away from the corners.
                //incoming road.
                Road* right2Me = new Road();
                //outgoing
                Road* me2Right = new Road();
                vector<pair<int, int>> myCorners = me->getCorners();
                vector<pair<int, int>> rightCorners = right->getCorners();
                //special cases
                if (i == 0 && i2 == 0) {
                    //this road is curved
                    //incoming road.
                    right2Me->setPath({(rightCorners.at(0).first - 1),(rightCorners.at(0).second + 3)}, {(rightCorners.at(0).first - 80), (rightCorners.at(0).second - 10)}, {(myCorners.at(1).first + 1),(myCorners.at(1).second + 3)}, {(myCorners.at(1).first + 60),(myCorners.at(1).second + 3)});
                    //outgoing
                    me2Right->setPath({(myCorners.at(3).first + 1),(myCorners.at(3).second - 3)}, {(myCorners.at(3).first + 60),(myCorners.at(3).second + 3)}, {(rightCorners.at(2).first - 1),(rightCorners.at(2).second - 3)}, {(rightCorners.at(2).first - 80),(rightCorners.at(2).second - 10)});
                }else if (i == 0 && i2 == 1) {
                    right2Me->setPath({(rightCorners.at(0).first - 1),(rightCorners.at(0).second + 3)}, {(rightCorners.at(0).first - 80),(rightCorners.at(0).second - 3)}, {(myCorners.at(1).first + 1),(myCorners.at(1).second + 3)}, {(myCorners.at(1).first + 50),(myCorners.at(1).second + 50)});
                    me2Right->setPath({(myCorners.at(3).first + 1),(myCorners.at(3).second - 3)}, {(myCorners.at(3).first + 50),(myCorners.at(3).second + 50)}, {(rightCorners.at(2).first - 1),(rightCorners.at(2).second - 3)}, {(rightCorners.at(2).first - 80),(rightCorners.at(2).second - 3)});
                }else if (i == 3 && i2 == 0) {
                    right2Me->setPath({(rightCorners.at(0).first - 1),(rightCorners.at(0).second + 3)}, {(rightCorners.at(0).first - 80),(rightCorners.at(0).second + 2)}, {(myCorners.at(1).first + 1),(myCorners.at(1).second + 3)}, {(myCorners.at(1).first + 20),(myCorners.at(1).second - 10)});
                    me2Right->setPath({(myCorners.at(3).first + 1),(myCorners.at(3).second - 3)}, {(myCorners.at(3).first + 30),(myCorners.at(3).second - 20)}, {(rightCorners.at(2).first - 1),(rightCorners.at(2).second - 3)}, {(rightCorners.at(2).first - 80),(rightCorners.at(2).second - 3)});
                }else {
                    //straightline
                    right2Me->setPath({(rightCorners.at(0).first - 1),(rightCorners.at(0).second + 3)}, {(myCorners.at(1).first + 1),(myCorners.at(1).second + 3)});
                    me2Right->setPath({(myCorners.at(3).first + 1),(myCorners.at(3).second - 3)}, {(rightCorners.at(2).first - 1),(rightCorners.at(2).second - 3)});
                }
                //the path from the node on my right to me is the path on its left side.
                right->setLeft(right2Me, me->getId());
                //path on my right is the path from me to the node on my right
                me->setRight(me2Right, right->getId());
            }
            vector<vector<pair<float, float>>> mySampled = me->getSampled();
            sampledRoads.push_back(mySampled);
        }
    }
}

//function for finding the closest intersection in terms of x value below the current intersection
Intersection* Infrastructure::findBelow(int row, int col) {
    //some exceptional cases for this func:
    if (row == 0 && col == 0) {
        return intersections->at(row + 5)->at(col);
    }
    if (row == 2 && col == 0) {
        return intersections->at(row + 1)->at(col);
    }
    //all the intersections that don't have an intersection below
    if ((row == 0 && col == 1) || (row == 5 && col == 2) || (row == 11 && col == 2)) {
        return NULL;
    }
    Intersection* me = intersections->at(row)->at(col);
    for (int i = row+1; i < intersections->size(); i++) {
        int n = intersections->at(i)->size();
        //target is the x value of the current intersections topleft corner
        int target = me->getCorners().at(0).first;
        // Doing binary search
        int i2 = 0, j = n, mid = 0;
        //left side case:
        if (target < intersections->at(i)->at(0)->getCorners().at(0).first) {
            if (intersections->at(i)->at(0)->getCorners().at(0).first - target < Variables::MAX_XDIST) {
                return intersections->at(i)->at(0);
            }else {
                continue;
            }   
        }
        //right side
        if (target > intersections->at(i)->at(n - 1)->getCorners().at(0).first) {
            if (target - intersections->at(i)->at(n - 1)->getCorners().at(0).first < Variables::MAX_XDIST) {
                return intersections->at(i)->at(n-1);
            }else {
                continue;
            }  
        }

        while (i2 < j) {
            mid = (i2 + j) / 2;
            cout<<intersections->at(i)->at(mid)->getCorners().at(0).first<<" ";
            if (mid > 0) {
                cout<<intersections->at(i)->at(mid - 1)->getCorners().at(0).first<<" ";
            }
            if (mid < n - 1) {
                cout<<intersections->at(i)->at(mid+1)->getCorners().at(0).first<<" ";
            }
            
            if (intersections->at(i)->at(mid)->getCorners().at(0).first == target) {
                return intersections->at(i)->at(mid);
            }
            /* If target is less than array element,
                then search in left */
            if (target < intersections->at(i)->at(mid)->getCorners().at(0).first) {
                // If target is greater than previous
                // to mid, return closest of two
                if (mid > 0 && target > intersections->at(i)->at(mid - 1)->getCorners().at(0).first)
                    //return the closest of the two
                    if(getClosest(intersections->at(i)->at(mid - 1), intersections->at(i)->at(mid), target) != NULL) {
                        return getClosest(intersections->at(i)->at(mid - 1), intersections->at(i)->at(mid), target);
                    }
                j = mid;
            }else {
                //target is greater than mid
                if (mid < n - 1 && target < intersections->at(i)->at(mid + 1)->getCorners().at(0).first)
                    if (getClosest(intersections->at(i)->at(mid), intersections->at(i)->at(mid + 1), target) != NULL) {
                        return getClosest(intersections->at(i)->at(mid), intersections->at(i)->at(mid + 1), target);
                    }
                // update i
                i2 = mid + 1; 
            }
        }
    
        // Only single element left after search
        if (abs(target - intersections->at(i)->at(mid)->getCorners().at(0).first) < Variables::MAX_XDIST) {
            return intersections->at(i)->at(mid);
        }
    }
    return NULL;
}

//helper for binary search
Intersection* Infrastructure::getClosest(Intersection* val1, Intersection* val2, int target)    {
    int diffVal1 = target - val1->getCorners().at(0).first;
    int diffVal2 = val2->getCorners().at(0).first - target;
    if (diffVal1 >= diffVal2 && (diffVal2 < Variables::MAX_XDIST)) {
        return val2;
    }else if(diffVal1 < Variables::MAX_XDIST) {
        return val1;
    }
    return NULL;
}


//helper for insertionsort
void Infrastructure::mySwap(Intersection* & first, Intersection* & second)  {
    Intersection* temp = first;
    first = second;
    second = temp;
}

//using an insertion sort since the arrays are small.
void Infrastructure::insertionSort(vector<Intersection*> * & vec) {
    int i, key, j;
    for (i = 1; i < vec->size(); i++) {
        //key is set to the x value of the topleft corner of the current intersection
        key = vec->at(i)->getCorners().at(0).first;
        j = i - 1;
        // Move elements of arr[0..i-1],
        // that are greater than key, 
        // to one position ahead of their
        // current position
        while (j >= 0 && vec->at(j)->getCorners().at(0).first > key) {
            //swap the two elements
            mySwap(vec->at(j), vec->at(j+1));
            j = j - 1;
        }
    }
}

void Infrastructure::findClosestXValue(vector<int> &xValues, int *right, int x) {
    //we need to set the rightI to the appropriate index.
    //find the value closest to the current x value...
    int index;
    int min = INT_MAX;
    for (int i = 0; i < xValues.size(); i++) {
        if (abs(x - xValues.at(i)) < min) {
            index = i;
            min = abs(x - xValues.at(i));
        }
    }
    //make sure xvalue is really close to the desired x
    if (abs(xValues.at(index) - x) > 1) {
        //error value
        *right = -1;
    }else {
        xValues.at(index) = 0;
        *right = index;
    }
}

bool Infrastructure::isCorner(int x, int y, uint8_t *pixels, int pitch, int bytes) {
    //check if Intersection
    if (!isGreen(x, y, pixels, pitch, bytes)) {
        //not a corner of any sort.
        return false;
    }
    
    //check if above is not green
    if (!isGreen(x, (y - 1), pixels, pitch, bytes)) {
        if (!isGreen((x - 1), y, pixels, pitch, bytes)) {
            //top left corner
            return true;
        }
    }
    return false;
}

bool Infrastructure::isGreen(int x, int y, uint8_t *pixels, int pitch, int bytes) {
    vector<int> colour = Variables::INTERSECTIONCOLOUR;
    //check that the colour matches
    if (pixels[y * pitch + x * bytes+0] == colour.at(0)) {
        if (pixels[y * pitch + x * bytes+1] == colour.at(1)) {
            if (pixels[y * pitch + x * bytes+2] == colour.at(2)) {
                //is colour.
                return true;
            }
        }
    }
    return false; 
}


//helper function to change the colour of all the corners of the intersections to confirm they are correctly speified
void Infrastructure::colourCorners(uint8_t* &pixelArray, int pitch, int bytes) {
    for (int i = 0; i < intersections->size(); i++) {
        for (int i2 = 0; i2 < (intersections->at(i)->size()); i2++) {
            //get the corners
            vector<pair<int,int>> corners = intersections->at(i)->at(i2)->getCorners();
            //check and change the pimentions
            if (corners.at(1).first - corners.at(0).first == (Variables::INTERSECTION_DIMS + 1)) {
                //too wide by 1.
                //shift topright x value be 1 to the left
                intersections->at(i)->at(i2)->setCorner(Variables::TOPRIGHT, corners.at(1).first - 1, corners.at(1).second);
                intersections->at(i)->at(i2)->setCorner(Variables::BOTTOMRIGHT, corners.at(3).first - 1, corners.at(3).second);
            }
            //update the values;
            corners = intersections->at(i)->at(i2)->getCorners();
            if (corners.at(2).second - corners.at(0).second == (Variables::INTERSECTION_DIMS + 1)) {
                //too tall by 1.
                intersections->at(i)->at(i2)->setCorner(Variables::BOTTOMLEFT, corners.at(2).first, corners.at(2).second - 1);
                intersections->at(i)->at(i2)->setCorner(Variables::BOTTOMRIGHT, corners.at(3).first, corners.at(3).second - 1);
            }
            for (int i3 = 0; i3 < corners.size(); i3++) {
                //paint the corner
                pixelArray[corners.at(i3).second * pitch + corners.at(i3).first * bytes+0] = 255;
                pixelArray[corners.at(i3).second * pitch + corners.at(i3).first * bytes+1] = 255;
                pixelArray[corners.at(i3).second * pitch + corners.at(i3).first * bytes+2] = 255;
            }
        }
    }
}

void Infrastructure::print() {
    for(int i = 0; i < (intersections->size()); i++) {
        cout<<"Row "<<i<<":\n";
        for (int i2 = 0; i2 < (intersections->at(i)->size()); i2++) {
            cout<<"     Intersection "<<i2<<": ";
            intersections->at(i)->at(i2)->print();
        }
    }
}