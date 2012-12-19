#include "CreditsState.h"
#include "MainMenuState.h"
#include "TextureManager.h"
#include "SoundManager.h"

#include "SDL/Include/SDL.h"
#include "SDL/Include/SDL_mixer.h"

CreditsState::CreditsState(int width, int height, int music_chan) : SCREEN_WIDTH(width), SCREEN_HEIGHT(height) {
	credits_gfx = TextureManager::load("Credits_page.png");

	if (music_chan == -1) {
		Mix_PlayMusic(Mix_LoadMUS("menu_music.ogg"), -1);
	} else {
		music = music_chan;
	}
}

void CreditsState::display() {
	glScalef(float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 1.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
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
}

GameState *CreditsState::update(Uint32 timeDif, bool * prevInput, bool * curInput) {

	if (curInput[SDLK_RETURN] || curInput[SDLK_ESCAPE])
			return new MainMenuState(SCREEN_WIDTH, SCREEN_HEIGHT, music);

	return NULL;
}

CreditsState::~CreditsState() {

}