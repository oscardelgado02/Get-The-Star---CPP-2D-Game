#pragma once

#include "player.h"
#include "framework.h"

class Planet
{
public:
	double posx; double posy;
	double g; double m; unsigned int r;

	// CONSTRUCTORS 
	Planet(unsigned int px, unsigned int py, unsigned int mass, unsigned int radius);

	//methods
	Vector2 gravity(double px, double py, double seconds_elapsed);
	//void reverted_gravity(Player* p);
};

