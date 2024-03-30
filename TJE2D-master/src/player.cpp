#include "player.h"

Player::Player() {
	iposx = 0; iposy = 0;
	posx = 0; posy = 0;
	v = 25.0; a = 2.4;
	dx = 0; dy = 0; ax = 0; ay = 0;
}

Player::Player(unsigned int x, unsigned int y) {
	iposx = x; iposy = y;
	posx = x; posy = y;
	v = 25.0; a = 5.0;
	dx = 0; dy = 0; ax = 0; ay = 0;
}

void Player::move(unsigned int directionx, unsigned int directiony, double seconds_elapsed) {

	double dt = seconds_elapsed;

	if (directionx == RIGHT) ax = 1.0f;
	if (directionx == LEFT) ax = -1.0f;
	if (directiony == UP) ay = -1.0f;
	if (directiony == DOWN) ay = 1.0f;

	ax *= v; ay *= v;

	stop_move(directionx, directiony); //Stop completely

	posx += dt * dx + dt * dt * 0.5f * ax;
	posy += dt * dy + dt * dt * 0.5f * ay;

	dx += dt * ax; dy += dt * ay;

	ax = 0.0f;
	ay = 0.0f;
}

bool Player::out_of_limits() {
	//MAP LIMIT
	bool output = false;

	if (posx - PLAYER_RADIUS < 0) {
		output = true;
	}
	else if (posx + PLAYER_RADIUS > 160) {
		output = true;
	}

	//MAP LIMIT
	if (posy - PLAYER_RADIUS < 0) {
		output = true;
	}
	else if (posy + PLAYER_RADIUS > 120) {
		output = true;
	}

	return output;
}

void Player::stop_move(unsigned int directionx, unsigned int directiony) {
	if (directionx == STOP || directiony == STOP) {
		ax -= a*dx; ay -= a * dy;
	}
}

void Player::respawn() {
	posx = iposx; posy = iposy;
	dx = 0; dy = 0; ax = 0; ay = 0;
}