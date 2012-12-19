#ifndef ARROW_H
#define ARROW_H

#include "Common.h"
#include "GameObject.h"
#include "Map.h"
#include "TextureManager.h"

#include "SDL/Include/SDL.h"
#include "SDL/Include/SDL_opengl.h"

#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class Arrow : public GameObject {

	static const int DIR_DOWN = 0;
	static const int DIR_UP = 1;
	static const int DIR_LEFT = 2;
	static const int DIR_RIGHT = 3;

	int tile_x, tile_y;
	int pix_x, pix_y;
	int direction;
	int destX, destY, moveTicks;
	int MOVE_SPEED;
	int width_in_tiles, height_in_tiles, width_in_pixels, height_in_pixels;
	GLuint gfx;
	Map &tilemap;
	bool alive;

	rect<float> tex;

public:
	Arrow(int x, int y, int dir, int tile_width, int tile_height, Map &m, string filename);
	rect<int> get_bounds();
	void draw();
	void die(int damage);
	bool is_dead();
	int get_damage();
	list<GameObject*> update(Uint32 tick_change, bool *prevInput, bool *curInput);
	~Arrow() {}
};

#endif