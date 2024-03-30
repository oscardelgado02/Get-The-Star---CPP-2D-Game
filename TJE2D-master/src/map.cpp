#include "map.h"
#include "level.h"

Map::Map(char* path) {
	loadMap(path);
}

void Map::addPlayer(int x, int y) {
	player.push_back(x);
	player.push_back(y);
}
void Map::addGoal(int gx, int gy) {
	goal.push_back(gx);
	goal.push_back(gy);
}

void Map::addPlanet(unsigned int px, unsigned int py, unsigned int mass, unsigned int radius) {
	std::vector<int> planet;
	planet.push_back(px);
	planet.push_back(py);
	planet.push_back(mass);
	planet.push_back(radius);

	planets.push_back(planet);
}
void Map::addParticle(unsigned int px, unsigned int py, unsigned int radius, int initial_dx, int initial_dy, int m) {
	std::vector<int> particle;
	particle.push_back(px);
	particle.push_back(py);
	particle.push_back(radius);
	particle.push_back(initial_dx);
	particle.push_back(initial_dy);
	particle.push_back(m);

	particles.push_back(particle);
}

void Map::loadMap(char* path) {
	std::string map_content;
	readFile(path, map_content);

	std::vector<std::string> lines = split(map_content, '\n');

	for (int i = 0; i < lines.size(); ++i) {
		std::vector<std::string> line = split(lines[i], ' ');
		std::vector<int> values;

		for (int j = 0; j < line.size(); ++j) {
			values.push_back(stoi(line[j]));
		}

		int type = values[0];
		switch (type) {
		case PLAYER: addPlayer(values[1], values[2]); break;
		case GOAL: addGoal(values[1], values[2]); break;
		case PLANET: addPlanet(values[1], values[2], values[3], values[4]); break;
		case PARTICLE: addParticle(values[1], values[2], values[3], values[4], values[5], values[6]); break;
		}
	}
}