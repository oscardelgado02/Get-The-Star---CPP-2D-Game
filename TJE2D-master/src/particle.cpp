#include "particle.h"
#include "math.h"

Particle::Particle(unsigned int px, unsigned int py, unsigned int radius, int initial_dx, int initial_dy, bool m) {
	posx = px; posy = py; r = radius; dx = initial_dx; dy = initial_dy; metallic = m;
}

void Particle::move(double seconds_elapsed) {
	double dt = seconds_elapsed;

	posx += dt * dx;
	posy += dt * dy;
}

void Particle::redirection(double dirx, double diry) {

	dx += dirx; dy += diry;
}

void Particle::push(Player p, double seconds_elapsed) {

	if(metallic){
		double dt = seconds_elapsed;

		double distx = p.posx - posx; double disty = p.posy - posy;

		double r = sqrt(pow(distx, 2) + pow(disty, 2));

		double inverse_distance = 1.0 / r;

		double normalized_x = inverse_distance * distx;
		double normalized_y = inverse_distance * disty;

		dx -= dt*normalized_x * 12000 * pow(inverse_distance, 2);
		dy -= dt*normalized_y * 12000 * pow(inverse_distance, 2);
	}
	
}