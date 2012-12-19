
#include "Common.h"
#include "Config.h"
#include "Arrow.h"

#include "Map.h"
#include "TextureManager.h"

#include "SDL/Include/SDL.h"
#include "SDL/Include/SDL_opengl.h"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

Arrow::Arrow(int x, int y, int dir, int tile_width, int tile_height, Map &m, string filename) : tilemap(m), alive(true),
			moveTicks(0) {
	tile_x = x;
	tile_y = y;
	direction = dir;

	switch (dir) {
		case DIR_DOWN:
			tex.x = 0.0f;
			tex.y = 0.0f;
			tex.w = 0.25f;
			tex.h = 1.0f;
			break;
		case DIR_UP:
			tex.x = 0.25f;
			tex.y = 0.0f;
			tex.w = 0.25f;
			tex.h = 1.0f;
			break;
		case DIR_LEFT:
			tex.x = 0.5f;
			tex.y = 0.0f;
			tex.w = 0.5f;
			tex.h = 0.5f;
			break;
		case DIR_RIGHT:
			tex.x = 0.5f;
			tex.y = 0.5f;
			tex.w = 0.5f;
			tex.h = 0.5f;
			break;
	}

	if (direction == DIR_DOWN || direction == DIR_UP) {
		width_in_tiles = 1;
		height_in_tiles = 2;
		width_in_pixels = tile_width;
		height_in_pixels = tile_height * 2;

		destX = tile_x;
		if (direction == DIR_DOWN) 
			destY = tile_y + 1;
		else destY = tile_y - 1;
	} else {
		width_in_tiles = 2;
		height_in_tiles = 1;
		width_in_pixels = tile_width * 2;
		height_in_pixels = tile_height;

		destY = tile_y;
		if (direction == DIR_RIGHT) 
			destX = tile_x + 1;
		else destX = tile_x - 1;
	}

	gfx = TextureManager::load(filename);

	MOVE_SPEED = atoi(Config::get("ARROW_MOVE_SPEED").c_str());
}

void Arrow::draw() {
	glPushMatrix();

	float x = tile_x * (width_in_pixels / width_in_tiles);
	x += (destX - tile_x)*(width_in_pixels / width_in_tiles)*(float(moveTicks) / float(MOVE_SPEED));

	float y = tile_y * (height_in_pixels / height_in_tiles);
	y += (destY - tile_y)*(height_in_pixels / height_in_tiles)*(float(moveTicks) / float(MOVE_SPEED));

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

rect<int> Arrow::get_bounds() {
	return rect<int>(tile_x * (width_in_pixels / width_in_tiles) + (destX - tile_x)*(width_in_pixels / width_in_tiles)*(float(moveTicks) / float(MOVE_SPEED)), 
		tile_y * (height_in_pixels / height_in_tiles) + (destY - tile_y)*(height_in_pixels / height_in_tiles)*(float(moveTicks) / float(MOVE_SPEED)), 
		width_in_pixels, height_in_pixels);

}

void Arrow::die(int damage) {
	alive = false;
}

bool Arrow::is_dead() {
	return !alive;
}

int Arrow::get_damage() {
	return 1;
}

list<GameObject*> Arrow::update(Uint32 tick_change, bool *prevInput, bool *curInput) {

	moveTicks += tick_change;
	if (moveTicks > MOVE_SPEED) {
		//At destination
		tile_x = destX;
		tile_y = destY;

		switch(direction) {
			case DIR_UP:
				destY--;
				if (!tilemap.walkable(destX, destY)) {
					tilemap.explode(destX, destY, get_damage());
					die(10);
				}
				break;
			case DIR_DOWN:
				destY++;
				if (!tilemap.walkable(destX, destY+1)) {
					tilemap.explode(destX, destY+1, get_damage());
					die(10);
				}
				break;
			case DIR_LEFT:
				destX--;
				if (!tilemap.walkable(destX, destY)) {
					tilemap.explode(destX, destY, get_damage());
					die(10);
				}
				break;
			case DIR_RIGHT:
				destX++;
				if (!tilemap.walkable(destX+1, destY)) {
					tilemap.explode(destX+1, destY, get_damage());
					die(10);
				}
				break;
		}

		//Stop moving
		moveTicks = 0;
	}

	return list<GameObject*>();
}