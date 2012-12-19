#ifndef INSTRUCTIONSSTATE
#define INSTRUCTIONSSTATE

#include "GameState.h"

#include "SDL/Include/SDL.h"
#include "SDL/Include/SDL_opengl.h"

#include <string>

class InstructionsState : public GameState {

	int SCREEN_WIDTH, SCREEN_HEIGHT;
	GLuint instructions_gfx;

	int music;
	std::string map_name;

public:
	InstructionsState(int width, int height, std::string m, int music_chan = -1);
	void display();
	GameState *update(Uint32 timeDif, bool * prevInput, bool * curInput);
	~InstructionsState();
};

#endif