#include <iostream>
#include <MapBuild.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <Globals.h>
#include <Intersection.h>

using namespace std;

int SCREEN_WIDTH = 1000;
int SCREEN_HEIGHT = 565;
const char* BACKGROUND_PATH = "./images/Background.png";

mapBuild::mapBuild()    {
    //The window we'll be rendering to
    gWindow = NULL;
    //The surface contained by the window
    gScreenSurface = NULL;
    //The image we will load and show on the screen
    gBackground = NULL;
}

bool mapBuild::init()   {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 && IMG_Init(IMG_INIT_PNG) )    {
        cout<<"SDL could not initialize! SDL_Error:";
        success = false;
    }
    else    {
        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )   {
            cout<<"Window could not be created! SDL_Error: "<<SDL_GetError;
            success = false;
        }
        else    {
            //Get window surface
            gScreenSurface = SDL_GetWindowSurface( gWindow );
        }
    }

    return success;
}

bool mapBuild::loadMedia()  {
    //Loading success flag
    bool success = true;

    //Load splash image
    gBackground = IMG_Load(BACKGROUND_PATH);
    if( gBackground == NULL )   {
        cout<<"Unable to load image"<<BACKGROUND_PATH<<"! SDL Error: "<<SDL_GetError();
        success = false;
    }

    SDL_BlitSurface( gBackground, NULL, gScreenSurface, NULL );
    //Update the surface
    SDL_UpdateWindowSurface( gWindow );

    return success;
}


void mapBuild::close()  {
    //Deallocate surface
    SDL_FreeSurface( gBackground );
    gBackground = NULL;

    //Destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    //quit IMG
    IMG_Quit();
    //Quit SDL subsystems
    SDL_Quit();
}


void mapBuild::buildIntersections(Infrastructure* inf) {
    inf->buildIntersections(gScreenSurface);
}
