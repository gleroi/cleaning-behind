#pragma once

#include <array>

const int WINDOW_Y_MAX = 1080;
const int WINDOW_X_MAX = 1920;

const int MAP_X_MAX = 1500;
const int MAP_Y_MAX = 1500;

const int TILE_HEIGHT = 32;
const int TILE_WIDTH = 32;

const int TILE_Y_MAX = WINDOW_Y_MAX / TILE_HEIGHT + 1;
const int TILE_X_MAX = WINDOW_X_MAX / TILE_WIDTH;

const double SPEED = TILE_HEIGHT * 4;

struct Player {
    int x = TILE_X_MAX / 2;
    int y = TILE_Y_MAX / 2;

    double x_intile = TILE_WIDTH / 2;
    double y_intile = TILE_HEIGHT / 2;

    double speed_x = 0;
    double speed_y = 0;

};

enum TileType : int {
    GROUND = 0,
    STAIN = 1,
    WALL = 2,
};

class GameState
{
public:
    using Map = std::array<double, MAP_X_MAX * MAP_Y_MAX>;

    Player PLAYER;
    Map& TILES = *new Map();

    void InitGame();
    void CleanUp();

    GameState();
    ~GameState();
};
