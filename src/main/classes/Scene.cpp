#include <Scene.h>
#include <Variables.h>
#include <iostream>

using namespace std;

Scene::Scene(SDL_Renderer* &r) {
    renderer = r;
    boundary = new SDL_Rect();
    boundary->x = 0;
    boundary->y = 0;
    boundary->w = Variables::SCREEN_WIDTH;
    boundary->h = Variables::SCREEN_HEIGHT;
}

Scene::~Scene() {
    delete boundary;
}

void Scene::drawBackground(SDL_Texture* background) {
    //this always needs to be done at the start, otherwise we will end up drawing over everything.
    SDL_RenderCopy(renderer, background, NULL, boundary);
}

void Scene::drawRoads(vector<vector<vector<pair<float, float>>>> intersections) {
    //drawstuff
    SDL_SetRenderDrawColor(renderer, 0,0,0, SDL_ALPHA_OPAQUE);
    //intersections are the set of the lines going into each intersection
    for (int i = 0; i < intersections.size(); i++) {
        //each intersection has 4 lines
        for (int i2 = 0; i2 < 4; i2++) {
            //each line is made up of points, some are many points since they're curved
            int size = intersections.at(i).at(i2).size();
            //straightline
            if (size == 2) {
                pair<float, float> p1 = intersections.at(i).at(i2).at(0);
                pair<float, float> p2 = intersections.at(i).at(i2).at(1);
                SDL_RenderDrawLine(renderer, p1.first, p1.second, p2.first, p2.second);
            }else {
                for (int i3 = 0; i3 < size - 1; i3++) {
                    //draw each line
                    pair<float, float> p1 = intersections.at(i).at(i2).at(i3);
                    pair<float, float> p2 = intersections.at(i).at(i2).at(i3 + 1);
                    SDL_RenderDrawLine(renderer, p1.first, p1.second, p2.first, p2.second);
                }
            }
        }
    }
}

void Scene::drawCars(vector<SDL_Rect*> rectangles, SDL_Texture* carTexture, vector<float*> rotations) {
    //Now draw Cars:
    for (int i = 0; i < rectangles.size(); i++) {
        //SDL_RenderCopy(renderer, carTexture, NULL, rectangles.at(i));
        //need to use renderCopyEx to rotate the car.
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        SDL_RenderCopyEx(renderer, carTexture, NULL, rectangles.at(i), (*rotations.at(i)), NULL, flip);
    }
}

void Scene::present() {
    //present to the window
    SDL_RenderPresent(renderer);
}