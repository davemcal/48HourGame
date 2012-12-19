#ifndef BOMB_H
#define BOMB_H

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

class Bomb : public GameObject {

	int tile_x, tile_y;
	int width_in_pixels, height_in_pixels;
	GLuint gfx;
	Map &tilemap;
	bool alive;

	rect<float> tex;

	int countdown;

	static int RADIUS;
	static int EXPLODE_SPEED;
	static int PULSE_SPEED;

public:
	Bomb(int x, int y, int tile_width, int tile_height, Map &m, string filename);
	rect<int> get_bounds();
	void draw();
	void die(int damage);
	bool is_dead();
	int get_damage();
	void collide(int pnum);
	list<GameObject*> update(Uint32 tick_change, bool *prevInput, bool *curInput);
	~Bomb();
};

#endif