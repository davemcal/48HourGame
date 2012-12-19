#ifndef TILEMAP_H
#define TILEMAP_H

#include "TextureManager.h"
#include "SDL/Include/SDL_opengl.h"

#include <vector>
#include <string>
#include <set>
#include <map>
#include <fstream>
#include <iostream>

using namespace std;

class GameObject;

class Map {
	//Map Data
	int num_tiles_x, num_tiles_y;
	int tile_width, tile_height;

	vector<int> tiles;
	set<int> walls;
	map<int,GameObject*> blocks;

	//Tileset Data
	GLuint tileset;
	int tileset_x, tileset_y;
	float tileset_width, tileset_height;

	void drawTile(int x, int y);
	int get(int x, int y);

public:

	Map(ifstream &infile);

	void block(int x, int y, GameObject *g);
	void unblock(int x, int y);
	bool isBlocked(int x, int y);
	bool explode(int x, int y, int damage);
	bool walkable(int x, int y); 
	void draw();
	void reset_position(int &tile_x, int &tile_y, int width_in_tiles, int height_in_tiles);
	GameObject *spawn_powerup();

	string music_name;
};

#endif