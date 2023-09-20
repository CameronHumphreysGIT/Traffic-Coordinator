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

void Infrastructure::buildInfrastructure(SDL_Surface* gScreenSurface) {

    //surface must be locked for modification
    SDL_LockSurface(gScreenSurface);
    //The for loops require a const paramater for their bounds.
    const int height = gScreenSurface->h;
    const int width = gScreenSurface->w;
    //we need to modify the pixels, 
    uint8_t* pixelArray = (uint8_t*)gScreenSurface->pixels;
    //FIXME
    int b = pixelArray[10 * gScreenSurface->pitch + 20 * gScreenSurface->format->BytesPerPixel+0];
    int g = pixelArray[10 * gScreenSurface->pitch + 20 * gScreenSurface->format->BytesPerPixel+1];
    int r = pixelArray[10 * gScreenSurface->pitch + 20 * gScreenSurface->format->BytesPerPixel+2];
    cout<<b<< " ";
    cout<<g<< " ";
    cout<<r<< " ";

    cout<<"test";
    int pitch = gScreenSurface->pitch;
    int bytes = gScreenSurface->format->BytesPerPixel;
    //index of intersection that the Right corner belongs to
    int rightI = 0;
    //index of right Road corner
    int rightR = 0;
    //stored left boundary of Intersections
    vector<int> IxValues;
    //stored left boundary of Roads
    vector<int> RxValues;
    //TODO this algorithm is a first draft, maybe a more efficient solution
    for(int y = 0; y < height; y++)  {
        //cout<<"loop";
        for(int x = 0; x < width; x++)  {
            int cornerReturn = isCorner(x, y, pixelArray, pitch, bytes);
            if (cornerReturn > 0) {
                //this means we found a Intersection
                if (cornerReturn == Variables::TOPLEFT) {
                    //make an intersection and set it's top left corner
                    Intersection* intersection = new Intersection();
                    intersection->setCorner(Variables::TOPLEFT, x, y);
                    //Add to the infrastructure collection
                    addI(intersection);
                    rightI = (intersections->size() - 1);
                    //We store the left border of each intersection
                    IxValues.push_back(x);
                }else {
                    checkAndSetInter(cornerReturn, &rightI, IxValues, x, y);
                }
            }
            if (cornerReturn < 0) {
                //make it positive
                cornerReturn = cornerReturn * -1;
                //we found a Road
                if (cornerReturn == Variables::TOPLEFT) {
                    //make an intersection and set it's top left corner
                    Road* road = new Road();
                    road->setCorner(Variables::TOPLEFT, x, y);
                    //Add to the infrastructure collection
                    addR(road);
                    rightR = (roads->size() - 1);
                    //We store the left border of each intersection
                    RxValues.push_back(x);
                }else {
                    checkAndSetRoad(cornerReturn, &rightR, RxValues, x, y);
                }
            }

            
        }
    }
    //finally, colour the corners:
    colourCorners(pixelArray, pitch, bytes);

    SDL_UnlockSurface(gScreenSurface);
}

void Infrastructure::checkAndSetInter(int cornerReturn, int *right, vector<int> &xValues, int x, int y) {
    if (cornerReturn != Variables::NOT) {
        //it is a corner
        if (cornerReturn == Variables::TOPRIGHT) {
            //whatever was the last created intersection is the one the right corner is apart of.
            intersections->at(*right).setCorner(Variables::TOPRIGHT, x, y);
        }
        if (cornerReturn == Variables::BOTTOMLEFT) {
            //we look up the x borders to see which matches the current x value:
            for (int i = 0; i < xValues.size(); i++) {
                if (xValues.at(i) == x) {
                    intersections->at(i).setCorner(Variables::BOTTOMLEFT, x, y);
                    //reset the xvalue so as not to be confused with lower levels
                    xValues.at(i) = 0;
                    //index of the intersection we set the corner at
                    *right = i;
                }
            }
        }
        if (cornerReturn == Variables::BOTTOMRIGHT) {
            //whatever was the last created intersection is the one the right corner is apart of.
            intersections->at(*right).setCorner(Variables::BOTTOMRIGHT, x, y);
        }
    }
}

void Infrastructure::checkAndSetRoad(int cornerReturn, int *right, vector<int> &xValues, int x, int y) {
    if (cornerReturn != Variables::NOT) {
        //it is a corner
        if (cornerReturn == Variables::TOPRIGHT) {
            //whatever was the last created intersection is the one the right corner is apart of.
            roads->at(*right).setCorner(Variables::TOPRIGHT, x, y);
        }
        if (cornerReturn == Variables::BOTTOMLEFT) {
            //we look up the x borders to see which matches the current x value:
            for (int i = 0; i < xValues.size(); i++) {
                if (xValues.at(i) == x) {
                    roads->at(i).setCorner(Variables::BOTTOMLEFT, x, y);
                    //reset the xvalue so as not to be confused with lower levels
                    xValues.at(i) = 0;
                    //index of the intersection we set the corner at
                    *right = i;
                }
            }
        }
        if (cornerReturn == Variables::BOTTOMRIGHT) {
            //whatever was the last created intersection is the one the right corner is apart of.
            roads->at(*right).setCorner(Variables::BOTTOMRIGHT, x, y);
        }
    }
}
//TODO FIRST THING FIX ALL THIS BULLSHIT AND MAYBE TRY TO DO THIS THE RIGHT WAY... also this function makes the compiler nervous.

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
    int returnVal;
    //check if above is not green
    if (!isColour(x, (y - 1), pixels, pitch, bytes, road)) {
        if (!isColour((x - 1), y, pixels, pitch, bytes, road)) {
            //top left corner
            returnVal = Variables::TOPLEFT;
        }
        if (!isColour((x + 1), y, pixels, pitch, bytes, road)) {
            //top left corner
            returnVal = Variables::TOPRIGHT;
        }
    }
    if (!isColour(x, (y + 1), pixels, pitch, bytes, road)) {
        if (!isColour((x - 1), y, pixels, pitch, bytes, road)) {
            //bottom left corner
            returnVal = Variables::BOTTOMLEFT;
        }
        if (!isColour((x + 1), y, pixels, pitch, bytes, road)) {
            //bottom right corner
            returnVal = Variables::BOTTOMRIGHT;
        }
    }
    return returnVal * -(int)road;
}

bool Infrastructure::isColour(int x, int y, uint8_t *pixels, int pitch, int bytes, bool road) {
    uint8_t b, g, r;
    if (road) {
        //look for road colours
        b = Variables::ROADCOLOUR.at(0);
        g = Variables::ROADCOLOUR.at(1);
        r = Variables::ROADCOLOUR.at(2);
    }else {
        b = Variables::INTERSECTIONCOLOUR.at(0);
        g = Variables::INTERSECTIONCOLOUR.at(1);
        r = Variables::INTERSECTIONCOLOUR.at(2);
    }
    //though it is a little strange, I've imbedded the if statements to make it more readible instead of one extremely long line.
    if (pixels[y * pitch + x * bytes+0] == b) {
        if (pixels[y * pitch + x * bytes+1] == g) {
            if (pixels[y * pitch + x * bytes+2] == r) {
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
        advance(iterator, 1);
    }
}