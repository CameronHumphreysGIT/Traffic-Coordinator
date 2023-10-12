#include <Infrastructure.h>
#include <list>
#include <iostream>
#include <Variables.h>


using namespace std;

Infrastructure::Infrastructure() {
    intersections = new vector<vector<Intersection*>*>();
    roads = new vector<Road*>();
}

Infrastructure::~Infrastructure() {
    //delete all roads:
    for (int i = 0; i < roads->size(); i++) {
        delete roads->at(i);
    }
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
    delete roads;
    delete intersections;
}

void Infrastructure::addI(Intersection* i, int row) {
    intersections->at(row)->push_back(i);
}

void Infrastructure::addR(Road* r) {
    roads->push_back(r);
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
    //sort the arrays by x value of the top left corner:
    for (int i = 0 ; i < intersections->size(); i++) {
        insertionSort(intersections->at(i));
    }
    //finally, colour the corners:
    colourCorners(pixelArray, pitch, bytes);
    //there will be an empty row when done:
    auto erase = intersections->end();
    intersections->erase(erase);

    SDL_UnlockSurface(screenSurface);
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
            for (int i3 = 0; i3 < corners.size(); i3++) {
                //paint the corner
                pixelArray[corners.at(i3).second * pitch + corners.at(i3).first * bytes+0] = 255;
                pixelArray[corners.at(i3).second * pitch + corners.at(i3).first * bytes+1] = 255;
                pixelArray[corners.at(i3).second * pitch + corners.at(i3).first * bytes+2] = 255;
            }
        }
    }
    for (int i = 0; i < roads->size(); i++) {
        //get the corners
        vector<pair<int,int>> corners = roads->at(i)->getCorners();
        for (int i2 = 0; i2 < corners.size(); i2++) {
            //paint the corner
            pixelArray[corners.at(i2).second * pitch + corners.at(i2).first * bytes+0] = 255;
            pixelArray[corners.at(i2).second * pitch + corners.at(i2).first * bytes+1] = 255;
            pixelArray[corners.at(i2).second * pitch + corners.at(i2).first * bytes+2] = 255;
        }
    }
}

void Infrastructure::print() {
    //iterator
    for(int i = 0; i < (intersections->size()); i++) {
        cout<<"Row "<<i<<":\n";
        for (int i2 = 0; i2 < (intersections->at(i)->size()); i2++) {
            cout<<"     Intersection "<<i2<<": ";
            intersections->at(i)->at(i2)->print();
        }
    }
    auto iterator = roads->begin();
    for(int i = 0; i < (roads->size()); i++) {
        cout<<"Road "<<i<<": ";
        (*iterator)->print();
        advance(iterator, 1);
    }
}