#ifndef TEXTURE_MANAGER
#define TEXTURE_MANAGER

#include "SDL/Include/SDL.h"
#include "SDL/Include/SDL_opengl.h"
#include "SDL/Include/SDL_image.h"

#include <map>
#include <string>

class TextureManager {

public:
	static GLuint load(std::string file);
	static GLuint get(std::string file);
	static void displayText(std::string text, int x, int y, int w, int h);

private:
	static void displayChar(char c);

	static std::map<std::string, GLuint> textures;
};

#endif