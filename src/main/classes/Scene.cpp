#include <Scene.h>
#include <Variables.h>

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

void Scene::draw(std::vector<SDL_Rect*> rectangles, std::vector<std::pair<float, float>> lines) {
    //this always needs to be done at the start, otherwise we will end up drawing over everything.
    SDL_RenderCopy(renderer, backgroundTexture, NULL, boundary);
    
    //drawstuff
    SDL_SetRenderDrawColor(renderer, 200,120,200, SDL_ALPHA_OPAQUE);
    //lines are sampled from curves, in one big vector of points
    for (int i = 0; i < lines.size() - 1; i++) {
        //draw each line
        SDL_RenderDrawLine(renderer, lines[i].first, lines[i].second, lines[i+1].first, lines[i+1].second);
    }
    //Now draw rectangles:
    for (int i = 0; i < rectangles.size(); i++) {
        SDL_RenderDrawRect(renderer, rectangles.at(i));
    }

    //present to the window
    SDL_RenderPresent(renderer);
}