#include <Infrastructure.h>
#include <list>
#include <iostream>
#include <Variables.h>


using namespace std;

Infrastructure::Infrastructure() {
    intersections = new vector<Intersection>();
    roads = new vector<Road>();
}

void Infrastructure::addI(Intersection* i) {
    intersections->push_back(*i);
}

void Infrastructure::addR(Road* r) {
    roads->push_back(*r);
}

Intersection* Infrastructure::getI(int index) {
    return &intersections->at(index);
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
    //index of right Road corner
    int rightR = 0;
    //stored left boundary of Intersections
    vector<int> IxValues;
    //stored left boundary of Roads
    vector<int> RxValues;
    //TODO this algorithm is a first draft, maybe a more efficient solution
    //TODO consider ommitting all roads, and constructing them as the corners of intersections?
    pixelArray[60 * pitch + 200 * bytes+0] = 255;
    pixelArray[60 * pitch + 200 * bytes+1] = 255;
    pixelArray[60 * pitch + 200 * bytes+2] = 255;
    for(int y = 0; y < height; y++)  {
        //cout<<"loop";
        for(int x = 0; x < width; x++)  {
            int cornerReturn = isCorner(x, y, pixelArray, pitch, bytes);
            if (cornerReturn > 0) {
                //this means we found a Intersection
                if (cornerReturn == Variables::TOPLEFT) {
                    //make an intersection and set it's top left corner
                    Intersection* intersection = new Intersection();
                    //Add to the infrastructure collection
                    addI(intersection);
                    rightI = (intersections->size() - 1);
                    //We store the left border of each intersection
                    IxValues.push_back(x);
                } else if (cornerReturn == Variables::BOTTOMLEFT) {
                    findClosestXValue(IxValues, &rightI, x);
                }
                intersections->at(rightI).setCorner((Variables::Corner)cornerReturn, x, y);
            }
            if (cornerReturn < 0) {
                //make it positive
                cornerReturn = cornerReturn * -1;
                //we found a Road
                if (cornerReturn == Variables::TOPLEFT) {
                    //make an intersection and set it's top left corner
                    Road* road = new Road();
                    //Add to the infrastructure collection
                    addR(road);
                    rightR = (roads->size() - 1);
                    //We store the left border of each intersection
                    RxValues.push_back(x);
                } else if (cornerReturn == Variables::BOTTOMLEFT) {
                    findClosestXValue(RxValues, &rightR, x);
                }
                roads->at(rightR).setCorner((Variables::Corner)cornerReturn, x, y);
            } 
        }
    }
    //finally, colour the corners:
    colourCorners(pixelArray, pitch, bytes);

    SDL_UnlockSurface(screenSurface);
}
//first, figure out how road 0 got it's corners.
//we don't need to store x values, we just need to count how many roads we have encountered in the row...?
//rework this...
void Infrastructure::findClosestXValue(vector<int> &xValues, int *right, int x) {
    //we need to set the rightR to the appropriate index.
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
    bool road;
    //check if Intersection
    if (isColour(x, y, pixels, pitch, bytes, false)) {
        road = false;
    }else if (isColour(x, y, pixels, pitch, bytes, true)) {
        road = true;
    }else {
        return Variables::NOT;
    }
    int returnVal = Variables::NOT;
    //check if above is not green
    if (!isColour(x, (y - 1), pixels, pitch, bytes, road)) {
        //roads need a more rigid definition since they are diagonal.
        if (road) {
            if (!isColour((x - 1), y, pixels, pitch, bytes, road) && (isColour(x, (y - 2), pixels, pitch, bytes, !road) || isColour((x - 2), y, pixels, pitch, bytes, !road))) {
                //top left corner
                returnVal = Variables::TOPLEFT;
            }
            if (!isColour((x + 1), y, pixels, pitch, bytes, road) && ((isColour(x, (y - 2), pixels, pitch, bytes, !road) || isColour((x + 2), y, pixels, pitch, bytes, !road)))) {
                //top right corner
                returnVal = Variables::TOPRIGHT;
            }
        }else {
            if (!isColour((x - 1), y, pixels, pitch, bytes, road)) {
                //top left corner
                returnVal = Variables::TOPLEFT;
            }
            if (!isColour((x + 1), y, pixels, pitch, bytes, road)) {
                //top left corner
                returnVal = Variables::TOPRIGHT;
            }
        }
    }
    if (!isColour(x, (y + 1), pixels, pitch, bytes, road)) {
        //roads need a more rigid definition since they are diagonal.
        if (road) {
            if (!isColour((x - 1), y, pixels, pitch, bytes, road) && (isColour(x, (y + 2), pixels, pitch, bytes, !road) || isColour((x - 2), y, pixels, pitch, bytes, !road))) {
                //bottom left corner
                returnVal = Variables::BOTTOMLEFT;
            }
            if (!isColour((x + 1), y, pixels, pitch, bytes, road) && (isColour(x, (y + 2), pixels, pitch, bytes, !road) || isColour((x + 2), y, pixels, pitch, bytes, !road))) {
                //bottom right corner
                returnVal = Variables::BOTTOMRIGHT;
            } 
        }else {
            if (!isColour((x - 1), y, pixels, pitch, bytes, road)) {
                //bottom left corner
                returnVal = Variables::BOTTOMLEFT;
            }
            if (!isColour((x + 1), y, pixels, pitch, bytes, road)) {
                //bottom right corner
                returnVal = Variables::BOTTOMRIGHT;
            }
        }
    }
    if (road) {
        return -returnVal;
    }else {
        return returnVal;
    }
}

bool Infrastructure::isColour(int x, int y, uint8_t *pixels, int pitch, int bytes, bool road) {
    vector<int> colour;
    if (road) {
        //look for road colours
        colour = Variables::ROADCOLOUR;
    }else {
        colour = Variables::INTERSECTIONCOLOUR;
    }
    //though it is a little strange, I've imbedded the if statements to make it more readible instead of one extremely long line.
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
        //get the corners
        vector<pair<int,int>> corners = intersections->at(i).getCorners();
        for (int i2 = 0; i2 < corners.size(); i2++) {
            //paint the corner
            pixelArray[corners.at(i2).second * pitch + corners.at(i2).first * bytes+0] = 255;
            pixelArray[corners.at(i2).second * pitch + corners.at(i2).first * bytes+1] = 255;
            pixelArray[corners.at(i2).second * pitch + corners.at(i2).first * bytes+2] = 255;
        }
    }
    for (int i = 0; i < roads->size(); i++) {
        //get the corners
        vector<pair<int,int>> corners = roads->at(i).getCorners();
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
    auto iterator = intersections->begin();
    for(int i = 0; i < (intersections->size()); i++) {
        cout<<"Intersection "<<i<<": ";
        iterator->print();
        advance(iterator, 1);
    }
    auto iterator2 = roads->begin();
    for(int i = 0; i < (roads->size()); i++) {
        cout<<"Road "<<i<<": ";
        iterator2->print();
        advance(iterator2, 1);
    }
}