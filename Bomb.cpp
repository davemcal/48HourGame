#include "Bomb.h"
#include "Explosion.h"
#include "Common.h"
#include "GameObject.h"
#include "Map.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Config.h"

#include "SDL/Include/SDL.h"
#include "SDL/Include/SDL_opengl.h"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

int Bomb::EXPLODE_SPEED;
int Bomb::PULSE_SPEED;
int Bomb::RADIUS;

Bomb::Bomb(int x, int y, int tile_width, int tile_height, Map &m, string filename) :
	tile_x(x), tile_y(y), width_in_pixels(tile_width), height_in_pixels(tile_height), tilemap(m), alive(true) {

	EXPLODE_SPEED = atoi(Config::get("BOMB_EXPLODE_SPEED").c_str());
	PULSE_SPEED = atoi(Config::get("BOMB_PULSE_SPEED").c_str());
	RADIUS = atoi(Config::get("BOMB_EXPLODE_RADIUS").c_str());
	countdown = EXPLODE_SPEED;

	gfx = TextureManager::load(filename);

	tex.x = 0.0f; tex.y = 0.0f; tex.w = (1.0f/6.0f); tex.h = 1.0f;

	tilemap.block(x, y, this);
}

rect<int> Bomb::get_bounds() {
	return rect<int>(tile_x * width_in_pixels, tile_y * height_in_pixels, width_in_pixels, height_in_pixels);
}

void Bomb::draw() {

	glPushMatrix();

	float tex_offset = float((countdown / PULSE_SPEED) % 2) / 6.0f;

	float x = tile_x * width_in_pixels;
	float y = tile_y * height_in_pixels;

	glTranslatef(x, y, 0.0f);
	glScalef(float(width_in_pixels), float(height_in_pixels), 1.0f);

	glBindTexture( GL_TEXTURE_2D, gfx );
	glBegin( GL_QUADS );
	
	//Top-left vertex (corner)
	glTexCoord2f( tex.x + tex_offset, tex.y );
	glVertex3f( 0.0f, 0.0f, 0.0f );
	
	//Bottom-left vertex (corner)
	glTexCoord2f( tex.x + tex_offset + tex.w, tex.y );
	glVertex3f( 1.0f, 0.0f, 0.0f );
	
	//Bottom-right vertex (corner)
	glTexCoord2f( tex.x + tex_offset + tex.w, tex.y + tex.h );
	glVertex3f( 1.0f, 1.0f, 0.0f );
	
	//Top-right vertex (corner)
	glTexCoord2f( tex.x + tex_offset, tex.y + tex.h);
	glVertex3f( 0.0f, 1.0f, 0.0f );

	glEnd();

	glPopMatrix();

}

void Bomb::die(int damage) {
	countdown = 0;	
}

bool Bomb::is_dead() {
	return !alive;
}

int Bomb::get_damage() {
	return 0;
}

void Bomb::collide(int pnum) {

}

list<GameObject*> Bomb::update(Uint32 tick_change, bool *prevInput, bool *curInput) {
	countdown -= tick_change;

	list<GameObject*> expl;

	if (countdown <= 0) {
		SoundManager::play("explosion1.ogg", false);
		alive = false;
		expl.push_back(new Explosion(tile_x, tile_y, width_in_pixels, height_in_pixels, tilemap, gfx));
		for (int x = -1 * RADIUS; x <= RADIUS; x++) {
			for (int y = -1 * RADIUS; y <= RADIUS; y++) {
				if (x*x + y*y <= RADIUS*RADIUS && (tilemap.explode(tile_x + x, tile_y + y, 10) || tilemap.walkable(tile_x + x, tile_y + y))) {
					expl.push_back(new Explosion(tile_x + x, tile_y + y, width_in_pixels, height_in_pixels, tilemap, gfx));
				}
			}
		}
	}

	return expl;
}

Bomb::~Bomb() {
	tilemap.unblock(tile_x, tile_y);
}