#include "planet.h"
#include <math.h>

Planet::Planet(unsigned int px, unsigned int py, unsigned int mass, unsigned int radius) {
	posx = px; posy = py;
	g = 6.67; m = mass; r = radius;
}

Vector2 Planet::gravity(double px, double py, double seconds_elapsed) {

	double dt = seconds_elapsed;

	double distx = posx - px; double disty = posy - py;

	double r = sqrt(pow(distx, 2) + pow(disty, 2));

	double inverse_distance = 1.0 / r;

	double normalized_x = inverse_distance * distx;
	double normalized_y = inverse_distance * disty;

	Vector2 vector;
	vector.x = dt * normalized_x * g * m * pow(inverse_distance, 2); vector.y = dt * normalized_y * g * m * pow(inverse_distance, 2);

	return vector;
}

/*void Planet::reverted_gravity(Player* p) {

	double distx = posx - p->posx; double disty = posy - p->posy;

	double r = sqrt(pow(distx, 2) + pow(disty, 2));

	double inverse_distance = 1.0 / r;

	double normalized_x = inverse_distance * distx;
	double normalized_y = inverse_distance * disty;

	p->dx -= normalized_x * g * m * pow(inverse_distance, 2);
	p->dy -= normalized_y * g * m * pow(inverse_distance, 2);
}
*/