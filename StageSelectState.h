#ifndef STAGESELECTSTATE
#define STAGESELECTSTATE

#include "GameState.h"

#include "SDL/Include/SDL.h"
#include "SDL/Include/SDL_opengl.h"

class StageSelectState : public GameState {

	int SCREEN_WIDTH, SCREEN_HEIGHT;
	int selected;
	GLuint stage_gfx;

	int music;

public:
	StageSelectState(int width, int height, int music_chan = -1);
	void display();
	GameState *update(Uint32 timeDif, bool * prevInput, bool * curInput);
	~StageSelectState();
};

#endif