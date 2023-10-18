#ifndef VARIABLES_H
#define VARIABLES_H

#include <vector>

class Variables {
    public:
        //corner enum is used to identify the corners of a rectangle with a word
        enum Corner { TOPLEFT = 1, TOPRIGHT = 2, BOTTOMLEFT = 3, BOTTOMRIGHT = 4, NOT = 0};
        static inline int SCREEN_WIDTH = 1920;
        static inline int SCREEN_HEIGHT = 1080;
        static inline int BACKGROUND_WIDTH = 1420;
        static inline int BACKGROUND_HEIGHT = 1033;
        //background is the satellite image used for the map
        static inline const char* BACKGROUND_PATH = "./images/Map.svg";
        static inline std::vector<int> INTERSECTIONCOLOUR = {0,255,0};
        static inline std::vector<int> ROADCOLOUR = {0,0,255};
        //default speed of carse in pixels/second
        static inline float DEFAULT_SPEED = 50.0f;
        //defining the square width and height of intersections.
        static inline int INTERSECTION_DIMS = 12;
        //maximum distance in x value between an intersection and the intersection below it.
        static inline int MAX_XDIST = 20;
        //how many samples in a curved path
        static inline int CURVED_SAMPLES = 10;
        //default car width
        static inline int CAR_WIDTH = 4;
        //default car height
        static inline int CAR_HEIGHT = 6;
        //path for image for car texture
        static inline const char* CAR_PATH = "./images/Car.png";
        //tolerance for how far from a waypoint before we go to the next one.
        static inline int WAYPOINT_TOL = 4;
};

#endif