#include "level.h"

Level::Level(char* path, std::vector<Image*> images) {
	loadMap(path);
	loadSprites(images);
}

void Level::addPlayer(double x, double y) {
	player = new Player(x,y);
}
void Level::addGoal(double gx, double gy) {
	goalx = gx; goaly = gy;
}

void Level::addPlanet(unsigned int px, unsigned int py, unsigned int mass, unsigned int radius) {
	planets.push_back(new Planet(px, py, mass, radius));
}
void Level::addParticle(unsigned int px, unsigned int py, unsigned int radius, int initial_dx, int initial_dy, bool m) {
	particles.push_back(new Particle(px, py, radius, initial_dx, initial_dy, m));
}

void Level::loadMap(char* path) {
	map = new Map(path);
	addPlayer(map->player[0], map->player[1]);
	addGoal(map->goal[0], map->goal[1]);
	for (int i = 0; i < map->planets.size(); i++) {
		std::vector<int> planet = map->planets[i];
		addPlanet(planet[0], planet[1], planet[2], planet[3]);
	}
	for (int i = 0; i < map->particles.size(); i++) {
		std::vector<int> particle = map->particles[i];
		addParticle(particle[0], particle[1], particle[2], particle[3], particle[4], particle[5]);
	}
}

void Level::loadSprites(std::vector<Image*> images) {

	AddSprite(&sprites, &player->posx, &player->posy, images[PLAYER], PLAYER_RADIUS * 2, PLAYER); //player
	AddSprite(&sprites, &goalx, &goaly, images[GOAL], GOAL_RADIUS * 2, GOAL); //goal sprite

	for (int i = 0; i < planets.size(); ++i) {
		AddSprite(&sprites, &planets[i]->posx, &planets[i]->posy, images[PLANET], planets[i]->r * 2, PLANET); //planets sprite
	}

	for (int i = 0; i < particles.size(); ++i) {
		if (particles[i]->metallic) {
			AddSprite(&sprites, &particles[i]->posx, &particles[i]->posy, images[PARTICLE], particles[i]->r * 2, PARTICLE); //particles sprite
		}
		else {
			AddSprite(&sprites, &particles[i]->posx, &particles[i]->posy, images[ROCK], particles[i]->r * 2, ROCK); //rocks sprite
		}
	}
}

void Level::animateSprites(Image* framebuffer, int startx, int startx_player, int starty_player) {

	for (int i = 0; i < sprites.size(); i++) {
		unsigned int type = sprites[i]->GetType();
		if (type == PLAYER) sprites[i]->SetStart(startx_player, starty_player); else sprites[i]->SetStart(startx, 0);
		sprites[i]->render(framebuffer);
	}
}

//method to detect collision between objects
bool Level::detect_collision(unsigned int px, unsigned int py, unsigned int poffset, unsigned int qx, unsigned int qy, unsigned int qoffset) {
	if ((px + poffset >= qx - qoffset + 2) && (px - poffset + 2 <= qx + qoffset)) {
		if ((py + poffset >= qy - qoffset + 2) && (py - poffset + 2 <= qy + qoffset)) {
			return true;
		}
	}

	return false;
}
//method to move the player inside the level
void Level::player_move(int player_directionx, int player_directiony, double seconds_elapsed) {
	player->move(player_directionx, player_directiony, seconds_elapsed);
}
//method to calculate the gravity pull of the planets to the rest of elements
void Level::planet_gravity(double seconds_elapsed) {
	Vector2 gvector;
	for (int i = 0; i < planets.size(); i++) { //PLAYER GRAVITY PULL
		gvector = planets[i]->gravity(player->posx, player->posy, seconds_elapsed);
		player->dx += gvector.x; player->dy += gvector.y;
	}

	for (int j = 0; j < particles.size(); j++) { //PARTICLES Gravity PULL
		for (int i = 0; i < planets.size(); i++) {
			gvector = planets[i]->gravity(particles[j]->posx, particles[j]->posy, seconds_elapsed);
			particles[j]->redirection(gvector.x, gvector.y);
			if (detect_collision(particles[j]->posx, particles[j]->posy, particles[j]->r, planets[i]->posx, planets[i]->posy, planets[i]->r)) {
				particles[j]->dx = 0; particles[j]->dy = 0;
			}
		}
		particles[j]->move(seconds_elapsed);
	}
}

void Level::particles_push(double seconds_elapsed, bool push) {
	if (push) {
		for (int i = 0; i < particles.size(); i++) { //PLAYER GRAVITY PULL
			particles[i]->push(*player, seconds_elapsed);
		}
	}
}

bool Level::player_status() {
	//check if the player is out of limits
	if (player->out_of_limits()) restart();

	//check if the player hit the goal
	if (detect_collision(player->posx, player->posy, PLAYER_RADIUS, goalx, goaly, GOAL_RADIUS / 2)) return true;

	//check if the player collition with planets
	for (int i = 0; i < planets.size(); i++) {
		if (detect_collision(player->posx, player->posy, PLAYER_RADIUS, planets[i]->posx, planets[i]->posy, planets[i]->r)) restart();
	}

	//check if the player collition with particles
	for (int i = 0; i < particles.size(); i++) {
		if (detect_collision(player->posx, player->posy, PLAYER_RADIUS, particles[i]->posx, particles[i]->posy, particles[i]->r)) restart();
	}

	return false;
}

void Level::restart() {
	Game::instance->synth.playSample("data/sounds/hit_sound.wav", 1); //play hit sound

	player->respawn(); //reset player

	for (int i = 0; i < map->planets.size(); i++) { //reset planets
		std::vector<int> planet = map->planets[i];
		planets[i]->posx = planet[0]; planets[i]->posy = planet[1]; planets[i]->m = planet[2]; planets[i]->r = planet[3];
	}
	for (int i = 0; i < map->particles.size(); i++) { //reset particles
		std::vector<int> particle = map->particles[i];
		particles[i]->posx = particle[0]; particles[i]->posy = particle[1]; particles[i]->r = particle[2];
		particles[i]->dx = particle[3]; particles[i]->dy = particle[4]; particles[i]->metallic = particle[5];
	}
}

Level* GetLevel(std::vector<Level*> levels, LEVEL_ID id) {
	return levels[(int)id];
}

void SetLevel(LEVEL_ID id, LEVEL_ID* currentLevel) {
	*currentLevel = id;
}