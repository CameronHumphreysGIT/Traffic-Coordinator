#include <System.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <BezierPath.h>

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
    cars = new vector<Car*>;
}
System::~System() {
    delete infrastructure;
    delete scene;
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
    //update the time
    time = SDL_GetTicks();
    //TODO temp, move the cars?
    //Car wants time in seconds
    cars->push_back(new Car({0,0}, (time * 0.001f)));
    cars->at(0)->addPath(infrastructure->getSampled().at(0).at(1));
    //redraw background
    loadMedia(false);
    scene->setBackground(backgroundTexture);
}

void System::testdraw() {
    vector<SDL_Rect*> rects;
    for (int i = 0; i < cars->size(); i++) {
        //update the cars
        time = SDL_GetTicks();
        cars->at(i)->update(time * 0.001f);
        rects.push_back(cars->at(i)->getChassis());
    }
    vector<vector<vector<pair<float, float>>>> sampled = infrastructure->getSampled();
    scene->draw(rects, sampled, carTexture);
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
        if ((time - startTime) >= (17)) {
            testdraw();
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