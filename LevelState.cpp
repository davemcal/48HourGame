#include "LevelState.h"
#include "MainMenuState.h"
#include "StageSelectState.h"
#include "GameObject.h"
#include "Player.h"
#include "PowerUp.h"
#include "SoundManager.h"
#include "Config.h"

#include "SDL/Include/SDL.h"
#include "SDL/Include/SDL_mixer.h"
#include "SDL/Include/SDL_opengl.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <list>
#include <string>

using namespace std;

bool check_dead(GameObject *a) {
	return a->is_dead();
}

void deleter(GameObject *a) {
	if (a->is_dead()) 
		delete a;
}

struct updater {
	Uint32 tick_change;
	bool * prevInput;
	bool * curInput;
	list<GameObject*> new_objects;

public:
	updater(Uint32 t, bool* prevI, bool* curI): tick_change(t), prevInput(prevI), curInput(curI) {}

	void operator() (GameObject *a) {
		if (a->is_dead()) return;
		list<GameObject*> objs = a->update(tick_change, prevInput, curInput);

		new_objects.insert(new_objects.end(), objs.begin(), objs.end());
	}
};

void object_draw(GameObject *a) {
	a->draw();
}

class check_collisions {
	Player &p1;
	Player &p2;

public:
	check_collisions(Player &np1, Player &np2): p1(np1), p2(np2) {}

	void operator() (GameObject *a) {
		p1.check_collision(*a);
		p2.check_collision(*a);
	}
};

LevelState::LevelState(std::string map_file_name, int width, int height) : 
						m(ifstream(map_file_name.c_str())) , p1(ifstream("red_player.txt"), m, 0) ,
						p2(ifstream("blue_player.txt"), m, 1) , red_score(0), blue_score(0) ,
						SCREEN_WIDTH(width), SCREEN_HEIGHT(height) {

	p1.reset();
	p2.reset();
	p2.set_input(SDLK_a, SDLK_d, SDLK_w, SDLK_s, SDLK_g, SDLK_h, SDLK_j);

	red_gfx = TextureManager::load("red_winner.png");
	blue_gfx = TextureManager::load("blue_winner.png");

	PowerUp::gfx = TextureManager::load("power_down_set.png");
	PowerUp::players[0] = &p1;
	PowerUp::players[1] = &p2;

	spawn_ticks = 1;

	Mix_PlayMusic(Mix_LoadMUS(m.music_name.c_str()), -1);
}

