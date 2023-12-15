#include <System.h>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <cassert>
#include <BezierPath.h>
#include <Router.h>
#include <SDL_ttf.h>
#include <stack>
#include <AStar.h>
#include <ScenarioBuilder.h>

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
    toggleBackground = new Button(970, 50, 100, 50);
    scenBuild = new ScenarioBuilder();
}
System::~System() {
    delete infrastructure;
    delete scene;
    delete carHandler;
    delete toggleBackground;
    delete scenBuild;
}

bool System::init()   {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 && IMG_Init(IMG_INIT_PNG) )    {
        cout<<"SDL could not initialize! SDL_Error:";
        success = false;
    }else    {
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
    //initialize ttf
    if (TTF_Init() < 0 ) {
	    cout << "Error initializing SDL_ttf: " << TTF_GetError() << endl;
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
    io = SDL_RWFromFile(Variables::MAP_PATH, "r");
    background = IMG_LoadPNG_RW(io);
    if( background == NULL )   {
        cout<<"Unable to load image"<<Variables::MAP_PATH<<"! SDL Error: "<<SDL_GetError();
        success = false;
    }
    //blitting the surface sets the surface.
    SDL_BlitSurface(background, NULL, screenSurface, NULL );
    //we now turn that surface into a texture
    satalliteBackground = SDL_CreateTextureFromSurface(renderer, screenSurface);

    io = SDL_RWFromFile(Variables::BACKGROUND_PATH, "r");
    background = IMG_LoadSVG_RW(io);
    if( background == NULL )   {
        cout<<"Unable to load image"<<Variables::BACKGROUND_PATH<<"! SDL Error: "<<SDL_GetError();
        success = false;
    }
    SDL_FillRect(screenSurface, NULL, 0x000000);
    //blitting the surface sets the surface.
    SDL_BlitSurface(background, NULL, screenSurface, NULL );
    //we now turn that surface into a texture
    intersectionsBackground = SDL_CreateTextureFromSurface(renderer, screenSurface);
    if (svgFlag) {
        backgroundTexture = intersectionsBackground;
    }else {
        backgroundTexture = satalliteBackground;
    }
    //load in the Car texture:
    io = SDL_RWFromFile(Variables::CAR_PATH, "r");
    SDL_Surface* car = IMG_LoadPNG_RW(io);
    carTexture = SDL_CreateTextureFromSurface(renderer, car);
    //font for the text
    font = TTF_OpenFont("fonts/Bold.ttf", 28);
    if ( !font ) {
        cout << "Failed to load font: " << TTF_GetError() << endl;
    }
    //we can now create the scene:
    scene = new Scene(renderer, font);
    return success;
}

void System::swapBackground() {
    if (backgroundTexture == intersectionsBackground) {
        backgroundTexture = satalliteBackground;
    }else if (backgroundTexture == satalliteBackground) {
        backgroundTexture = intersectionsBackground;
    }
}

void System::buildInfrastructure() {
    infrastructure->buildInfrastructure(screenSurface);
    //this line is for debugging the infrastructure
    SDL_UpdateWindowSurface(window);
    infrastructure->print();
    //redraw background
    swapBackground();
}

//This getter is used in testing
vector<vector<Intersection*>*>* System::getIntersections() {
    return infrastructure->getIntersections();
}
//This getter is used in testing
CarHandler* System::getCarHandler() {
    return carHandler;
}

void System::scenario(int scenario) {
    time = SDL_GetTicks();
    //build the scenario
    scenBuild->scenario(scenario, time, carHandler, infrastructure);
}

void System::draw() {
    time = SDL_GetTicks();
    //spawn more cars if necessary
    scenBuild->spawnMore((time * 0.001f), carHandler, infrastructure->getIntersections());
    for (int i = 0; i < carHandler->size(); i++) {
        time = SDL_GetTicks();
        //update the cars
        carHandler->updateCar(i, (time * 0.001f));  
    }
    vector<vector<vector<pair<float, float>>>> paths = (carHandler->getPaths());
    vector<SDL_Rect*> rects = carHandler->getData().first;
    vector<float*> rotations = carHandler->getData().second;
    vector<vector<vector<pair<float, float>>>> sampled = infrastructure->getSampled();
    scene->clear();
    scene->drawBackground(backgroundTexture);
    scene->drawRoads(sampled);
    vector<vector<vector<vector<pair<float, float>>>>> lights = infrastructure->getLights();
    scene->drawLights(lights);
    scene->drawCars(rects, carTexture, rotations);
    scene->drawPaths(paths);
    scene->drawButton(toggleBackground->getBorders(), toggleBackground->getColour(), toggleBackground->getText());
    scene->present();
    carHandler->detectCollisions();
}


void System::run() {
    SDL_Event e;
    bool quit = false;
    Uint32 startTime = time;
    while(quit == false)  {
        //check for quit
        while(SDL_PollEvent(&e) != 0)    {
            if(e.type == SDL_QUIT) {
                quit = true;
            }else {
                if(toggleBackground->isClicked(e)) {
                    swapBackground();
                };
            }    
        }
        time = SDL_GetTicks();
        if ((time - startTime) >= ((1.0f/Variables::FRAME_RATE) * 1000)) {
            draw();
            startTime = time;
        }
    }
}

//Run function used for testing takes in a timeout, also closes when scenario is done.
void System::run(int timeout) {
    //this means that we are being called from the test suite for a run 
    carHandler->setDestroy(false);
    SDL_Event e;
    bool quit = false;
    Uint32 startTime = time;
    Uint32 lastUpdate = time;
    while(!quit && (int)((time - startTime)/1000) < timeout)  {
        //check for quit
        while(SDL_PollEvent(&e) != 0)    {
            if(e.type == SDL_QUIT) {
                quit = true;
            }else {
                if(toggleBackground->isClicked(e)) {
                    swapBackground();
                };
            }
        }
        time = SDL_GetTicks();
        if ((time - lastUpdate) >= ((1.0f/Variables::FRAME_RATE) * 1000)) {
            draw();
            lastUpdate = time;
        }
        //once we have drawn the system, check that all cars haven't reached the end.
        if (!carHandler->isNotDone()) {
            quit = true;
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

    TTF_CloseFont( font );
    font = NULL;
    //quit IMG
    IMG_Quit();
    //quit ttf:
    TTF_Quit();
    //Quit SDL subsystems
    SDL_Quit();
}