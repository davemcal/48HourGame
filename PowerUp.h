#ifndef POWERUP_H
#define POWERUP_H

#include "GameObject.h"
#include "Common.h"
#include "Player.h"
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

class PowerUp : public GameObject {

	int tile_x, tile_y;
	int width_in_pixels, height_in_pixels;
	Map &tilemap;
	bool alive;
	int type;
	int countdown; static int LIFE_SPAN;

	static const int NO_ARROW = 0;
	static const int NO_BOMB = 1;
	static const int NO_MOVE = 2;
	static const int NO_WALL = 3;

public:
	PowerUp(int x, int y, int tile_width, int tile_height, Map &m);
	rect<int> get_bounds();
	void draw();
	void die(int damage);
	bool is_dead();
	int get_damage();
	void collide(int pnum);
	list<GameObject*> update(Uint32 tick_change, bool *prevInput, bool *curInput);
	~PowerUp();

	static GLuint gfx;
	static Player *players[2];
	static void drawPowerUp(int type);
};

#endif