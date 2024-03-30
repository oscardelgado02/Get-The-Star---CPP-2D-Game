#pragma once

#include "utils.h"

class Map
{
public:
	std::vector<int> player;
	std::vector<std::vector<int>> planets;
	std::vector<std::vector<int>> particles;
	std::vector<int> goal;

	//constructor
	Map(char* path);

	//methods
	void Map::addPlayer(int x, int y);
	void Map::addGoal(int gx, int gy);
	void Map::addPlanet(unsigned int px, unsigned int py, unsigned int mass, unsigned int radius);
	void Map::addParticle(unsigned int px, unsigned int py, unsigned int radius, int initial_dx, int initial_dy, int m);
	void loadMap(char* path);
};

