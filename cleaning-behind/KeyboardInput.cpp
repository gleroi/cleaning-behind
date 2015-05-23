#include "KeyboardInput.h"

KeyboardInput::KeyboardInput()
{
}


KeyboardInput::~KeyboardInput()
{
}


void HandleKeyDown(SDL_Event& e, GameState& game)
{
    Player& PLAYER = game.PLAYER;

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


void HandleKeyUp(SDL_Event& e, GameState& game)
{
    Player& PLAYER = game.PLAYER;

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
        game.CleanUp();
        break;
    }
}

void KeyboardInput::HandleEvent(SDL_Event& e, GameState& game)
{
    if (e.type == SDL_KEYDOWN)
    {
        HandleKeyDown(e, game);
    }
    else if (e.type == SDL_KEYUP)
    {
        HandleKeyUp(e, game);
    }
}
