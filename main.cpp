#include "SDL/Include/SDL.h"
#include "SDL/Include/SDL_opengl.h"
#include "SDL/Include/SDL_mixer.h"

#include "MainMenuState.h"
#include "Config.h"

#include <ctime>
#include <iostream>

using namespace std;

int main(int argc, char **argv) {

	srand ( time(NULL) );

	Config::load("config.txt");
	int SCREEN_WIDTH = atoi(Config::get("SCREEN_WIDTH").c_str());
	int SCREEN_HEIGHT = atoi(Config::get("SCREEN_HEIGHT").c_str());
	bool FULL_SCREEN = (Config::get("FULL_SCREEN") == "TRUE");
	string TITLE_NAME = Config::get("TITLE_NAME");

	//Start SDL
    if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        exit(1);
    }

	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_Surface *screen;
	if (FULL_SCREEN) {
		screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_OPENGL|SDL_FULLSCREEN );
	} else {
		screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_OPENGL);
	}

	int audio_rate = 22050;
	Uint16 audio_format = AUDIO_S16SYS;
	int audio_channels = 2;
	int audio_buffers = 4096;
	 
	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
		cout << "Unable to initialize audio: " << Mix_GetError() << endl;
		exit(1);
	}

	glEnable( GL_TEXTURE_2D );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GREATER, 0.9f);
 
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT );
	 
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	 
	glOrtho(0, 75*16, 60*16, 0, -1.0f, 1.0f);
		
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	SDL_WM_SetCaption( TITLE_NAME.c_str(), NULL );

	bool running = true;

	Uint32 lastTick;
	Uint32 curTick = SDL_GetTicks();

	bool prevInput[SDLK_LAST];
	bool curInput[SDLK_LAST];

	fill(curInput, curInput + SDLK_LAST, false);

	GameState *state = new MainMenuState(75*16, 60*16);

	while (running) {
		lastTick = curTick;
		curTick = SDL_GetTicks();

		copy(curInput, curInput + SDLK_LAST, prevInput);

		SDL_Event event;

		while( SDL_PollEvent( &event ) ) {
			
			if( event.type == SDL_QUIT )
			{
				running = false;
			}  
			//Key Releases
			if ( event.type == SDL_KEYUP) {
				curInput[event.key.keysym.sym] = false;
			}
			//Key Presses
			if ( event.type == SDL_KEYDOWN) {
				curInput[event.key.keysym.sym] = true;
			}
		}

		GameState *newstate = state->update(curTick - lastTick, prevInput, curInput);
		if (newstate != NULL) {
			delete state;
			state = newstate;
			fill(curInput, curInput + SDLK_LAST, false);
		}

		glClear( GL_COLOR_BUFFER_BIT );

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		state->display();
		
		SDL_GL_SwapBuffers();

		//Try to cap fps at 60
		SDL_Delay((1000/60) > (curTick - lastTick) ? 1000 / 60 - (curTick - lastTick) : 0);
	}

	delete state;

	Mix_Quit();
	SDL_Quit();

	return 0;
}