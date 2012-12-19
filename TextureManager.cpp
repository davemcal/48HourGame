#include "TextureManager.h"
#include "SDL/Include/SDL_opengl.h"
#include "GL/glut.h"
#include "Common.h"

using namespace std;

map<std::string, GLuint> TextureManager::textures;

GLuint TextureManager::load(string file) {
	if (textures.find(file) != textures.end()) {
		return textures[file];
	}

	SDL_Surface *surface;	// This surface will tell us the details of the image
	GLenum texture_format;
	GLint  nOfColors;
	GLuint texture;
	 
//	if ( (surface = IMG_LoadPNG_RW(SDL_RWFromFile(file.c_str(), "rb"))) ) { 
	if ( surface = IMG_Load(file.c_str()) ) { 	 
		// get the number of channels in the SDL surface
		nOfColors = surface->format->BytesPerPixel;
		if (nOfColors == 4)  {   // contains an alpha channel
			if (surface->format->Rmask == 0x000000ff)
					texture_format = GL_RGBA;
			else
					texture_format = GL_BGRA;
		} else if (nOfColors == 3)  {   // no alpha channel
			if (surface->format->Rmask == 0x000000ff)
				texture_format = GL_RGB;
			else
				texture_format = GL_BGR;
		} else {
			return -1;
		}
	        
		// Have OpenGL generate a texture object handle for us
		glGenTextures( 1, &texture );
	 
		// Bind the texture object
		glBindTexture( GL_TEXTURE_2D, texture );
	 
		// Set the texture's stretching properties
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
		

 
		// Edit the texture object's image data using the information SDL_Surface gives us
		glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
						  texture_format, GL_UNSIGNED_BYTE, surface->pixels );
		//gluBuild2DMipmaps(	GL_TEXTURE_2D, 4, surface->w, surface->h, texture_format, GL_UNSIGNED_BYTE, surface->pixels );
	} 
	else {
		printf("IMG_Load: %s\n", IMG_GetError());
		return -1;
	}    
	 
	// Free the SDL_Surface only if it was successfully created
	if ( surface ) { 
		SDL_FreeSurface( surface );
	}

	textures[file] = texture;

	return texture;
}

GLuint TextureManager::get(string file) {

	if (textures.find(file) == textures.end()) {
		return load(file);
	}

	return textures[file];
}

void TextureManager::displayText(string text, int x, int y, int w, int h) {

	glPushMatrix();

	glTranslatef(float(x), float(y), 0.0f);
	glScalef(float(w), float(h), 1.0f);

	for (int i = 0; i < text.size(); i++) {
		displayChar(text[i]);
		glTranslatef(1.0f, 0.0f, 0.0f);
	}

	glPopMatrix();
}

void TextureManager::displayChar(char c) {
	rect<float> tex;
	tex.y = 0.0f;
	tex.h = 1.0f;

	if (c - '0' >= 0 && c - '0' < 10) {
		glBindTexture( GL_TEXTURE_2D, TextureManager::get("numbers.png") );
		tex.x = float(c - '0') / 10.0f;
		tex.w = 1.0f/ 10.0f;
	}
	
	glBegin( GL_QUADS );
	glTexCoord2f( tex.x, tex.y );
	glVertex3f( 0.0f, 0.0f, 0.0f );
	
	//Bottom-left vertex (corner)
	glTexCoord2f( tex.x + tex.w, tex.y );
	glVertex3f( 1.0f, 0.0f, 0.0f );
	
	//Bottom-right vertex (corner)
	glTexCoord2f( tex.x + tex.w, tex.y + tex.h );
	glVertex3f( 1.0f, 1.0f, 0.0f );
	
	//Top-right vertex (corner)
	glTexCoord2f( tex.x, tex.y + tex.h );
	glVertex3f( 0.0f, 1.0f, 0.0f );

	glEnd();
}