#pragma once

#include "planet.h"
#include "player.h"

class Particle
{
public:
	double posx; double posy; unsigned int r; double dx; double dy; bool metallic;

	//constructor
	Particle(unsigned int px, unsigned int py, unsigned int radius, int initial_dx, int initial_dy, bool m);

	//methods
	void move(double seconds_elapsed);
	void redirection(double dirx, double diry);
	void push(Player p, double seconds_elapsed);
};