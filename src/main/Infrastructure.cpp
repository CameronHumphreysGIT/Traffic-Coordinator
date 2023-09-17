#include <Infrastructure.h>
#include <list>
#include <iostream>


using namespace std;

Infrastructure::Infrastructure() {
    intersections = new vector<Intersection>();
}
void Infrastructure::addI(Intersection* i) {
    intersections->push_back(*i);
}

void Infrastructure::buildIntersections(SDL_Surface* gScreenSurface) {

    //surface must be locked for modification
    SDL_LockSurface(gScreenSurface);
    //The for loops require a const paramater for their bounds.
    const int height = gScreenSurface->h;
    const int width = gScreenSurface->w;
    //we need to modify the pixels, 
    uint8_t* pixelArray = (uint8_t*)gScreenSurface->pixels;
    //FIXME
    int b = pixelArray[63 * gScreenSurface->pitch + 130 * gScreenSurface->format->BytesPerPixel+0];
    int g = pixelArray[63 * gScreenSurface->pitch + 130 * gScreenSurface->format->BytesPerPixel+1];
    int r = pixelArray[63 * gScreenSurface->pitch + 130 * gScreenSurface->format->BytesPerPixel+2];
    cout<<b<< " ";
    cout<<g<< " ";
    cout<<r<< " ";

    cout<<"test";
    int pitch = gScreenSurface->pitch;
    int bytes = gScreenSurface->format->BytesPerPixel;
    //index of intersection that the Right corner belongs to
    int right = 0;
    vector<int> xValues;
    //TODO this algorithm is a first draft, maybe a more efficient solution
    for(int y = 0; y < 70; y++)  {
        for(int x = 0; x < 150; x++){
            if (isCorner(x, y, pixelArray, pitch, bytes) != NOT) {
                cout<<" is corner \n";
                //it is a corner
                if (isCorner(x, y, pixelArray, pitch, bytes) == TOPLEFT) {
                    //make an intersection and set it's top left corner
                    Intersection* intersection = new Intersection();
                    intersection->setCorner(TOPLEFT, x, y);
                    //Add to the infrastructure collection
                    addI(intersection);
                    right = (intersections->size() - 1);
                    //We store the left border of each intersection
                    xValues.push_back(x);
                }
                if (isCorner(x, y, pixelArray, pitch, bytes) == TOPRIGHT) {
                    //whatever was the last created intersection is the one the right corner is apart of.
                    intersections->at(right).setCorner(TOPRIGHT, x, y);
                }
                if (isCorner(x, y, pixelArray, pitch, bytes) == BOTTOMLEFT) {
                    //we look up the x borders to see which matches the current x value:
                    for (int i = 0; i < xValues.size(); i++) {
                        if (xValues.at(i) == x) {
                            intersections->at(i).setCorner(BOTTOMLEFT, x, y);
                            //reset the xvalue so as not to be confused with lower levels
                            xValues.at(i) = 0;
                            //index of the intersection we set the corner at
                            right = i;
                        }
                    }
                }
                if (isCorner(x, y, pixelArray, pitch, bytes) == BOTTOMRIGHT) {
                    //whatever was the last created intersection is the one the right corner is apart of.
                    intersections->at(right).setCorner(BOTTOMRIGHT, x, y);
                }
            }
            //cout<<x<<","<<y<<"\n";
        }
    }

    SDL_UnlockSurface(gScreenSurface);
}

int Infrastructure::isCorner(int x, int y, uint8_t *pixels, int pitch, int bytes) {
    //check if green
    if (isGreen(x, y, pixels, pitch, bytes)) {
        cout<<" it is green \n";
        //check if above is not green
        if (!isGreen(x, (y - 1), pixels, pitch, bytes)) {
            if (!isGreen((x - 1), y, pixels, pitch, bytes)) {
                //top left corner
                return TOPLEFT;
            }
            if (!isGreen((x + 1), y, pixels, pitch, bytes)) {
                //top left corner
                return TOPRIGHT;
            }
        }
        if (!isGreen(x, (y + 1), pixels, pitch, bytes)) {
            if (!isGreen((x - 1), y, pixels, pitch, bytes)) {
                //bottom left corner
                return BOTTOMLEFT;
            }
            if (!isGreen((x + 1), y, pixels, pitch, bytes)) {
                //bottom right corner
                return BOTTOMRIGHT;
            }
        }
    }
    return NOT;
}

bool Infrastructure::isGreen(int x, int y, uint8_t *pixels, int pitch, int bytes) {
    //check if green
    //though it is a little strange, I've imbedded the if statements to make it more readible instead of one extremely long line.
    if (pixels[y * pitch + x * bytes+0] == 76) {
        if (pixels[y * pitch + x * bytes+1] == 177) {
            if (pixels[y * pitch + x * bytes+2] == 34) {
                //is green.
                return true;
            }
        }
    }
    return false; 
}



void Infrastructure::print() {
    //iterator
    auto iterator = intersections->begin();
    for(int i = 0; i < (intersections->size()); i++) {
        cout<<"Intersection "<<i<<": ";
        iterator->print();
        advance(iterator, 1);
    }
}