#include "Map.h"
#include "TextureManager.h"

#include "GameObject.h"
#include "PowerUp.h"

#include "SDL/Include/SDL_opengl.h"

#include <vector>
#include <set>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

Map::Map(ifstream &infile) {

	string gfx_filename;
	infile >> gfx_filename >> tileset_x >> tileset_y >> tileset_width >> tileset_height;

	tileset = TextureManager::load(gfx_filename);

	infile >> num_tiles_x >> num_tiles_y >> tile_width >> tile_height;

	infile >> music_name;

	for (int y = 0; y < num_tiles_y; y++) {
		for (int x = 0; x < num_tiles_x; x++) {
			int tile; infile >> tile;
			tiles.push_back(tile-1);
		}
	}

	while (infile) {
		int wall;
		infile >> wall;
		//cout << wall <<  " ";
		walls.insert(wall-1);
	}
}

void Map::block(int x, int y, GameObject *g) {
	blocks[x + y * num_tiles_x] = g;
}

void Map::unblock(int x, int y) {
	blocks.erase(blocks.find(x + y * num_tiles_x));
}

bool Map::explode(int x, int y, int damage) {
	map<int, GameObject*>::iterator it =  blocks.find(x + y * num_tiles_x);
	if (it != blocks.end()) {
		it->second->die(damage);
		return true;
	}
	return false;
}

bool Map::walkable(int x, int y) {
	if (x < 0 || y < 0 || x >= num_tiles_x || y >= num_tiles_y) return false;
	if (blocks.find(x + y * num_tiles_x) != blocks.end()) return false;
	int g = get(x,y);
	return walls.find(g) == walls.end();
}

void Map::draw() {
	glBindTexture( GL_TEXTURE_2D, tileset );

	for (int y = 0; y < num_tiles_y; y++) {
		for (int x = 0; x < num_tiles_x; x++) {
			//if (!walkable(x, y)) 
			drawTile(x, y);
		}
	}
}

int Map::get(int x, int y) {
	return tiles[x + y * num_tiles_x];
}

void Map::drawTile(int x, int y) {
	glPushMatrix();

	glTranslatef(float(tile_width * x), float(tile_height * y), 0.0f);
	glScalef(float(tile_width), float(tile_height), 1.0f);

	glBegin( GL_QUADS );
	//Top-left vertex (corner)
	glTexCoord2f( float(get(x, y) % tileset_x) / float(tileset_x), 
					float(get(x, y) / tileset_x) / float(tileset_y) );
	glVertex3f( 0.0f, 0.0f, 0.0f );
	
	//Bottom-left vertex (corner)
	glTexCoord2f( float(get(x, y) % tileset_x + 1) / float(tileset_x), 
					float(get(x, y) / tileset_x) / float(tileset_y) );
	glVertex3f( 1.0f, 0.0f, 0 );
	
	//Bottom-right vertex (corner)
	glTexCoord2f( float(get(x, y) % tileset_x + 1) / float(tileset_x), 
					float(get(x, y) / tileset_x + 1) / float(tileset_y));
	glVertex3f( 1.0f, 1.0f, 0 );
	
	//Top-right vertex (corner)
	glTexCoord2f( float(get(x, y) % tileset_x) / float(tileset_x), 
					float(get(x, y) / tileset_x + 1) / float(tileset_y) );
	glVertex3f( 0.0f, 1.0f, 0 );
	
	glEnd();

	glPopMatrix();

}

GameObject *Map::spawn_powerup() {
	int x, y;

	do {
		x = rand() % num_tiles_x;
		y = rand() % num_tiles_y;
	} while (!walkable(x, y) || !walkable(x + 1, y) || !walkable(x, y+1) || !walkable(x + 1, y + 1));

	GameObject *g = new PowerUp(x, y, tile_width, tile_height, *this);

	return g;
}

void Map::reset_position(int &tile_x, int &tile_y, int width_in_tiles, int height_in_tiles) {

	while (true) {
		tile_x = rand() % num_tiles_x;
		tile_y = rand() % num_tiles_y;

		bool ok = true;

		for (int j = 0; j < height_in_tiles; j++) {
			for (int i = 0; i < width_in_tiles; i++) {
				if (!walkable(tile_x + i, tile_y + j)) {
					ok = false;
				}
			}
		}

		if (ok) return;
	};
}