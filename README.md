# Traffic simulation in downtown Ottawa.

## A work in progress project that simulates traffic across 172 intersections, built with multiple scenarios for testing.

This project uses SDL, SDL_Image, and SDL_TTF to display a GUI that represents the car's positions and paths on the map.
The map is generated using a Map.svg map file that has all of the intersections on the satallite map, highlighted with green squares.
Cars are routed using the AStar algorithm, and using the GUI, car accidents can be indunced and are handled.
The roads and paths within each intersection are expressed as Bezier Curves, which are sampled to create a vector of waypoints for the cars to follow.
The project is built using CMake, and the testing is done using Catch2. The properties of the program can be tweaked in Variables.h.


![Alt text](\libs\images\legacy\Traffic Coordination Cover.png?raw=true "Title")


