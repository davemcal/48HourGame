#ifndef GAMESTATE
#define GAMESTATE

#include "SDL/Include/SDL.h"

class GameState {

public:
	virtual void display() = 0;
	virtual GameState *update(Uint32 timeDif, bool * prevInput, bool * curInput) = 0;
	virtual ~GameState() {}
};

#endif