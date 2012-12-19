#include "SoundManager.h"
#include <iostream>

using namespace std;

map<string, Mix_Chunk* > SoundManager::sounds;

Mix_Chunk *SoundManager::get(string file) {

	if (sounds.find(file) != sounds.end())
		return sounds[file];
	

	Mix_Chunk *sound = NULL;
 
	sound = Mix_LoadWAV(file.c_str());
	if(sound == NULL) {
		cout << "Unable to load WAV file: " << Mix_GetError() << endl;
	}

	sounds[file] = sound;

	return sound;
}

int SoundManager::play(string file, bool loop) { 
	int channel;
	 
	channel = Mix_PlayChannel(-1, SoundManager::get(file), (loop ? -1 : 0) );
	if(channel == -1) {
		cout << "Unable to play WAV file: " << Mix_GetError() << endl;
	}

	return channel;
}

void SoundManager::stop(int chan) {
	Mix_Pause(chan);

}