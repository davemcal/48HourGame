#include "Explosion.h"
#include "Common.h"
#include "GameObject.h"
#include "Map.h"
#include "TextureManager.h"
#include "Config.h"

#include "SDL/Include/SDL.h"
#include "SDL/Include/SDL_opengl.h"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include <cstdlib>

using namespace std;

int Explosion::DISAPPEAR_SPEED;

Explosion::Explosion(int x, int y, int tile_width, int tile_height, Map &m, GLuint ngfx) :
	tile_x(x), tile_y(y), width_in_pixels(tile_width), height_in_pixels(tile_height), tilemap(m), alive(true),
	gfx(ngfx) {

	tex.x = 2.0f / 6.0f; tex.y = 0.0f; tex.w = 1.0f / 6.0f; tex.h = 1.0f;

	DISAPPEAR_SPEED = atoi(Config::get("EXPLOSION_LENGTH").c_str());
	countdown = DISAPPEAR_SPEED;
}

rect<int> Explosion::get_bounds() {
	return rect<int>(tile_x * width_in_pixels, tile_y * height_in_pixels, width_in_pixels, height_in_pixels);
}

void Explosion::draw() {

	glPushMatrix();

	float tex_offset = float(rand() % 4) / 6.0f;

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

void Explosion::die(int damage) {

}

bool Explosion::is_dead() {
	return !alive;
}

int Explosion::get_damage() {
	return 10;
}

list<GameObject*> Explosion::update(Uint32 tick_change, bool *prevInput, bool *curInput) {
	countdown -= tick_change;

	if (countdown <= 0)
		alive = false;

	return list<GameObject*>();
}

Explosion::~Explosion() {
}