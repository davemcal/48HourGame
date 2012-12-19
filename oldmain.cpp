#include "SDL/Include/SDL.h"
#include "SDL/Include/SDL_opengl.h"
#include "SDL/Include/SDL_mixer.h"

#include "Map.h"
#include "Player.h"
#include "GameObject.h"
#include "PowerUp.h"

#include <list>

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

const int SCREEN_WIDTH = 75*16;
const int SCREEN_HEIGHT = 60*16;

const char * GAME_NAME = "48 Hour Game";

bool check_dead(GameObject *a);
void deleter(GameObject *a);

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

		a->draw();
	}
};

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

int main(int argc, char **argv) {
	//Start SDL
    SDL_Init( SDL_INIT_EVERYTHING );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_Surface *screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_OPENGL);//|SDL_FULLSCREEN );

	int audio_rate = 22050;
	Uint16 audio_format = AUDIO_S16SYS;
	int audio_channels = 2;
	int audio_buffers = 4096;
	 
	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
		cout << "Unable to initialize audio: " << Mix_GetError() << endl;
		exit(1);
	}

	glEnable( GL_TEXTURE_2D );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GREATER, 0.9f);
 
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT );
	 
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	 
	glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1.0f, 1.0f);
		
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	SDL_WM_SetCaption( GAME_NAME, NULL );

	bool running = true;

	Map m(ifstream("clouds.txt"));

	Player p1(ifstream("red_player.txt"), m, 0);
	p1.set_tile_pos(4, 8);

	Player p2(ifstream("blue_player.txt"), m, 1);
	p2.set_tile_pos(12, 8);
	p2.set_input(SDLK_a, SDLK_d, SDLK_w, SDLK_s, SDLK_g, SDLK_h, SDLK_j);

	Uint32 lastTick;
	Uint32 curTick = SDL_GetTicks();

	bool prevInput[SDLK_LAST];
	bool curInput[SDLK_LAST];

	fill(curInput, curInput + SDLK_LAST, false);

	list<GameObject*> objects;

	PowerUp::gfx = TextureManager::load("power_down_set.png");
	PowerUp::players[0] = &p1;
	PowerUp::players[1] = &p2;

	static const int SPAWN_TIME = 1000;
	static const int SPAWN_CHANCE = 5;
	int spawn_ticks = SPAWN_TIME;

	int red_score = 0;
	int blue_score = 0;

	while (running) {
		lastTick = curTick;
		curTick = SDL_GetTicks();

		copy(curInput, curInput + SDLK_LAST, prevInput);

		SDL_Event event;

		while( SDL_PollEvent( &event ) ) {
			
			if( event.type == SDL_QUIT )
			{
				running = false;
			}  
			//Key Releases
			if ( event.type == SDL_KEYUP) {
				curInput[event.key.keysym.sym] = false;
			}
			//Key Presses
			if ( event.type == SDL_KEYDOWN) {
				curInput[event.key.keysym.sym] = true;
			}
		}
		
		vector<GameObject*> new_objects = p1.update(curTick - lastTick, prevInput, curInput);

		objects.insert(objects.end(), new_objects.begin(), new_objects.end());

		new_objects = p2.update(curTick - lastTick, prevInput, curInput);

		objects.insert(objects.end(), new_objects.begin(), new_objects.end());

		spawn_ticks -= curTick - lastTick;
		if (spawn_ticks < 0) {
			spawn_ticks = SPAWN_TIME;
			if (rand() % SPAWN_CHANCE == 0) {
				objects.push_back(m.spawn_powerup());
			}
		}

		glClear( GL_COLOR_BUFFER_BIT );

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		m.draw();
		p1.draw();
		p2.draw();
		
		updater u(curTick - lastTick, prevInput, curInput);
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
		glTranslatef(SCREEN_WIDTH - 40.0f * Player::MAX_HEALTH - 20.0f, 0.0f, 0.0f);

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

		stringstream o;
		o << red_score;

		glColor3f(1.0f, 0.0f, 0.0f);
		TextureManager::displayText(o.str(), SCREEN_WIDTH / 2 - 75 - o.str().size() * 25 / 2, 10, 25, 40);

		stringstream p;
		p << blue_score;

		glColor3f(0.0f, 0.0f, 1.0f);
		TextureManager::displayText(p.str(), SCREEN_WIDTH / 2 + 75 - p.str().size() * 25 / 2, 10, 25, 40);
		
		glColor3f(1.0f, 1.0f, 1.0f);
		SDL_GL_SwapBuffers();

		//Try to cap fps at 60
		SDL_Delay((1000/60) > (curTick - lastTick) ? 1000 / 60 - (curTick - lastTick) : 0);
	}

	return 0;
}

bool check_dead(GameObject *a) {
	return a->is_dead();
}

void deleter(GameObject *a) {
	if (a->is_dead()) 
		delete a;
}