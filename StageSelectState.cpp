#include "StageSelectState.h"
#include "MainMenuState.h"
#include "LevelState.h"
#include "InstructionsState.h"
#include "SoundManager.h"

#include "SDL/Include/SDL.h"
#include "SDL/Include/SDL_mixer.h"

#include <string>

using namespace std;

StageSelectState::StageSelectState(int width, int height, int music_chan) : SCREEN_WIDTH(width), SCREEN_HEIGHT(height) {

	selected = 0;
	
	stage_gfx = TextureManager::load("Map_select.png");

	if (music_chan == -1) {
		Mix_PlayMusic(Mix_LoadMUS("menu_music.ogg"), -1);
	} else {
		music = music_chan;
	}
}

void StageSelectState::display() {

	glPushMatrix();
	glScalef(float(SCREEN_WIDTH / 2), float(SCREEN_HEIGHT / 2), 1.0f);
	glDisable( GL_TEXTURE_2D );
	switch (selected) {
		case 1:
			glTranslatef(1.0f, 0.0f, 0.0f);
			break;
		case 2:
			glTranslatef(0.0f, 1.0f, 0.0f);
			break;
		case 3:
			glTranslatef(1.0f, 1.0f, 0.0f);
			break;
	}
	glColor3f(1.0f, 1.0f, 0.0f);
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
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable( GL_TEXTURE_2D );
	glPopMatrix();

	glScalef(float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 1.0f);
	glBindTexture( GL_TEXTURE_2D, stage_gfx );
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

GameState *StageSelectState::update(Uint32 timeDif, bool * prevInput, bool * curInput) {
	string stages[4];
	stages[0] = "Grass.txt";
	stages[1] = "clouds.txt";
	stages[2] = "Metal_map.txt";
	stages[3] = "Arena.txt";

	if (curInput[SDLK_ESCAPE]) return new MainMenuState(SCREEN_WIDTH, SCREEN_HEIGHT, music);

	if (curInput[SDLK_RETURN]) {
		return new InstructionsState(SCREEN_WIDTH, SCREEN_HEIGHT, stages[selected], music);
	}
	if (curInput[SDLK_UP] && !prevInput[SDLK_UP]) {
		if (selected < 2) selected += 2;
		else if (selected > 1) selected -= 2;
	}
	if (curInput[SDLK_DOWN] && !prevInput[SDLK_DOWN]) {
		if (selected < 2) selected += 2;
		else if (selected > 1) selected -= 2;
	}
	if (curInput[SDLK_LEFT] && !prevInput[SDLK_LEFT]) {
		switch (selected) {
			case 0:
				selected = 1;
				break;
			case 1:
				selected = 0;
				break;
			case 2:
				selected = 3;
				break;
			case 3:
				selected = 2;
				break;
		}
	}
	if (curInput[SDLK_RIGHT] && !prevInput[SDLK_RIGHT]) {
		switch (selected) {
			case 0:
				selected = 1;
				break;
			case 1:
				selected = 0;
				break;
			case 2:
				selected = 3;
				break;
			case 3:
				selected = 2;
				break;
		}
	}

	return NULL;
}

StageSelectState::~StageSelectState() {

}