#include "PowerUp.h"
#include "Common.h"
#include "GameObject.h"
#include "Map.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Player.h"
#include "Config.h"

#include "SDL/Include/SDL.h"
#include "SDL/Include/SDL_opengl.h"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;

GLuint PowerUp::gfx = 0;
Player *PowerUp::players[2];
int PowerUp::LIFE_SPAN;

PowerUp::PowerUp(int x, int y, int tile_width, int tile_height, Map &m) : 
	tile_x(x), tile_y(y), width_in_pixels(tile_width*2), height_in_pixels(tile_height*2), tilemap(m), alive(true) {

	type = rand() % 4;

	LIFE_SPAN = atoi(Config::get("POWERUP_LIFE").c_str());
	countdown = LIFE_SPAN;
}

rect<int> PowerUp::get_bounds() {
	return rect<int>(tile_x * (width_in_pixels / 2), tile_y * (height_in_pixels / 2), width_in_pixels, height_in_pixels);
}

void PowerUp::draw() {

	glPushMatrix();

	float tex_offset = float(type) / 4.0f;

	float x = float(tile_x * (width_in_pixels / 2));
	float y = float(tile_y * (height_in_pixels / 2));

	glTranslatef(x, y, 0.0f);
	glScalef(float(width_in_pixels), float(height_in_pixels), 1.0f);

	glBindTexture( GL_TEXTURE_2D, gfx );
	glBegin( GL_QUADS );
	
	//Top-left vertex (corner)
	glTexCoord2f( tex_offset, 0.0f );
	glVertex3f( 0.0f, 0.0f, 0.0f );
	
	//Bottom-left vertex (corner)
	glTexCoord2f( 1.0f / 4.0f + tex_offset, 0.0f );
	glVertex3f( 1.0f, 0.0f, 0.0f );
	
	//Bottom-right vertex (corner)
	glTexCoord2f( 1.0f / 4.0f + tex_offset, 1.0f );
	glVertex3f( 1.0f, 1.0f, 0.0f );
	
	//Top-right vertex (corner)
	glTexCoord2f( tex_offset, 1.0f);
	glVertex3f( 0.0f, 1.0f, 0.0f );

	glEnd();

	glPopMatrix();

}

void PowerUp::die(int damage) {
	alive = false;
}

bool PowerUp::is_dead() {
	return !alive;
}

int PowerUp::get_damage() {
	return 0;
}

void PowerUp::collide(int pnum) {
	switch (type) {
		case NO_ARROW:
			SoundManager::play("power_down1.ogg", false);
			players[(pnum+1)%2]->no_attack();
			break;
		case NO_BOMB:
			SoundManager::play("power_down2.ogg", false);
			players[(pnum+1)%2]->no_bomb();
			break;
		case NO_MOVE:
			SoundManager::play("power_down3.ogg", false);
			players[(pnum+1)%2]->slow_move();
			break;
		case NO_WALL:
			SoundManager::play("power_down4.ogg", false);
			players[(pnum+1)%2]->no_wall();
			break;
	}

	die(10);
}

list<GameObject*> PowerUp::update(Uint32 tick_change, bool *prevInput, bool *curInput) {
	countdown -= tick_change;

	if (countdown <= 0) die(10);

	return list<GameObject*>();
}

PowerUp::~PowerUp() {
}

void PowerUp::drawPowerUp(int type) {
	glBindTexture( GL_TEXTURE_2D, PowerUp::gfx );
	
	glBegin( GL_QUADS );

	glTexCoord2f( float(type) / 4.0f, 0.0f );
	glVertex3f( 0.0f, 0.0f, 0.0f );

	//Bottom-left vertex (corner)
	glTexCoord2f( float(type + 1) / 4.0f, 0.0f );
	glVertex3f( 20.0f, 0.0f, 0.0f );

	//Bottom-right vertex (corner)
	glTexCoord2f( float(type + 1) / 4.0f, 1.0f );
	glVertex3f( 20.0f, 20.0f, 0.0f );

	//Top-right vertex (corner)
	glTexCoord2f( float(type) / 4.0f, 1.0f);
	glVertex3f( 0.0f, 20.0f, 0.0f );

	glEnd();

}
