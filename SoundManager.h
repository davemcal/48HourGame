#ifndef SOUND_MANAGER
#define SOUND_MANAGER

#include "SDL/Include/SDL.h"
#include "SDL/Include/SDL_mixer.h"

#include <map>
#include <string>

class SoundManager {

public:
	static Mix_Chunk *get(std::string file);
	static int play(std::string file, bool loop);
	static void stop(int chan);
private:
	static std::map<std::string, Mix_Chunk*> sounds;

};

#endif