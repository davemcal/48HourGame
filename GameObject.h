#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Common.h"
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

class GameObject {

public:
	virtual rect<int> get_bounds() = 0;
	virtual void draw() = 0;
	virtual void die(int damage) = 0;
	virtual void collide(int pnum) { die(10); }
	virtual bool is_dead() = 0;
	virtual int get_damage() = 0;
	virtual list<GameObject*> update(Uint32 tick_change, bool *prevInput, bool *curInput) = 0;
	virtual ~GameObject() { };
};

#endif