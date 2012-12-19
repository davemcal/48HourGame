#ifndef MAINMENUSTATE
#define MAINMENUSTATE

#include "GameState.h"

#include "SDL/Include/SDL.h"
#include "SDL/Include/SDL_opengl.h"

class MainMenuState : public GameState {

	int SCREEN_WIDTH, SCREEN_HEIGHT;
	int selected;
	GLuint play_gfx, credits_gfx, quit_gfx, bg_gfx;

	int music;

public:
	MainMenuState(int width, int height, int music_chan = -1);
	void display();
	GameState *update(Uint32 timeDif, bool * prevInput, bool * curInput);
	~MainMenuState();
};

#endif