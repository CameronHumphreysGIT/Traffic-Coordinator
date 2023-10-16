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
    //index of intersection that the Right corner belongs to
    int rightI = 0;
    //stored left boundary of Intersections
    vector<int> IxValues;
    //Index of the row we are adding members to
    int rowNum = 0;
    //the amount of fully realized intersections in our current row.
    //as we create intersections, we add to this number, as we complete intersections, we remove from this number
    //once the whole row has been completed, we change the rowNum
    int incompleteNum = 0;
    intersections->push_back(new vector<Intersection*>);
    for(int y = 0; y < height; y++)  {
        for(int x = 0; x < width; x++)  {
            int cornerReturn = isCorner(x, y, pixelArray, pitch, bytes);
            if (cornerReturn != Variables::NOT) {
                //this means we found a Intersection
                if (cornerReturn == Variables::TOPLEFT) {
                    //add to the incompleteNum
                    incompleteNum++;
                    //make an intersection and set it's top left corner
                    Intersection* intersection = new Intersection();
                    //Add to the infrastructure collection
                    addI(intersection, rowNum);
                    rightI = (intersections->at(rowNum)->size() - 1);
                    //We store the left border of each intersection in the row
                    IxValues.push_back(x);
                } else if (cornerReturn == Variables::BOTTOMLEFT) {
                    findClosestXValue(IxValues, &rightI, x);
                }
                intersections->at(rowNum)->at(rightI)->setCorner((Variables::Corner)cornerReturn, x, y);
                //set the rownum after we have set the last corner
                if (cornerReturn == Variables::BOTTOMRIGHT) {
                    incompleteNum--;
                    if (incompleteNum == 0) {
                        //we've completed all that is in this row
                        rowNum++;
                        //make a new row
                        intersections->push_back(new vector<Intersection*>);
                        //reset rightI and IxValues
                        rightI = 0;
                        IxValues = {};
                    }
                }
            }
        }
    }
    //before we sort we need to manually move two of the intersections:
    Intersection* outlier = intersections->at(0)->at(0);
    //remove first row
    intersections->erase(intersections->begin());
    intersections->at(0)->push_back(outlier);

    //before we sort we need to manually move two of the intersections:
    outlier = intersections->at(4)->at(0);
    //remove 5th row
    auto iter = intersections->begin();
    advance(iter, 4);
    intersections->erase(iter);
    intersections->at(3)->push_back(outlier);

    //sort the arrays by x value of the top left corner:
    for (int i = 0 ; i < intersections->size(); i++) {
        insertionSort(intersections->at(i));
    }
    //finally, colour the corners:
    colourCorners(pixelArray, pitch, bytes);
    //there will be an empty row when done:
    auto erase = intersections->end();
    intersections->erase(erase);
    //TODO change this to a test.
    //verify all are square and have same dims:
    for (int i = 0; i < intersections->size(); i++) {
        for (int i2 = 0; i2 < (intersections->at(i)->size()); i2++) {
            //check square
            vector<pair<int, int>> corners = intersections->at(i)->at(i2)->getCorners();
            if (corners.at(0).first != corners.at(2).first || corners.at(0).second != corners.at(1).second || corners.at(1).first != corners.at(3).first || corners.at(2).second != corners.at(3).second) {
                cout<<"not a square row: "<<i<<" col: "<<i2;
            }
            if (corners.at(1).first - corners.at(0).first == (Variables::INTERSECTION_DIMS + 1)) {
                //too wide by 1.
                cout<<"too wide by 1 row: "<<i<<" col: "<<i2;
            }
            if (corners.at(2).second - corners.at(0).second == (Variables::INTERSECTION_DIMS + 1)) {
                //too tall by 1.
                cout<<"too tall by 1 row: "<<i<<" col: "<<i2;
            }
        }
    }
    buildRoads();
    SDL_UnlockSurface(screenSurface);
}

vector<pair<float, float>> Infrastructure::getSampled() {
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
                    //the path from the node on my bottom to me is the path on my bottom side.
                    me->setBottom(bottom2Me);
                    //outgoing
                    Road* me2Bottom = new Road();
                    me2Bottom->setPath({(me->getCorners().at(2).first + 3),(me->getCorners().at(2).second + 1)}, {(bottom->getCorners().at(0).first + 3),(bottom->getCorners().at(0).second - 1)});
                    bottom->setTop(me2Bottom);
                }
            }
            if (i2 < (intersections->at(i)->size()) - 1) {
                //has space for a road to the right
                //intersection to the right
                Intersection* right = intersections->at(i)->at(i2 + 1);
                //all intersections are 12x12 squares, we want the roads to go in three pixels away from the corners.
                //incoming road.
                Road* right2Me = new Road();
                right2Me->setPath({(right->getCorners().at(0).first - 1),(right->getCorners().at(0).second + 3)}, {(me->getCorners().at(1).first + 1),(me->getCorners().at(1).second + 3)});
                //the path from the node on my right to me is the path on my right side.
                me->setRight(right2Me);
                //outgoing
                Road* me2Right = new Road();
                me2Right->setPath({(me->getCorners().at(3).first + 1),(me->getCorners().at(3).second - 3)}, {(right->getCorners().at(2).first - 1),(right->getCorners().at(2).second - 3)});
                right->setLeft(me2Right);
            }
            vector<pair<float, float>> mySampled = me->getSampled();
            sampledRoads.insert(sampledRoads.end(), mySampled.begin(), mySampled.end());
        }
    }
}

//function for finding the closest intersection in terms of x value below the current intersection
Intersection* Infrastructure::findBelow(int row, int col) {
    cout<<"finding below";
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
                    return getClosest(intersections->at(i)->at(mid - 1), intersections->at(i)->at(mid), target);
                j = mid;
            }else {
                //target is greater than mid
                if (mid < n - 1 && target < intersections->at(i)->at(mid + 1)->getCorners().at(0).first)
                    return getClosest(intersections->at(i)->at(mid), intersections->at(i)->at(mid + 1), target);
                // update i
                i2 = mid + 1; 
            }
        }
    
        // Only single element left after search
        if (abs(target - intersections->at(i)->at(mid)->getCorners().at(0).first) < Variables::MAX_XDIST) {
            return intersections->at(i)->at(mid);
        }else {
            return NULL;
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
    xValues.at(index) = 0;
    *right = index;
}

int Infrastructure::isCorner(int x, int y, uint8_t *pixels, int pitch, int bytes) {
    int returnVal = Variables::NOT;
    //check if Intersection
    if (!isGreen(x, y, pixels, pitch, bytes)) {
        //not a corner of any sort.
        return returnVal;
    }
    
    //check if above is not green
    if (!isGreen(x, (y - 1), pixels, pitch, bytes)) {
        if (!isGreen((x - 1), y, pixels, pitch, bytes)) {
            //top left corner
            returnVal = Variables::TOPLEFT;
        }
        if (!isGreen((x + 1), y, pixels, pitch, bytes)) {
            //top left corner
            returnVal = Variables::TOPRIGHT;
        }
    }
    if (!isGreen(x, (y + 1), pixels, pitch, bytes)) { 
        if (!isGreen((x - 1), y, pixels, pitch, bytes)) {
            //bottom left corner
            returnVal = Variables::BOTTOMLEFT;
        }
        if (!isGreen((x + 1), y, pixels, pitch, bytes)) {
            //bottom right corner
            returnVal = Variables::BOTTOMRIGHT;
        }
    }
    return returnVal;
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