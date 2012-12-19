#include "Player.h"
#include "GameObject.h"
#include "Arrow.h"
#include "Bomb.h"
#include "Wall.h"
#include "Map.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Config.h"

#include "SDL/Include/SDL.h"
#include "SDL/Include/SDL_opengl.h"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

int Player::MAX_MOVE_COOLDOWN ;
int Player::MAX_ATTACK_COOLDOWN;
int Player::MAX_BOMB_COOLDOWN;
int Player::MAX_WALL_COOLDOWN;
int Player::COOL_DOWN;
int Player::SLOW_MOVE_SPEED;
int Player::MAX_HEALTH;

Player::Player(ifstream &infile, Map &m, int npnum) : pnum(npnum), pix_x(0), pix_y(0), tilemap(m),
			MOVE_UP(SDLK_UP), MOVE_DOWN(SDLK_DOWN), MOVE_LEFT(SDLK_LEFT), MOVE_RIGHT(SDLK_RIGHT),
			FIRE_ARROW(SDLK_KP4), DROP_BOMB(SDLK_KP5), DROP_WALL(SDLK_KP6), cooldown(0),
			can_attack(true), can_wall(true), can_bomb(true), move_cooldown(0), attack_cooldown(0),
			wall_cooldown(true), bomb_cooldown(true) {
	string gfx_filename;
	infile >> gfx_filename >> arrow_gfx_filename >> bomb_gfx_filename >> wall_gfx_filename;

	infile >> width_in_tiles >> height_in_tiles >> width_in_pixels >> height_in_pixels;

	gfx = TextureManager::load(gfx_filename);

	MAX_MOVE_COOLDOWN = atoi(Config::get("SLOW_POWERUP_LENGTH").c_str());
	SLOW_MOVE_SPEED = atoi(Config::get("SLOW_MOVE_SPEED").c_str());
	MAX_ATTACK_COOLDOWN = atoi(Config::get("ATTACK_POWERUP_LENGTH").c_str());
	MAX_BOMB_COOLDOWN = atoi(Config::get("BOMB_POWERUP_LENGTH").c_str());
	MAX_WALL_COOLDOWN = atoi(Config::get("WALL_POWERUP_LENGTH").c_str());
	MOVE_SPEED = atoi(Config::get("PLAYER_MOVE_SPEED").c_str());
	COOL_DOWN = atoi(Config::get("PLAYER_ACTION_COOLDOWN").c_str());
	MAX_HEALTH = atoi(Config::get("PLAYER_HEALTH").c_str());

	direction = DIR_UP;
	moving = false;
	moveTicks = 0;

	alive = true;
}

int Player::get_health() {
	return health;
}

void Player::set_input(int left, int right, int up, int down, int fire, int bomb, int wall) {
	MOVE_LEFT = left;
	MOVE_RIGHT = right;
	MOVE_UP = up;
	MOVE_DOWN = down;
	FIRE_ARROW = fire;
	DROP_BOMB = bomb;
	DROP_WALL = wall;
}

void Player::set_tile_pos(int x, int y) {
	tile_x = x;
	tile_y = y;
	destX = tile_x;
	destY = tile_y;
	pix_x = 0;
	pix_y = 0;
}

void Player::draw() {
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
	glTexCoord2f( float(direction) / 4.0f, 0.0f );
	glVertex3f( 0.0f, 0.0f, 0.0f );
	
	//Bottom-left vertex (corner)
	glTexCoord2f( float(direction) / 4.0f + 0.25f, 0.0f );
	glVertex3f( 1.0f, 0.0f, 0.0f );
	
	//Bottom-right vertex (corner)
	glTexCoord2f( float(direction) / 4.0f + 0.25f, 1.0f );
	glVertex3f( 1.0f, 1.0f, 0.0f );
	
	//Top-right vertex (corner)
	glTexCoord2f( float(direction) / 4.0f, 1.0f );
	glVertex3f( 0.0f, 1.0f, 0.0f );

	glEnd();

	glPopMatrix();
}

rect<int> Player::get_bounds() {
	return rect<int>(tile_x * (width_in_pixels / width_in_tiles) + (destX - tile_x)*(width_in_pixels / width_in_tiles)*(float(moveTicks) / float(MOVE_SPEED)), 
		tile_y * (height_in_pixels / height_in_tiles) + (destY - tile_y)*(height_in_pixels / height_in_tiles)*(float(moveTicks) / float(MOVE_SPEED))
		, width_in_pixels, height_in_pixels);

}

