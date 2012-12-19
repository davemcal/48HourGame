#ifndef WALL_H
#define WALL_H

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

class Wall : public GameObject {

	int tile_x, tile_y;
	int width_in_pixels, height_in_pixels;
	GLuint gfx;
	Map &tilemap;
	static int MAX_HEALTH; int health;
	bool alive;

	rect<float> tex;

public:
	Wall(int x, int y, int tile_width, int tile_height, Map &m, string filename);
	rect<int> get_bounds();
	void draw();
	void die(int damage);
	bool is_dead();
	int get_damage();
	void collide(int pnum);
	list<GameObject*> update(Uint32 tick_change, bool *prevInput, bool *curInput);
	~Wall();
};

#endif