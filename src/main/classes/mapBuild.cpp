#include <iostream>
#include <MapBuild.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <Variables.h>

using namespace std;


mapBuild::mapBuild()    {
    //The window we'll be rendering to
    window = NULL;
    //TODO remove this shit//The surface contained by the window
    screenSurface = NULL;
    //The image we will load and show on the screen
    background = NULL;
    //renderer
    renderer = NULL;
    //screentexture
    screenTexture = NULL;
    //background rectangle for background
    boundary = new SDL_Rect();
    boundary->x = 0;
    boundary->y = 0;
    boundary->w = Variables::BACKGROUND_WIDTH;
    boundary->h = Variables::BACKGROUND_HEIGHT;
}

mapBuild::~mapBuild() {
    delete boundary;
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
        window = SDL_CreateWindow( "Traffic Controller", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Variables::SCREEN_WIDTH, Variables::SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        //make renderer
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if( window == NULL )   {
            cout<<"Window could not be created! SDL_Error: "<<SDL_GetError;
            success = false;
        }else if(renderer == NULL) {
            cout<<"Renderer could not be created! SDL_Error: "<<SDL_GetError;
            success = false;
        }else    {
            //Get window surface
            screenSurface = SDL_GetWindowSurface( window );
        }
    }
    return success;
}

bool mapBuild::loadMedia()  {
    //Loading success flag
    bool success = true;

    //FIXME remove IMG_Load, put in a seperate fnction for after the roads and intersections are built.
    //Load splash image
    SDL_RWops* io = SDL_RWFromFile(Variables::BACKGROUND_PATH, "r+");
    background = IMG_LoadSVG_RW(io);//IMG_Load(Variables::BACKGROUND_PATH);
    if( background == NULL )   {
        cout<<"Unable to load image"<<Variables::BACKGROUND_PATH<<"! SDL Error: "<<SDL_GetError();
        success = false;
    }
    //blitting the surface sets the surface.
    SDL_BlitSurface( background, NULL, screenSurface, NULL );
    //we now turn that surface into a texture
    screenTexture = SDL_CreateTextureFromSurface(renderer, screenSurface);

    return success;
}


void mapBuild::close()  {
    //Deallocate surface
    SDL_FreeSurface( background );
    background = NULL;

    //Destroy window
    SDL_DestroyWindow( window );
    window = NULL;

    //destroy renderer
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    //quit IMG
    IMG_Quit();
    //Quit SDL subsystems
    SDL_Quit();
}


void mapBuild::buildIntersections(Infrastructure* inf) {
    inf->buildInfrastructure(screenSurface);
    //this line is for debugging the infrastructure
    SDL_UpdateWindowSurface(window);
}

void mapBuild::testRectangle(SDL_Rect* rect) {
    //this always needs to be done at the start, otherwise we will end up drawing over everything.
    renderBackground();
    
    //drawstuff
    SDL_SetRenderDrawColor(renderer, 200,120,200, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, rect);
    
    //present to the window
    SDL_RenderPresent(renderer);
}

void mapBuild::renderBackground() {
    SDL_RenderCopy(renderer, screenTexture, NULL, boundary);
}