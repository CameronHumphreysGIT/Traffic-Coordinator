#include <Scene.h>
#include <Variables.h>
#include <iostream>

Scene::Scene(SDL_Renderer* &r, SDL_Texture* t) {
    renderer = r;
    backgroundTexture = t;
    boundary = new SDL_Rect();
    boundary->x = 0;
    boundary->y = 0;
    boundary->w = Variables::SCREEN_WIDTH;
    boundary->h = Variables::SCREEN_HEIGHT;
}

Scene::~Scene() {
    delete boundary;
}

void Scene::draw(std::vector<SDL_Rect*> rectangles, std::vector<std::vector<std::vector<std::pair<float, float>>>> intersections, SDL_Texture* carTexture) {
    //this always needs to be done at the start, otherwise we will end up drawing over everything.
    SDL_RenderCopy(renderer, backgroundTexture, NULL, boundary);
    
    //drawstuff
    SDL_SetRenderDrawColor(renderer, 200,120,200, SDL_ALPHA_OPAQUE);
    //intersections are the set of the lines going into each intersection
    for (int i = 0; i < intersections.size(); i++) {
        //each intersection has 4 lines
        for (int i2 = 0; i2 < 4; i2++) {
            //each line is made up of points, some are many points since they're curved
            int size = intersections.at(i).at(i2).size();
            //straightline
            if (size == 2) {
                std::pair<float, float> p1 = intersections.at(i).at(i2).at(0);
                std::pair<float, float> p2 = intersections.at(i).at(i2).at(1);
                SDL_RenderDrawLine(renderer, p1.first, p1.second, p2.first, p2.second);
            }else {
                for (int i3 = 0; i3 < size - 1; i3++) {
                    //draw each line
                    std::pair<float, float> p1 = intersections.at(i).at(i2).at(i3);
                    std::pair<float, float> p2 = intersections.at(i).at(i2).at(i3 + 1);
                    SDL_RenderDrawLine(renderer, p1.first, p1.second, p2.first, p2.second);
                }
            }
        }
    }

    //Now draw Cars:
    for (int i = 0; i < rectangles.size(); i++) {
        SDL_RenderCopy(renderer, carTexture, NULL, rectangles.at(i));
    }

    //present to the window
    SDL_RenderPresent(renderer);
}