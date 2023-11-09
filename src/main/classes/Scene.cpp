#include <Scene.h>
#include <Variables.h>
#include <iostream>

using namespace std;

Scene::Scene(SDL_Renderer* &r, TTF_Font* & f) {
    renderer = r;
    boundary = new SDL_Rect();
    boundary->x = 0;
    boundary->y = 0;
    boundary->w = Variables::SCREEN_WIDTH;
    boundary->h = Variables::SCREEN_HEIGHT;
    font = f;
}

Scene::~Scene() {
    delete boundary;
}

void Scene::clear() {
    SDL_SetRenderDrawColor(renderer, 0,0,0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
}

void Scene::drawBackground(SDL_Texture* background) {
    //this always needs to be done at the start, otherwise we will end up drawing over everything.
    SDL_RenderCopy(renderer, background, NULL, boundary);
}

void Scene::drawRoads(vector<vector<vector<pair<float, float>>>> intersections) {
    //drawstuff
    SDL_SetRenderDrawColor(renderer, 255,0,0, SDL_ALPHA_OPAQUE);
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

void Scene::drawButton(SDL_Rect* borders, vector<int> colour, const char * words) {
    SDL_SetRenderDrawColor(renderer, colour.at(0), colour.at(1), colour.at(2), SDL_ALPHA_OPAQUE);
    //create a text surface
    SDL_Surface* text;
    // Set color to black
    SDL_Color color = { 0, 0, 0 };
    text = TTF_RenderText_Solid( font, words, color );
    if ( !text ) {
        cout << "Failed to render text: " << TTF_GetError() << endl;
    }
    //convert the surface to a texture:
    SDL_Texture* text_texture;
    text_texture = SDL_CreateTextureFromSurface( renderer, text );
    //draw the button
    SDL_RenderFillRect(renderer, borders);
    //draw a border
    SDL_SetRenderDrawColor(renderer, (colour.at(0) + 40), (colour.at(1) + 40), (colour.at(2) + 40), SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(renderer, borders);
    //copy over the text
    SDL_RenderCopy(renderer, text_texture, NULL, borders);
}

void Scene::present() {
    //present to the window
    SDL_RenderPresent(renderer);
}