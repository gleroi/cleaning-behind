#include "GameState.h"
#include <math.h>

GameState::GameState()
{
}

GameState::~GameState()
{
}

void GameState::InitGame() {
    srand(42);

    for (int y = 0; y < MAP_Y_MAX; y++) {
        for (int x = 0; x < MAP_X_MAX; x++) {
            int value = rand() / (RAND_MAX * 1.0f) * 100.f;

            int wall_chance = 75;
            {
                int left = fmin(fmax(0, x - 1), MAP_X_MAX - 1);
                int top = fmin(fmax(0, y), MAP_Y_MAX - 1);
                if (this->TILES[top * MAP_Y_MAX + left] > 1) {
                    wall_chance -= 10;
                }
            }
            {
                int left = fmin(fmax(0, x), MAP_X_MAX - 1);
                int top = fmin(fmax(0, y - 1), MAP_Y_MAX - 1);
                if (this->TILES[top * MAP_Y_MAX + left] > 1) {
                    wall_chance -= 10;
                }
            }

            TileType tile = TileType::GROUND;
            if (value > 95) {
                tile = TileType::STAIN;
            }
            else if (value > wall_chance) {
                tile = TileType::WALL;
            }
            this->TILES[y * MAP_Y_MAX + x] = tile;
        }
    }
}

void GameState::CleanUp() {
    double dirty = this->TILES[PLAYER.y * MAP_Y_MAX + PLAYER.x];
    this->TILES[PLAYER.y * MAP_Y_MAX + PLAYER.x] = fmax(0, dirty - 0.25);
}