void Player::check_collision(GameObject &a) {
	if (a.get_bounds().collides(get_bounds())) {
		health -= a.get_damage();
		if (health <= 0) {
			die();
			if (a.get_damage() == 1) {
				SoundManager::play("death_arrow.ogg", false);
			} else {
				SoundManager::play("death_bomb.ogg", false);
			}
		}
		a.collide(pnum);
	}
}

void Player::die() {
	alive = false;
}

bool Player::is_dead() {
	return !alive;
}

void Player::reset() {
	health = MAX_HEALTH;
	alive = true;
	move_cooldown = 0;
	MOVE_SPEED = 50;
	moving = false;
	attack_cooldown = 0;
	bomb_cooldown = 0;
	wall_cooldown = 0;

	tilemap.reset_position(tile_x, tile_y, width_in_tiles, height_in_tiles);
}

void Player::fire_arrow(vector<GameObject*> &objects) {
	if (attack_cooldown) return;
	int start_x = tile_x, start_y = tile_y;

	switch(direction) {
		case DIR_UP:
			start_y-=3;
			start_x += width_in_tiles / 2;
			if (!tilemap.walkable(start_x, start_y-1)) return;
			break;
		case DIR_DOWN:
			start_y+=height_in_tiles+1;
			start_x += width_in_tiles / 2;
			if (!tilemap.walkable(start_x, start_y+2)) return;
			break;
		case DIR_LEFT:
			start_x-=3;
			start_y += height_in_tiles / 2;	
			if (!tilemap.walkable(start_x-1, start_y)) return;
			break;
		case DIR_RIGHT:
			start_x+=width_in_tiles+1;
			start_y += height_in_tiles / 2;
			if (!tilemap.walkable(start_x+2, start_y)) return;
			break;
	}

	if (tilemap.walkable(start_x, start_y)) {
		cooldown = COOL_DOWN;

		GameObject *g = new Arrow(start_x, start_y, direction, 
			width_in_pixels / width_in_tiles, height_in_pixels / height_in_tiles, tilemap, arrow_gfx_filename);

		objects.push_back(g);

		SoundManager::play("bow_shot.ogg", false);
	}
}

void Player::drop_wall(vector<GameObject*> &objects) {
	if (wall_cooldown) return;
	int start_x = tile_x, start_y = tile_y;

	switch(direction) {
		case DIR_UP:
			start_y-=3;
			start_x += width_in_tiles / 2;
			
			for (int i = -2; i <= 2; i++) {
				if (tilemap.walkable(start_x + i, start_y)) {
					cooldown = COOL_DOWN;

					GameObject *g = new Wall(start_x + i, start_y,  
					width_in_pixels / width_in_tiles, height_in_pixels / height_in_tiles, tilemap, wall_gfx_filename);

					objects.push_back(g);
				}
			}
			break;
		case DIR_DOWN:
			start_y+=height_in_tiles+1;
			start_x += width_in_tiles / 2;
			
			for (int i = -2; i <= 2; i++) {
				if (tilemap.walkable(start_x + i, start_y)) {
					cooldown = COOL_DOWN;

					GameObject *g = new Wall(start_x + i, start_y,  
					width_in_pixels / width_in_tiles, height_in_pixels / height_in_tiles, tilemap, wall_gfx_filename);

					objects.push_back(g);
				}
			}

			break;
		case DIR_LEFT:
			start_x-=3;
			start_y += height_in_tiles / 2;	

			for (int i = -2; i <= 2; i++) {
				if (tilemap.walkable(start_x, start_y + i)) {
					cooldown = COOL_DOWN;

					GameObject *g = new Wall(start_x, start_y + i,  
					width_in_pixels / width_in_tiles, height_in_pixels / height_in_tiles, tilemap, wall_gfx_filename);

					objects.push_back(g);
				}
			}

			break;
		case DIR_RIGHT:
			start_x+=width_in_tiles+1;
			start_y += height_in_tiles / 2;
			for (int i = -2; i <= 2; i++) {
				if (tilemap.walkable(start_x, start_y + i)) {
					cooldown = COOL_DOWN;

					GameObject *g = new Wall(start_x, start_y + i,  
					width_in_pixels / width_in_tiles, height_in_pixels / height_in_tiles, tilemap, wall_gfx_filename);

					objects.push_back(g);
				}
			}
			break;
	}
	SoundManager::play("make_wall.ogg", false);
}

