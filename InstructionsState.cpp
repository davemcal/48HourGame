#include "InstructionsState.h"
#include "StageSelectState.h"
#include "MainMenuState.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "LevelState.h"

#include "SDL/Include/SDL.h"
#include "SDL/Include/SDL_mixer.h"

#include <string>

using namespace std;

InstructionsState::InstructionsState(int width, int height, string m, int music_chan) : SCREEN_WIDTH(width), SCREEN_HEIGHT(height), map_name(m) {
	instructions_gfx = TextureManager::load("Instructions.png");

	if (music_chan == -1) {
		Mix_PlayMusic(Mix_LoadMUS("menu_music.ogg"), -1);
	} else {
		music = music_chan;
	}
}

void InstructionsState::display() {
	glScalef(float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 1.0f);

	glBindTexture( GL_TEXTURE_2D, instructions_gfx );
	glBegin( GL_QUADS);
		glTexCoord2f( 0.0f, 0.0f );
		glVertex3f( 0.0f, 0.0f, 0.0f );
		glTexCoord2f( 1.0f, 0.0f );
		glVertex3f( 1.0f, 0.0f, 0.0f );
		glTexCoord2f( 1.0f, 1.0f );
		glVertex3f( 1.0f, 1.0f, 0.0f );
		glTexCoord2f( 0.0f, 1.0f );
		glVertex3f( 0.0f, 1.0f, 0.0f );
	glEnd();
}

GameState *InstructionsState::update(Uint32 timeDif, bool * prevInput, bool * curInput) {

	if (curInput[SDLK_RETURN]) {
		Mix_HaltMusic();
		return new LevelState(map_name, SCREEN_WIDTH, SCREEN_HEIGHT);
	}
	if (curInput[SDLK_ESCAPE]) 
			return new StageSelectState(SCREEN_WIDTH, SCREEN_HEIGHT, music);

	return NULL;
}

InstructionsState::~InstructionsState() {

}