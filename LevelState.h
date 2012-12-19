#ifndef LEVELSTATE
#define LEVELSTATE

#include "SDL/Include/SDL.h"
#include "SDL/Include/SDL_opengl.h"

#include "GameState.h"
#include "GameObject.h"
#include "Map.h"
#include "Player.h"

#include <string>

class LevelState : public GameState {

	static const int SPAWN_TIME = 1000;
	static const int SPAWN_CHANCE = 5;

	int SCREEN_WIDTH, SCREEN_HEIGHT;

	Map m;
	Player p1, p2;

	list<GameObject*> objects;

	int spawn_ticks;

	int red_score;
	int blue_score;

	GLuint blue_gfx, red_gfx;

	int music;

public:
	LevelState(std::string map_file_name, int width, int height);
	void display();
	GameState *update(Uint32 time_dif, bool * prevInput, bool * curInput);
	~LevelState();
};

#endif