#ifndef VARIABLES_H
#define VARIABLES_H

#include <vector>
//FIXME Figure out how to do globals like this
class Variables {
    public:
        //corner enum is used to identify the corners of a rectangle with a word
        enum Corner { TOPLEFT = 1, TOPRIGHT = 2, BOTTOMLEFT = 3, BOTTOMRIGHT = 4, NOT = 0};
        static inline int SCREEN_WIDTH = 1920;
        static inline int SCREEN_HEIGHT = 1080;
        //background is the satellite image used for the map
        static inline const char* BACKGROUND_PATH = "./images/Background2.svg";
        static inline std::vector<int> INTERSECTIONCOLOUR = {0,255,0};
        static inline std::vector<int> ROADCOLOUR = {0,0,255};
};

#endif