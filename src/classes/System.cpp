#include <System.h>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <cassert>
#include <BezierPath.h>
#include <Router.h>
#include <SDL_ttf.h>

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
}
System::~System() {
    delete infrastructure;
    delete scene;
    delete carHandler;
    delete toggleBackground;
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
    //using an iff statement instead of switch to avoid redefinition issues
    if (scenario == 0) {
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
        vector<Intersection*> vec = {i1,i2,i3,i4,i5};
        //vec->push_back(i1);
        //vec->push_back(i2);
        //vec->push_back(i3);
        //vec->push_back(i4);
        //vec->push_back(i5);
        assert(carHandler->setRoute(0, &vec));
    }
    if (scenario == 1) {
        //This function exists to create a car and a path for the car.
        //update the time
        time = SDL_GetTicks();
        //Car wants time in seconds, the carHandler will make the car
        carHandler->addCar({319,369}, (time * 0.001f));//orig(9,1) //dest (8,2)
        Intersection* i1 = infrastructure->getI(9,1);
        Intersection* i2 = infrastructure->getI(9,0);
        Intersection* i3 = infrastructure->getI(8,5);
        Intersection* i4 = infrastructure->getI(8,4);
        Intersection* i5 = infrastructure->getI(8,3);
        Intersection* i6 = infrastructure->getI(8,2);
        vector<Intersection*> vec = {i1,i2,i3,i4,i5,i6};
        assert(carHandler->setRoute(0, &vec));
        carHandler->addCar({144,579}, (time * 0.001f));//orig(16,0) //dest (3,0)
        i1 = infrastructure->getI(16,0);
        i2 = infrastructure->getI(14,1);
        i3 = infrastructure->getI(11,4);
        i4 = infrastructure->getI(8,3);
        i5 = infrastructure->getI(8,4);
        i6 = infrastructure->getI(5,4);
        Intersection* i7 = infrastructure->getI(5,5);
        Intersection* i8 = infrastructure->getI(4,0);
        Intersection* i9 = infrastructure->getI(3,0);
        vec = {i1,i2,i3,i4,i5,i6, i7, i8, i9};
        assert(carHandler->setRoute(1, &vec));
        carHandler->addCar({130,351}, (time * 0.001f));//orig(8,5) //dest (11,13)
        i1 = infrastructure->getI(8,5);
        i2 = infrastructure->getI(9,0);
        i3 = infrastructure->getI(9,1);
        i4 = infrastructure->getI(9,2);
        i5 = infrastructure->getI(9,3);
        i6 = infrastructure->getI(9,4);
        i7 = infrastructure->getI(9,5);
        i8 = infrastructure->getI(9,6);
        i9 = infrastructure->getI(9,7);
        Intersection* i10 = infrastructure->getI(10,7);
        Intersection* i11 = infrastructure->getI(11,13);
        vec = {i1,i2,i3,i4,i5,i6, i7, i8, i9, i10, i11};
        assert(carHandler->setRoute(2, &vec));
        carHandler->addCar({273,215}, (time * 0.001f));//orig(4,0) //dest (16,0)
        i1 = infrastructure->getI(4,0);
        i2 = infrastructure->getI(5,5);
        i3 = infrastructure->getI(6,0);
        i4 = infrastructure->getI(7,0);
        i5 = infrastructure->getI(8,5);
        i6 = infrastructure->getI(9,0);
        i7 = infrastructure->getI(10,0);
        i8 = infrastructure->getI(11,6);
        i9 = infrastructure->getI(12,3);
        i10 = infrastructure->getI(13,0);
        i11 = infrastructure->getI(14,3);
        Intersection* i12 = infrastructure->getI(14,2);
        Intersection* i13 = infrastructure->getI(14,1);
        Intersection* i14 = infrastructure->getI(16,0);
        vec = {i1,i2,i3,i4,i5,i6, i7, i8, i9, i10, i11, i12, i13, i14};
        assert(carHandler->setRoute(3, &vec));
        carHandler->addCar({717,453}, (time * 0.001f));//orig(11,12) //dest (8,1)
        i1 = infrastructure->getI(11,12);
        i2 = infrastructure->getI(11,11);
        i3 = infrastructure->getI(11,10);
        i4 = infrastructure->getI(11,9);
        i5 = infrastructure->getI(11,8);
        i6 = infrastructure->getI(11,7);
        i7 = infrastructure->getI(11,6);
        i8 = infrastructure->getI(10,0);
        i9 = infrastructure->getI(9,0);
        i10 = infrastructure->getI(8,5);
        i11 = infrastructure->getI(8,4);
        i12 = infrastructure->getI(8,3);
        i13 = infrastructure->getI(8,2);
        i14 = infrastructure->getI(8,1);
        vec = {i1,i2,i3,i4,i5,i6, i7, i8, i9, i10, i11, i12, i13, i14};
        assert(carHandler->setRoute(4, &vec));
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
    scene->clear();
    scene->drawBackground(backgroundTexture);
    scene->drawRoads(sampled);
    scene->drawCars(rects, carTexture, rotations);
    scene->drawButton(toggleBackground->getBorders(), toggleBackground->getColour(), toggleBackground->getText());
    scene->present();
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