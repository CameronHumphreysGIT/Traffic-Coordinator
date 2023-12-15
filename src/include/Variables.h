#ifndef VARIABLES_H
#define VARIABLES_H

#include <vector>

class Variables {
    public:
        //corner enum is used to identify the corners of a rectangle with a word
        enum Corner { TOPLEFT = 1, TOPRIGHT = 2, BOTTOMLEFT = 3, BOTTOMRIGHT = 4, NOT = 0};
        static inline int SCREEN_WIDTH = 1200;
        static inline int SCREEN_HEIGHT = 700;
        static inline int BACKGROUND_WIDTH = 1420;
        static inline int BACKGROUND_HEIGHT = 1033;
        //background is the satellite image used for the map
        static inline const char* BACKGROUND_PATH = "./images/Map.svg";
        static inline const char* MAP_PATH = "./images/Map.png";
        static inline std::vector<int> INTERSECTIONCOLOUR = {0,255,0};
        //default speed of carse in pixels/second
        static inline float DEFAULT_SPEED = 25.0f;
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
        //tolerance for how far (pixels) from a waypoint before we go to the next one.
        static inline int WAYPOINT_TOL = 4;
        //the max frame rate of the program
        static inline int FRAME_RATE = 60;
        enum Side { TOP = 0, RIGHT = 1, BOTTOM = 2, LEFT = 3, END = 4};
        //Expression used for the Car to rotate
        static inline float RAD_TO_DEG = (180.0f/3.14159265f);
        //Colour for the buttons
        static inline std::vector<int> BUTTONCOLOUR = {135, 135, 135};
        //Hover colour for buttons
        static inline std::vector<int> HOVERCOLOUR = {150, 150, 150};
        //time in seconds that the traffic lights need to switch.
        static inline int LIGHTTIME = 3;
        //this is an approximate time for clearing an intersection, rounded up.
        static inline int CLEARTIME = (int)ceil(((sqrt(pow(INTERSECTION_DIMS, 2) + pow(INTERSECTION_DIMS, 2)))/DEFAULT_SPEED + 0.5));
        //The amount of time between spawning vehicles, set to a follow distance of 2 car lengths.
        static inline float SPAWNDIFF = (CAR_HEIGHT * 2)/DEFAULT_SPEED;
};

#endif