void LevelState::display() {

	m.draw();

	for_each(objects.begin(), objects.end(), object_draw);

	p1.draw();
	p2.draw();

	//Health Bars
	glDisable(GL_TEXTURE_2D);
	glTranslatef(20.0f, 10.0f, 0.0f);

	glPushMatrix();
	glBegin(GL_QUADS);
		glColor3f(0.5f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(40.0f * Player::MAX_HEALTH, 0.0f, 0.0f);
		glVertex3f(40.0f * Player::MAX_HEALTH, 20.0f, 0.0f);
		glVertex3f(0.0f, 20.0f, 0.0f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(2.0f, 2.0f, 0.0f);
		glVertex3f(40.0f * p1.get_health() - 2.0f, 2.0f, 0.0f);
		glVertex3f(40.0f * p1.get_health() - 2.0f, 20.0f - 2.0f, 0.0f);
		glVertex3f(2.0f, 20.0f - 2.0f, 0.0f);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(40.0f * Player::MAX_HEALTH, 0.0f, 0.0f);
	if (p1.is_no_attack()) {
		glTranslatef(20.0f, 0.0f, 0.0f);
		PowerUp::drawPowerUp(0);
	}
	if (p1.is_no_bomb()) {
		glTranslatef(20.0f, 0.0f, 0.0f);
		PowerUp::drawPowerUp(1);
	}
	if (p1.is_slow_move()) {
		glTranslatef(20.0f, 0.0f, 0.0f);
		PowerUp::drawPowerUp(2);
	}
	if (p1.is_no_wall()) {
		glTranslatef(20.0f, 0.0f, 0.0f);
		PowerUp::drawPowerUp(3);
	}

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(SCREEN_WIDTH - 40.0f * Player::MAX_HEALTH - 40.0f, 0.0f, 0.0f);

	glBegin(GL_QUADS);
		glColor3f(0.0f, 0.0f, 0.5f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(40.0f * Player::MAX_HEALTH, 0.0f, 0.0f);
		glVertex3f(40.0f * Player::MAX_HEALTH, 20.0f, 0.0f);
		glVertex3f(0.0f, 20.0f, 0.0f);

		glColor3f(0.3f, 0.3f, 1.0f);
		glVertex3f(2.0f, 2.0f, 0.0f);
		glVertex3f(40.0f * p2.get_health() - 2.0f, 2.0f, 0.0f);
		glVertex3f(40.0f * p2.get_health() - 2.0f, 20.0f - 2.0f, 0.0f);
		glVertex3f(2.0f, 20.0f - 2.0f, 0.0f);
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glTranslatef(-20.0f, 0.0f, 0.0f);
	if (p2.is_no_attack()) {
		glTranslatef(-20.0f, 0.0f, 0.0f);
		PowerUp::drawPowerUp(0);
	}
	if (p2.is_no_bomb()) {
		glTranslatef(-20.0f, 0.0f, 0.0f);
		PowerUp::drawPowerUp(1);
	}
	if (p2.is_slow_move()) {
		glTranslatef(-20.0f, 0.0f, 0.0f);
		PowerUp::drawPowerUp(2);
	}
	if (p2.is_no_wall()) {
		glTranslatef(-20.0f, 0.0f, 0.0f);
		PowerUp::drawPowerUp(3);
	}

	glPopMatrix();

	glPushMatrix();
	stringstream o;
	o << red_score;

	glColor3f(1.0f, 0.0f, 0.0f);
	TextureManager::displayText(o.str(), SCREEN_WIDTH / 2 - 75 - o.str().size() * 25 / 2, 10, 25, 40);

	stringstream p;
	p << blue_score;

	glColor3f(0.0f, 0.0f, 1.0f);
	TextureManager::displayText(p.str(), SCREEN_WIDTH / 2 + 75 - p.str().size() * 25 / 2, 10, 25, 40);
	glPopMatrix();
	
	glColor3f(1.0f, 1.0f, 1.0f);

	if (red_score >= 5) {
		glTranslatef(float(SCREEN_WIDTH / 2) - 250.0f, float(SCREEN_HEIGHT / 2) - 50.0f, 0.0f);
		glScalef(500.0f, 100.0f, 1.0f);
		glBindTexture( GL_TEXTURE_2D, red_gfx );
		
		glBegin( GL_QUADS );

		glTexCoord2f( 0.0f, 0.0f );
		glVertex3f( 0.0f, 0.0f, 0.0f );

		//Bottom-left vertex (corner)
		glTexCoord2f( 1.0f, 0.0f );
		glVertex3f( 1.0f, 0.0f, 0.0f );

		//Bottom-right vertex (corner)
		glTexCoord2f( 1.0f, 1.0f );
		glVertex3f( 1.0f, 1.0f, 0.0f );

		//Top-right vertex (corner)
		glTexCoord2f( 0.0f, 1.0f);
		glVertex3f( 0.0f, 1.0f, 0.0f );

		glEnd();
	} else if (blue_score >= 5) {
		glTranslatef(float(SCREEN_WIDTH / 2) - 250.0f, float(SCREEN_HEIGHT / 2) - 50.0f, 0.0f);
		glScalef(500.0f, 100.0f, 1.0f);
		glBindTexture( GL_TEXTURE_2D, blue_gfx );
		
		glBegin( GL_QUADS );

		glTexCoord2f( 0.0f, 0.0f );
		glVertex3f( 0.0f, 0.0f, 0.0f );

		//Bottom-left vertex (corner)
		glTexCoord2f( 1.0f, 0.0f );
		glVertex3f( 1.0f, 0.0f, 0.0f );

		//Bottom-right vertex (corner)
		glTexCoord2f( 1.0f, 1.0f );
		glVertex3f( 1.0f, 1.0f, 0.0f );

		//Top-right vertex (corner)
		glTexCoord2f( 0.0f, 1.0f);
		glVertex3f( 0.0f, 1.0f, 0.0f );

		glEnd();
	}

}

GameState *LevelState::update(Uint32 time_dif, bool * prevInput, bool * curInput) {

	if (curInput[SDLK_ESCAPE]) {
		Mix_HaltMusic();
		return new MainMenuState(SCREEN_WIDTH, SCREEN_HEIGHT);
	}

	if (red_score >= 5 || blue_score >= 5) {

		if (curInput[SDLK_RETURN] || curInput[SDLK_ESCAPE])
			return new StageSelectState(SCREEN_WIDTH, SCREEN_HEIGHT);

		return NULL;
	}
	
	vector<GameObject*> new_objects = p1.update(time_dif, prevInput, curInput);

	objects.insert(objects.end(), new_objects.begin(), new_objects.end());

	new_objects = p2.update(time_dif, prevInput, curInput);

	objects.insert(objects.end(), new_objects.begin(), new_objects.end());

	spawn_ticks -= time_dif;
	if (spawn_ticks < 0) {
		spawn_ticks = atoi(Config::get("POWERUP_SPAWN_TIME").c_str());
		if (rand() % atoi(Config::get("POWERUP_SPAWN_CHANCE").c_str()) == 0) {
			objects.push_back(m.spawn_powerup());
		}
	}

	updater u(time_dif, prevInput, curInput);
	u = for_each(objects.begin(), objects.end(), u);

	objects.insert(objects.end(), u.new_objects.begin(), u.new_objects.end());
	
	for_each(objects.begin(), objects.end(), check_collisions(p1, p2));
	
	list<GameObject *> alive_objects;
	remove_copy_if(objects.begin(), objects.end(), back_inserter(alive_objects), check_dead);

	for_each(objects.begin(), objects.end(), deleter);
	
	objects = alive_objects;

	if (p1.is_dead()) {
		p1.reset();
		blue_score++;
	}

	if (p2.is_dead()) {
		p2.reset();
		red_score++;
	}

	return NULL;
}

LevelState::~LevelState() {
	for_each(objects.begin(), objects.end(), deleter);
}