#include <iostream>
#include <SDL.h>
#include <vector>
#include <random>
#include <chrono>

const int WINDOW_Y_MAX = 480;
const int WINDOW_X_MAX = 640;

const int MAP_X_MAX = 1500;
const int MAP_Y_MAX = 1500;

const int TILE_Y_MAX = WINDOW_Y_MAX / 32;
const int TILE_X_MAX = WINDOW_X_MAX / 32;

const int tile_h = WINDOW_Y_MAX / TILE_Y_MAX;
const int tile_w = WINDOW_X_MAX / TILE_X_MAX;

enum TileType : int {
    GROUND = 0,
    STAIN = 1,
};

std::vector<double>& TILES = *(new std::vector<double>(MAP_X_MAX * MAP_Y_MAX));

const double SPEED = 50;

struct Player {
    int x = TILE_X_MAX / 2;
    int y = TILE_Y_MAX / 2;

    double x_intile = tile_w / 2;
    double y_intile = tile_h / 2;

    double speed_x;
    double speed_y;

};

Player PLAYER;

void InitGame() {
    for (int y = 0; y < MAP_Y_MAX; y++) {
        for (int x = 0; x < MAP_X_MAX; x++) {
            int value = rand() / (RAND_MAX * 1.0f) * 100.f;

            TileType tile = value > 80 ? TileType::STAIN : TileType::GROUND;
            TILES[y * MAP_Y_MAX + x] = tile;
        }
    }
}

void Render(SDL_Renderer& ren)
{
    SDL_Rect rect;
    rect.h = tile_h;
    rect.w = tile_w;

    int left = (PLAYER.x / TILE_X_MAX) * TILE_X_MAX;
    int top = (PLAYER.y / TILE_Y_MAX) * TILE_Y_MAX;

    std::cout << "top,left: " << top << ", " << left << std::endl;
    std::cout << "player: " << PLAYER.x << ", " << PLAYER.y << std::endl;

    for (int y = top; y < top + TILE_Y_MAX; y++) {
        for (int x = left; x < left + TILE_X_MAX; x++) {
            rect.x = (x - left) * tile_w;
            rect.y = (y - top) * tile_h;
            SDL_SetRenderDrawColor(&ren, TILES[y * MAP_Y_MAX + x] * 255, 0, 0, 0);
            SDL_RenderFillRect(&ren, &rect);
        }
    }

    rect.x = (PLAYER.x - left) * tile_w;
    rect.y = (PLAYER.y - top) * tile_h;
    SDL_SetRenderDrawColor(&ren, 255, 255, 0, 0);
    SDL_RenderDrawRect(&ren, &rect);

    int px = (PLAYER.x - left) * tile_w + PLAYER.x_intile;
    int py = (PLAYER.y - top) * tile_h + PLAYER.y_intile;
    rect.x = px - (tile_w / 2);
    rect.y = py - (tile_h / 2);

    SDL_SetRenderDrawColor(&ren, 0, 255, 255, 0);
    SDL_RenderFillRect(&ren, &rect);
}

int cl = 0;

void CleanUp() {
    double dirty = TILES[PLAYER.y * MAP_Y_MAX + PLAYER.x];
    TILES[PLAYER.y * MAP_Y_MAX + PLAYER.x] = fmax(0, dirty - 0.25);
}

void HandleKeyDown(SDL_Event& e)
{
    switch (e.key.keysym.sym)
    {
    case SDLK_LEFT:
        PLAYER.speed_x = -SPEED;
        break;
    case SDLK_RIGHT:
        PLAYER.speed_x = SPEED;
        break;
    case SDLK_UP:
        PLAYER.speed_y = -SPEED;
        break;
    case SDLK_DOWN:
        PLAYER.speed_y = SPEED;
        break;
    }
}


void HandleKeyUp(SDL_Event& e)
{
    switch (e.key.keysym.sym)
    {
    case SDLK_LEFT:
    case SDLK_RIGHT:
        PLAYER.speed_x = 0;
        break;
    case SDLK_UP:
    case SDLK_DOWN:
        PLAYER.speed_y = 0;
        break;
    case SDLK_SPACE:
        CleanUp();
        break;
    }
}

void UpdateGame(double dt) {
    double x_pix = PLAYER.x * tile_w + PLAYER.x_intile;
    double y_pix = PLAYER.y * tile_h + PLAYER.y_intile;

    double dx = PLAYER.speed_x * dt;
    double dy = PLAYER.speed_y * dt;

    x_pix = x_pix + dx;
    y_pix = y_pix + dy;

    PLAYER.x = x_pix / tile_w;
    PLAYER.x_intile = fmod(x_pix, tile_w);

    PLAYER.y = y_pix / tile_h;
    PLAYER.y_intile = fmod(y_pix, tile_h);

    //std::cout << "delta t: " << dt << std::endl;
    //std::cout << "speed: " << PLAYER.speed_x << ", " << PLAYER.speed_y << std::endl;
    //std::cout << "delta pos: " << dx << ", " << dy << std::endl;
    //std::cout << "position: " << x_pix << ", " << y_pix << std::endl;
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

    using std::chrono::steady_clock;
    steady_clock::time_point last = steady_clock::now();

    //Handle events on queue
    while (!quit)
    {
        if (SDL_PollEvent(&e) != 0) 
        {
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
            {
                quit = true;
            }

            if (e.type == SDL_KEYDOWN) 
            {
                HandleKeyDown(e);
            }
            else if (e.type == SDL_KEYUP) 
            {
                HandleKeyUp(e);
            }
        }

        steady_clock::time_point now = steady_clock::now();
        steady_clock::duration delta = now - last;
        last = now;
        UpdateGame(delta.count() * 1.f / steady_clock::duration::period::den);

        //First clear the renderer
        SDL_RenderClear(ren);

        Render(*ren);

        SDL_RenderPresent(ren);
    }
}
