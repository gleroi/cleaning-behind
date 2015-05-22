#include <iostream>
#include <SDL.h>
#include <vector>
#include <random>

const int WINDOW_Y_MAX = 640;
const int WINDOW_X_MAX = 800;

const int TILE_Y_MAX = 25;
const int TILE_X_MAX = TILE_Y_MAX * 1.33;

enum TileType : int {
    GROUND = 0,
    STAIN = 1,
};

std::vector<TileType>& TILES = *(new std::vector<TileType>(TILE_X_MAX * TILE_Y_MAX));

void InitGame() {
    for (int y = 0; y < TILE_Y_MAX; y++) {
        for (int x = 0; x < TILE_X_MAX; x++) {
            int value = rand() / (RAND_MAX * 1.0f) * 100.f;

            TileType tile = value > 80 ? TileType::STAIN : TileType::GROUND;
            TILES[y * TILE_Y_MAX + x] = tile;
        }
    }
}

void Render(SDL_Renderer& ren)
{
    int tile_h = WINDOW_Y_MAX / TILE_Y_MAX;
    int tile_w = WINDOW_X_MAX / TILE_X_MAX;

    SDL_Rect rect;
    rect.h = tile_h;
    rect.w = tile_w;

    for (int y = 0; y < TILE_Y_MAX; y++) {
        for (int x = 0; x < TILE_X_MAX; x++) {
            rect.x = x * tile_w;
            rect.y = y * tile_h;
            SDL_SetRenderDrawColor(&ren, TILES[y * TILE_Y_MAX + x] * 255, 0, 0, 0);
            SDL_RenderFillRect(&ren, &rect);
        }
    }
}

int main(int, char**)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    if (win == nullptr)
    {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr)
    {
        SDL_DestroyWindow(win);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    srand(5142);

    InitGame();

    bool quit = false;
    SDL_Event e;
    //Handle events on queue
    while (SDL_PollEvent(&e) != 0 || !quit)
    {
        //User requests quit
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
        {
            quit = true;
        }

        //First clear the renderer
        SDL_RenderClear(ren);

        Render(*ren);

        SDL_RenderPresent(ren);
    }
}

