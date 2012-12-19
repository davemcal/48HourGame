#ifndef CREDITSSTATE
#define CREDITSSTATE

#include "GameState.h"

#include "SDL/Include/SDL.h"
#include "SDL/Include/SDL_opengl.h"

class CreditsState : public GameState {

	int SCREEN_WIDTH, SCREEN_HEIGHT;
	GLuint credits_gfx;

	int music;

public:
	CreditsState(int width, int height, int music_chan = -1);
	void display();
	GameState *update(Uint32 timeDif, bool * prevInput, bool * curInput);
	~CreditsState();
};

#endif