void Player::drop_bomb(vector<GameObject*> &objects) {
	if (bomb_cooldown) return;
	int x = tile_x + width_in_tiles / 2, y = tile_y + height_in_tiles / 2;

	if (tilemap.walkable(x, y)) {
		cooldown = COOL_DOWN;

		GameObject *g = new Bomb(x, y,  
			width_in_pixels / width_in_tiles, height_in_pixels / height_in_tiles, tilemap, bomb_gfx_filename);

		objects.push_back(g);
	}
}

vector<GameObject*> Player::update(Uint32 tick_change, bool *prevInput, bool *curInput) {
	
	vector<GameObject*> new_objects;

	if (curInput[MOVE_RIGHT]) {
		move_right();
	}
	if (curInput[MOVE_LEFT]) {
		move_left();
	}
	if (curInput[MOVE_UP]) {
		move_up();
	}
		
	if (curInput[MOVE_DOWN]) {
		move_down();
	}

	if (curInput[FIRE_ARROW]) {
		if (cooldown <= 0)	
			fire_arrow(new_objects);
	}

	if (curInput[DROP_BOMB]) {
		if (cooldown <= 0)	
			drop_bomb(new_objects);
	}

	if (curInput[DROP_WALL]) {
		if (cooldown <= 0)	
			drop_wall(new_objects);
	}

	if (cooldown > 0) {
		cooldown -= tick_change;
		if (cooldown < 0) cooldown = 0;
	}

	if (move_cooldown > 0) {
		move_cooldown -= tick_change;
		if (move_cooldown <= 0) {
			MOVE_SPEED = 50;
			move_cooldown = 0;
		}
	}

	if (attack_cooldown > 0) {
		attack_cooldown -= tick_change;
		if (attack_cooldown <= 0) attack_cooldown = 0;
	}

	if (bomb_cooldown > 0) {
		bomb_cooldown -= tick_change;
		if (bomb_cooldown <= 0) bomb_cooldown = 0;
	}

	if (wall_cooldown > 0) {
		wall_cooldown -= tick_change;
		if (wall_cooldown <= 0) wall_cooldown = 0;
	}

	if (!moving) return new_objects;
	moveTicks += tick_change;
	if (moveTicks > MOVE_SPEED) {
		//At destination
		tile_x = destX;
		tile_y = destY;

		//Stop moving
		moving = false;
		moveTicks = 0;
	}

	return new_objects;
}

void Player::move_right() {
	if (moving) return;
	direction = DIR_RIGHT;

	//Can't move into a wall
	for (int i = 0; i < height_in_tiles; i++)
		if (!tilemap.walkable(tile_x + width_in_tiles, tile_y + i)) return;

	//Start moving
	destX = tile_x + 1;
	destY = tile_y;
	moving = true;
}

void Player::move_left() {
	if (moving) return;
	direction = DIR_LEFT;

	//Can't move into a wall
	for (int i = 0; i < height_in_tiles; i++)
		if (!tilemap.walkable(tile_x - 1, tile_y + i)) return;

	//Start moving
	destX = tile_x - 1;
	destY = tile_y;
	moving = true;
}

void Player::move_up() {
	if (moving) return;
	direction = DIR_UP;

	//Can't move into a wall
	for (int i = 0; i < width_in_tiles; i++)
		if (!tilemap.walkable(tile_x + i, tile_y - 1)) return;

	//Start moving
	destX = tile_x;
	destY = tile_y - 1;
	moving = true;
}


void Player::move_down() {
	if (moving) return;
	direction = DIR_DOWN;

	//Can't move into a wall
	for (int i = 0; i < width_in_tiles; i++)
		if (!tilemap.walkable(tile_x + i, tile_y + height_in_tiles)) return;
	
	//Start moving
	destX = tile_x;
	destY = tile_y + 1;
	moving = true;
}

void Player::slow_move() {
	MOVE_SPEED = SLOW_MOVE_SPEED;
	move_cooldown = MAX_MOVE_COOLDOWN;
}

void Player::no_attack() {
	attack_cooldown = MAX_ATTACK_COOLDOWN;
}

void Player::no_bomb() {
	bomb_cooldown = MAX_BOMB_COOLDOWN;
}

void Player::no_wall() {
	wall_cooldown = MAX_WALL_COOLDOWN;
}

bool Player::is_slow_move() {
	return move_cooldown > 0;
}

bool Player::is_no_attack() {
	return attack_cooldown > 0;
}

bool Player::is_no_bomb() {
	return bomb_cooldown > 0;
}

bool Player::is_no_wall() {
	return wall_cooldown > 0;
}