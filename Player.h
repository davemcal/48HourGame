#ifndef PLAYER_H
#define PLAYER_H

#include "Map.h"
#include "TextureManager.h"
#include "Common.h"
#include "GameObject.h"

#include "SDL/Include/SDL.h"
#include "SDL/Include/SDL_opengl.h"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class Player {

	static const int DIR_DOWN = 0;
	static const int DIR_UP = 1;
	static const int DIR_LEFT = 2;
	static const int DIR_RIGHT = 3;

	int pnum;

	int tile_x, tile_y;
	int pix_x, pix_y;
	int direction;
	int destX, destY, moveTicks;
	int MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT, FIRE_ARROW, DROP_BOMB, DROP_WALL;
	int MOVE_SPEED;
	static int COOL_DOWN;
	static int SLOW_MOVE_SPEED;
	int width_in_tiles, height_in_tiles, width_in_pixels, height_in_pixels;
	GLuint gfx; string arrow_gfx_filename; string bomb_gfx_filename; string wall_gfx_filename;
	Map &tilemap;

	bool alive;

	int health;

	int cooldown;

	bool moving;

	bool can_attack, can_wall, can_bomb;
	int move_cooldown; static int MAX_MOVE_COOLDOWN ;
	int attack_cooldown; static int MAX_ATTACK_COOLDOWN;
	int bomb_cooldown; static int MAX_BOMB_COOLDOWN;
	int wall_cooldown; static int MAX_WALL_COOLDOWN;

	void move_left();
	void move_right();
	void move_up();
	void move_down();

	void fire_arrow(vector<GameObject*> &objects);
	void drop_bomb(vector<GameObject*> &objects);
	void drop_wall(vector<GameObject*> &objects);

public:
	Player(ifstream &infile, Map &m, int npnum);
	void set_tile_pos(int x, int y);
	void set_input(int left, int right, int up, int down, int fire, int bomb, int wall);
	void draw();
	bool is_dead();
	void die();
	void reset();
	int get_health();
	rect<int> get_bounds();
	void check_collision(GameObject &a);
	vector<GameObject*> update(Uint32 tick_change, bool *prevInput, bool *curInput);

	void slow_move();
	void no_attack();
	void no_bomb();
	void no_wall();

	bool is_slow_move();
	bool is_no_attack();
	bool is_no_bomb();
	bool is_no_wall();

	static int MAX_HEALTH;
};

#endif