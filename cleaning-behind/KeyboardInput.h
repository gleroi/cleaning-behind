#pragma once

#include <SDL_events.h>
#include "GameState.h"

class KeyboardInput
{

public:
    KeyboardInput();

    void HandleEvent(SDL_Event& e, GameState& game);

    ~KeyboardInput();
};

