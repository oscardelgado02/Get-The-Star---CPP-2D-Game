#pragma once

#include "player.h"
#include "planet.h"
#include "particle.h"
#include "utils.h"
#include "sprite.h"
#include "map.h"
#include "synth.h"
#include "game.h"

#define GOAL_RADIUS 16

enum class LEVEL_ID {
	LEVEL1 = 0,
	LEVEL2 = 1,
	LEVEL3 = 2,
	LEVEL4 = 3
};

class Level
{
public:
	Player* player; std::vector<Planet*> planets; std::vector<Particle*> particles; double goalx; double goaly;
	std::vector<Sprite*> sprites; Map* map;

	//constructor
	Level(char* path, std::vector<Image*> images);

	//methods
	void addPlayer(double x, double y);
	void addGoal(double gx, double gy);
	void addPlanet(unsigned int px, unsigned int py, unsigned int mass, unsigned int radius);
	void addParticle(unsigned int px, unsigned int py, unsigned int radius, int initial_dx, int initial_dy, bool m);
	void loadMap(char* path);
	void loadSprites(std::vector<Image*> images);
	void animateSprites(Image* framebuffer, int startx, int startx_player, int starty_player);
	bool detect_collision(unsigned int px, unsigned int py, unsigned int poffset, unsigned int qx, unsigned int qy, unsigned int qoffset);
	void player_move(int player_directionx, int player_directiony, double seconds_elapsed);
	void planet_gravity(double seconds_elapsed);
	void particles_push(double seconds_elapsed, bool push);
	bool player_status();
	void restart();
};

Level* GetLevel(std::vector<Level*> levels, LEVEL_ID id);
void SetLevel(LEVEL_ID id, LEVEL_ID* currentLevel);

/*
*GAMEMAP loaded by files. Files are line by line with the info of all the objects of the scene:
* PLAYER [TYPE 0] INFO: TYPE:INT[0] POSX:INT[1] POSY:INT[2]
* GOAL [TYPE 1] INFO: TYPE:INT[0] POSX:INT[1] POSY:INT[2]
* PLANET [TYPE 2] INFO: TYPE:INT[0] POSX:INT[1] POSY:INT[2] MASS:INT[3] RADIUS:INT[4]
* PARTICLE [TYPE 3] INFO: TYPE:INT[0] POSX:INT[1] POSY:INT[2] RADIUS:INT[3] INITIAL_DX:INT[4] INITIAL_DY:INT[5] METALLIC:INT[6]
*/
