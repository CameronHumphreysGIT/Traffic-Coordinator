#include <System.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cassert>
#include <BezierPath.h>
#include <Router.h>

using namespace std;

System::System() {
    //The window we'll be rendering to
    window = NULL;
    //The image we will load and show on the screen
    screenSurface = NULL;
    //renderer
    renderer = NULL;
    //screentexture
    backgroundTexture = NULL;
    //texture for Cars
    carTexture = NULL;
    infrastructure = new Infrastructure();
    carHandler = new CarHandler();
}
System::~System() {
    delete infrastructure;
    delete scene;
    delete carHandler;
}

bool System::init()   {
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
        }else {
            //Get window surface
            screenSurface = SDL_GetWindowSurface( window );
        }
    }
    time = SDL_GetTicks();
    return success;
}

//loads media and sets the backgroundTexture
bool System::loadMedia(bool svgFlag)  {
    SDL_Surface* background;
    //used temporarily to read from a file
    SDL_RWops* io;
    //Loading success flag
    bool success = true;
    if (svgFlag) {
        //Load splash image
        io = SDL_RWFromFile(Variables::BACKGROUND_PATH, "r+");
        background = IMG_LoadSVG_RW(io);
        if( background == NULL )   {
            cout<<"Unable to load image"<<Variables::BACKGROUND_PATH<<"! SDL Error: "<<SDL_GetError();
            success = false;
        }
    }else {
        io = SDL_RWFromFile(Variables::MAP_PATH, "r");
        background = IMG_LoadPNG_RW(io);
        if( background == NULL )   {
            cout<<"Unable to load image"<<Variables::BACKGROUND_PATH<<"! SDL Error: "<<SDL_GetError();
            success = false;
        }
    }
    //blitting the surface sets the surface.
    SDL_BlitSurface( background, NULL, screenSurface, NULL );
    //we now turn that surface into a texture
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, screenSurface);
    //load in the Car texture:
    io = SDL_RWFromFile(Variables::CAR_PATH, "r");
    SDL_Surface* car = IMG_LoadPNG_RW(io);
    carTexture = SDL_CreateTextureFromSurface(renderer, car);
    //we can now create the scene:
    scene = new Scene(renderer, backgroundTexture);
    return success;
}

void System::buildInfrastructure() {
    infrastructure->buildInfrastructure(screenSurface);
    //this line is for debugging the infrastructure
    SDL_UpdateWindowSurface(window);
    infrastructure->print();
    //redraw background
    loadMedia(false);
    scene->setBackground(backgroundTexture);
}

void System::scenario(int scenario) {
    switch (scenario) {
        case 0:
            //This function exists to create a car and a path for the car.
            //update the time
            time = SDL_GetTicks();
            //Car wants time in seconds, the carHandler will make the car
            carHandler->addCar({0,0}, (time * 0.001f));
            Intersection* i1 = infrastructure->getI(0,0);
            Intersection* i2 = infrastructure->getI(0,1);
            Intersection* i3 = infrastructure->getI(0,2);
            Intersection* i4 = infrastructure->getI(1,0);
            Intersection* i5 = infrastructure->getI(1,1);
            vector<Intersection*>* vec = new vector<Intersection*>();
            vec->push_back(i1);
            vec->push_back(i2);
            vec->push_back(i3);
            vec->push_back(i4);
            vec->push_back(i5);
            assert(carHandler->setRoute(0, vec));
            break;
    }
}

void System::draw() {
    for (int i = 0; i < carHandler->size(); i++) {
        //update the cars
        time = SDL_GetTicks();
        carHandler->updateCar(i, (time * 0.001f));
    }
    vector<SDL_Rect*> rects = carHandler->getData().first;
    vector<float*> rotations = carHandler->getData().second;
    vector<vector<vector<pair<float, float>>>> sampled = infrastructure->getSampled();
    scene->draw(rects, sampled, carTexture, rotations);
}

void System::run() {
    SDL_Event e;
    bool quit = false;
    Uint32 startTime = time;
    while(quit == false)  {
        //check for quit
        while(SDL_PollEvent(&e) != 0)    {
           if(e.type == SDL_QUIT) quit = true;
        }
        time = SDL_GetTicks();
        if ((time - startTime) >= ((1.0f/Variables::FRAME_RATE) * 1000)) {
            draw();
            startTime = time;
        }
    }
}

void System::close()  {
    //Deallocate surface
    SDL_FreeSurface( screenSurface );
    screenSurface = NULL;

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