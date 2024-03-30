#pragma once

#include "image.h"

//TYPES OF SPRITE
#define PLAYER 0
#define GOAL 1
#define PLANET 2
#define PARTICLE 3
#define ROCK 4

class Sprite
{
public:
	double* x; double* y;
	Image* im; unsigned int size; unsigned int startx; unsigned int starty; unsigned int type;

	//constructors
	Sprite(double* posx, double* posy, Image* image, unsigned int s, unsigned int t);

	//methods
	void render(Image* framebuffer);
	void SetPos(unsigned int posx, unsigned int posy);
	void SetStart(unsigned int startx, unsigned int starty);
	int GetType();
};

void AddSprite(std::vector<Sprite*>* stages, double* posx, double* posy, Image* image, unsigned int s, unsigned int t);