#include "Wall.h"
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

using namespace std;

int Wall::MAX_HEALTH;

Wall::Wall(int x, int y, int tile_width, int tile_height, Map &m, string filename) :
	tile_x(x), tile_y(y), width_in_pixels(tile_width), height_in_pixels(tile_height), tilemap(m), alive(true) {

	gfx = TextureManager::load(filename);

	MAX_HEALTH = atoi(Config::get("WALL_HEALTH").c_str());
	health = MAX_HEALTH;

	tex.x = 0.0f; tex.y = 0.0f; tex.w = 1.0f; tex.h = 1.0f;

	tilemap.block(x, y, this);
}

rect<int> Wall::get_bounds() {
	return rect<int>(tile_x * width_in_pixels, tile_y * height_in_pixels, width_in_pixels, height_in_pixels);
}

void Wall::draw() {

	glPushMatrix();

	float x = tile_x * width_in_pixels;
	float y = tile_y * height_in_pixels;

	glTranslatef(x, y, 0.0f);
	glScalef(float(width_in_pixels), float(height_in_pixels), 1.0f);

	glBindTexture( GL_TEXTURE_2D, gfx );
	glBegin( GL_QUADS );
	
	//Top-left vertex (corner)
	glTexCoord2f( tex.x, tex.y );
	glVertex3f( 0.0f, 0.0f, 0.0f );
	
	//Bottom-left vertex (corner)
	glTexCoord2f( tex.x + tex.w, tex.y );
	glVertex3f( 1.0f, 0.0f, 0.0f );
	
	//Bottom-right vertex (corner)
	glTexCoord2f( tex.x + tex.w, tex.y + tex.h );
	glVertex3f( 1.0f, 1.0f, 0.0f );
	
	//Top-right vertex (corner)
	glTexCoord2f( tex.x, tex.y + tex.h);
	glVertex3f( 0.0f, 1.0f, 0.0f );

	glEnd();

	glPopMatrix();

}

void Wall::die(int damage) {
	health -= damage;
	if (health <= 0) alive = false;
}

bool Wall::is_dead() {
	return !alive;
}

int Wall::get_damage() {
	return 0;
}

void Wall::collide(int pnum) {

}

list<GameObject*> Wall::update(Uint32 tick_change, bool *prevInput, bool *curInput) {
	return list<GameObject*>();
}

Wall::~Wall() {
	tilemap.unblock(tile_x, tile_y);
}