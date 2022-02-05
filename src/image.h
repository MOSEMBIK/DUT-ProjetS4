#ifndef __IMAGE_H__
#define __IMAGE_H__
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
using std::string;

class Image {
public:
	Image(string filename);
	~Image();

	void draw();

private:
	GLuint texture;
	SDL_Surface * surface;
};


#endif // __IMAGE_H__
