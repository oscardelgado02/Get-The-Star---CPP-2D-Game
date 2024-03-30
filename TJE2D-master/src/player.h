#pragma once

#define RIGHT 0
#define LEFT 1
#define UP 0
#define DOWN 1
#define STOP 2
#define FREE 3
#define PLAYER_RADIUS 8

class Player
{
public:
	double iposx; double iposy; //initial position
	double posx; double posy; //current position
	double v; double a; //velocity values
	double dx; double dy; double ax; double ay; //directional velocity/acceleration

	// CONSTRUCTORS 
	Player();
	Player(unsigned int x, unsigned int y);

	//methods
	void move(unsigned int directionx, unsigned int directiony, double seconds_elapsed);
	bool out_of_limits();
	void stop_move(unsigned int directionx, unsigned int directiony);
	void respawn();
};