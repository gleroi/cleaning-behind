#include <iostream>
#include <SDL.h>
#include <random>
#include <chrono>
#include "KeyboardInput.h"
#include "GameState.h"

SDL_Texture* ground = nullptr;

void Render(SDL_Renderer& ren, GameState& game)
{
    Player& PLAYER = game.PLAYER;
    GameState::Map& TILES = game.TILES;

    SDL_Rect rect;
    rect.h = TILE_HEIGHT;
    rect.w = TILE_WIDTH;

    int left = (PLAYER.x / TILE_X_MAX) * TILE_X_MAX;
    int top = (PLAYER.y / TILE_Y_MAX) * TILE_Y_MAX;

    //std::cout << "top,left: " << top << ", " << left << std::endl;
    //std::cout << "player: " << PLAYER.x << ", " << PLAYER.y << std::endl;

    for (int y = top; y < top + TILE_Y_MAX; y++) {
        for (int x = left; x < left + TILE_X_MAX; x++) {
            double tile = TILES[y * MAP_Y_MAX + x];

            rect.x = (x - left) * TILE_WIDTH;
            rect.y = (y - top) * TILE_HEIGHT;
            if (tile <= 1)
            {
                SDL_SetRenderDrawColor(&ren, tile * 255, 0, 0, 0);
                SDL_RenderCopy(&ren, ground, nullptr, &rect);
            }
            else {
                SDL_SetRenderDrawColor(&ren, 255, 255, 255, 0);
                SDL_RenderFillRect(&ren, &rect);
            }
        }
    }

    rect.x = (PLAYER.x - left) * TILE_WIDTH;
    rect.y = (PLAYER.y - top) * TILE_HEIGHT;
    SDL_SetRenderDrawColor(&ren, 255, 255, 0, 0);
    SDL_RenderDrawRect(&ren, &rect);

    int px = (PLAYER.x - left) * TILE_WIDTH + PLAYER.x_intile;
    int py = (PLAYER.y - top) * TILE_HEIGHT + PLAYER.y_intile;
    rect.x = px - (TILE_WIDTH / 2);
    rect.y = py - (TILE_HEIGHT / 2);

    SDL_SetRenderDrawColor(&ren, 0, 255, 255, 0);
    SDL_RenderFillRect(&ren, &rect);

    SDL_SetRenderDrawColor(&ren, 0, 0, 0, 0);
    SDL_RenderDrawLine(&ren, rect.x, rect.y, rect.x + rect.w, rect.y + rect.h);
    SDL_RenderDrawLine(&ren, rect.x + rect.w, rect.y, rect.x, rect.y + rect.h);

}

void UpdateGame(double dt, GameState& game)
{
    Player& PLAYER = game.PLAYER;
    GameState::Map& TILES = game.TILES;

    double x_pix = PLAYER.x * TILE_WIDTH + PLAYER.x_intile;
    double y_pix = PLAYER.y * TILE_HEIGHT + PLAYER.y_intile;

    double dx = PLAYER.speed_x * dt;
    double dy = PLAYER.speed_y * dt;

    x_pix = x_pix + dx;
    y_pix = y_pix + dy;

    int new_x = x_pix / TILE_WIDTH;
    double new_x_intile = fmod(x_pix, TILE_WIDTH);

    int new_y = y_pix / TILE_HEIGHT;
    double new_y_intile = fmod(y_pix, TILE_HEIGHT);

    double tile = TILES[new_y * MAP_Y_MAX + new_x];
    if (tile <= 1) 
    {
        PLAYER.x = new_x;
        PLAYER.x_intile = new_x_intile;

        PLAYER.y = new_y;
        PLAYER.y_intile = new_y_intile;
    }

    //std::cout << "delta t: " << dt << std::endl;
    //std::cout << "speed: " << PLAYER.speed_x << ", " << PLAYER.speed_y << std::endl;
    //std::cout << "delta pos: " << dx << ", " << dy << std::endl;
    //std::cout << "position: " << new_x << ", " << new_y << " val: " << tile << std::endl;
}

int main(int, char**)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("Cleaning Behind", 100, 100, WINDOW_X_MAX, WINDOW_Y_MAX, SDL_WINDOW_SHOWN);
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

    SDL_Surface* img = SDL_LoadBMP("resources/carrelage.bmp");
    ground = SDL_CreateTextureFromSurface(ren, img);
    SDL_FreeSurface(img);
    
    GameState game;
    game.InitGame();

    bool quit = false;
    SDL_Event e;

    using std::chrono::steady_clock;
    steady_clock::time_point last = steady_clock::now();
    double elapsed_seconds = 0;
    int frame = 0;

    KeyboardInput keyboard;
    while (!quit)
    {
        if (SDL_PollEvent(&e) != 0) 
        {
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
            {
                quit = true;
            }
            keyboard.HandleEvent(e, game);
        }

        steady_clock::time_point now = steady_clock::now();
        steady_clock::duration delta = now - last;
        double dt_s = delta.count() * 1.f / steady_clock::duration::period::den;
        last = now;
        UpdateGame(dt_s, game);

        //First clear the renderer
        SDL_RenderClear(ren);

        Render(*ren, game);

        SDL_RenderPresent(ren);

        elapsed_seconds += dt_s;
        frame += 1;
        if (elapsed_seconds >= 5) {
            std::cout << "FPS: " << frame / elapsed_seconds << std::endl;
            frame = 0;
            elapsed_seconds = 0;
        }
    }
}
