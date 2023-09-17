#ifndef GLOBALS_H
#define GLOBALS_H
//FIXME Figure out how to do globals like this
class Globals {
    public:
        enum Corner { TOPLEFT = 1, TOPRIGHT = 2, BOTTOMLEFT = 3, BOTTOMRIGHT = 4};
        int SCREEN_WIDTH = 1000;
        int SCREEN_HEIGHT = 565;
        const char* BACKGROUND_PATH = "./images/Background.jpg";
};

#endif