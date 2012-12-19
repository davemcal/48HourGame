#include "MainMenuState.h"
#include "StageSelectState.h"
#include "CreditsState.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "SDL/Include/SDL_mixer.h"

#include "SDL/Include/SDL.h"

MainMenuState::MainMenuState(int width, int height, int music_chan) : SCREEN_WIDTH(width), SCREEN_HEIGHT(height) {

	selected = 0;
	
	play_gfx = TextureManager::load("play.png");
	credits_gfx = TextureManager::load("credits.png");
	quit_gfx = TextureManager::load("quit.png");
	bg_gfx = TextureManager::load("menuBG.png");

	if (music_chan == -1) {
		Mix_PlayMusic(Mix_LoadMUS("menu_music.ogg"), -1);
	} else {
		music = music_chan;
	}
}

void MainMenuState::display() {

	glPushMatrix();
	glScalef(float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 1.0f);
	glBindTexture( GL_TEXTURE_2D, bg_gfx );
	glColor3f(1.0f, 1.0f, 1.0f);
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
	glPopMatrix();

	glTranslatef(float(3 * SCREEN_WIDTH / 4), float(SCREEN_HEIGHT / 2), 0.0f);
	glScalef(200.0f, 50.0f, 1.0f);

	glBindTexture( GL_TEXTURE_2D, play_gfx );
	glColor3f(0.0f, 0.0f, 0.0f); if (selected == 0) glColor3f(1.0f, 0.0f, 0.0f);
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

	glTranslatef(0.0f, 2.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 0.0f); if (selected == 1) glColor3f(1.0f, 0.0f, 0.0f);
	glBindTexture( GL_TEXTURE_2D, credits_gfx );
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

	glTranslatef(0.0f, 2.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 0.0f); if (selected == 2) glColor3f(1.0f, 0.0f, 0.0f);
	glBindTexture( GL_TEXTURE_2D, quit_gfx );
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

GameState *MainMenuState::update(Uint32 timeDif, bool * prevInput, bool * curInput) {

	if (curInput[SDLK_RETURN]) {
		if (selected == 0)
			return new StageSelectState(SCREEN_WIDTH, SCREEN_HEIGHT, music);
		if (selected == 1)
			return new CreditsState(SCREEN_WIDTH, SCREEN_HEIGHT, music);
		if (selected == 2)
			exit(0);
	}

	if (curInput[SDLK_UP] && !prevInput[SDLK_UP]) selected = (selected + 2) % 3;
	if (curInput[SDLK_DOWN] && !prevInput[SDLK_DOWN]) selected = (selected + 1) % 3;

	return NULL;
}

MainMenuState::~MainMenuState() {

}