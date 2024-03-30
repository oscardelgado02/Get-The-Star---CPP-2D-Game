#include "stage.h"

//STAGE

Stage::Stage() {
	this->font = new Image(); //empty
	this->minifont = new Image(); //empty
}

Stage::Stage(Image* font, Image* minifont) {
	this->font = font;
	this->minifont = minifont;
}

//INTRO STAGE

IntroStage::IntroStage(Image* font, Image* minifont) :Stage(font, minifont) {}

STAGE_ID IntroStage::GetId() { return STAGE_ID::INTRO; }

void IntroStage::Render(Image* framebuffer, float time, float anim_speed, int startx_player, int starty_player) {
	framebuffer->drawText("GET THE STARS", 30, 50, *font);
	framebuffer->drawText("PRESS SPACE TO START", 35, 65, *minifont, 4, 6);
}

void IntroStage::Update(float seconds_elapsed, int player_directionx, int player_directiony, bool push, STAGE_ID* currentStage, bool next) {}

//TUTORIAL1 STAGE

Tutorial1Stage::Tutorial1Stage(Image* font, Image* minifont, Image* keys, Image* player, Image* tutorial_background) :Stage(font, minifont) {
	this->keys = new Sprite(new double(50), new double(14), keys, 14, PLAYER);
	this->player = new Sprite(new double(108), new double(30), player, PLAYER_RADIUS*2, PLAYER);
	this->tutorial_background = tutorial_background;
}

STAGE_ID Tutorial1Stage::GetId() { return STAGE_ID::TUTORIAL1; }

void Tutorial1Stage::Render(Image* framebuffer, float time, float anim_speed, int startx_player, int starty_player) {
	framebuffer->drawImage(*tutorial_background, 0, 0);

	framebuffer->drawText("MOVE THE SHIP", 33, 48, *minifont, 4, 6);
	framebuffer->drawText("SLOW DOWN", 95, 88, *minifont, 4, 6);
	framebuffer->drawText("PUSH METAL PARTICLES", 8, 88, *minifont, 4, 6);

	
	for (int i = 0; i < NUMBER_KEY_IMAGE; i++) {
		keys->SetStart(i, 0);
		switch (i) {
			case UP_KEY_IMAGE: keys->SetPos(58, 22); break;
			case LEFT_KEY_IMAGE: keys->SetPos(44, 38); break;
			case DOWN_KEY_IMAGE: keys->SetPos(59, 38); break;
			case RIGHT_KEY_IMAGE: keys->SetPos(74, 38); break;
			case Z_KEY_IMAGE: keys->SetPos(44, 80); break;
			case X_KEY_IMAGE: keys->SetPos(110, 80); break;
		}
		keys->render(framebuffer);
	}
	player->render(framebuffer);
}

void Tutorial1Stage::Update(float seconds_elapsed, int player_directionx, int player_directiony, bool push, STAGE_ID* currentStage, bool next) {}

//TUTORIAL2 STAGE

Tutorial2Stage::Tutorial2Stage(Image* font, Image* minifont, Image* goal, Image* planet, Image* particle, Image* rock, Image* tutorial_background) : Stage(font, minifont) {
	this->goal = new Sprite(new double(25), new double(96), goal, GOAL_RADIUS*2, GOAL);
	this->planet = new Sprite(new double(24), new double(24), planet, 24, PLANET);
	this->particle = new Sprite(new double(25), new double(51), particle, 12, PARTICLE);
	this->rock = new Sprite(new double(25), new double(72), rock, 12, ROCK);
	this->tutorial_background = tutorial_background;
}

STAGE_ID Tutorial2Stage::GetId() { return STAGE_ID::TUTORIAL2; }

void Tutorial2Stage::Render(Image* framebuffer, float time, float anim_speed, int startx_player, int starty_player) {
	framebuffer->drawImage(*tutorial_background, 0, 0);

	framebuffer->drawText("PLANETS: THEY PULL THE SHIP", 40, 18, *minifont, 4, 6);
	framebuffer->drawText("AND PARTICLES WITH GRAVITY", 40, 25, *minifont, 4, 6);

	framebuffer->drawText("METAL PARTICLE: IT CAN BE", 40, 46, *minifont, 4, 6);
	framebuffer->drawText("PUSHED BY THE SHIP POWER", 40, 53, *minifont, 4, 6);

	framebuffer->drawText("ROCK PARTICLE: IT CAN'T BE", 40, 66, *minifont, 4, 6);
	framebuffer->drawText("PUSHED BY THE SHIP POWER", 40, 73, *minifont, 4, 6);

	framebuffer->drawText("STAR: TO WIN THE LEVEL, THE", 40, 90, *minifont, 4, 6);
	framebuffer->drawText("SHIP HAS TO REACH IT", 40, 97, *minifont, 4, 6);

	framebuffer->drawText("DON'T TOUCH THE LIMITS OF THE MAP", 15, 110, *minifont, 4, 6);

	goal->render(framebuffer);
	planet->render(framebuffer);
	particle->render(framebuffer);
	rock->render(framebuffer);
}

void Tutorial2Stage::Update(float seconds_elapsed, int player_directionx, int player_directiony, bool push, STAGE_ID* currentStage, bool next) {}

//PLAY STAGE

PlayStage::PlayStage(std::vector<Level*>* levels) {
	this->levels = levels;
}

STAGE_ID PlayStage::GetId() { return STAGE_ID::PLAY; }

void PlayStage::Render(Image* framebuffer, float time, float anim_speed, int startx_player, int starty_player) {
	//SPRITES ANIMATION
	const int startx = (int(time * anim_speed) % 4);
	GetLevel(*levels, currentLevel)->animateSprites(framebuffer, startx, startx_player, starty_player);
}

void PlayStage::Update(float seconds_elapsed, int player_directionx, int player_directiony, bool push, STAGE_ID* currentStage, bool next) {
	//PLAYER MOVE
	GetLevel(*levels, currentLevel)->player_move(player_directionx, player_directiony, seconds_elapsed);

	//PLANET GRAVITY PULL
	GetLevel(*levels, currentLevel)->planet_gravity(seconds_elapsed);

	//PUSH
	GetLevel(*levels, currentLevel)->particles_push(seconds_elapsed, push);

	//DETECT COLLISIONS AND IF GOAL IS REACHED IT RETURNS TRUE
	if (next || GetLevel(*levels, currentLevel)->player_status()) {
		Game::instance->synth.playSample("data/sounds/goal_sound.wav", 10); //play goal sound
		int nextLevelIndex = (((int)currentLevel) + 1);
		if (nextLevelIndex >= levels->size()) {
			SetStage(STAGE_ID::END, currentStage);
		}
		else {
			SetLevel((LEVEL_ID)nextLevelIndex, &currentLevel);
		}
	};
}

//END STAGE

EndStage::EndStage(Image* font, Image* minifont, Image* final_background) :Stage(font, minifont) {
	this->final_background = final_background;
}

STAGE_ID EndStage::GetId() { return STAGE_ID::END; }

void EndStage::Render(Image* framebuffer, float time, float anim_speed, int startx_player, int starty_player) {
	framebuffer->drawImage(*final_background, 0, 0);
	framebuffer->drawText("THE END", 55, 55, *font);
}

void EndStage::Update(float seconds_elapsed, int player_directionx, int player_directiony, bool push, STAGE_ID* currentStage, bool next) {}

//OTHER METHODS

void InitStages(std::vector<Stage*>* stages, std::vector<Level*>* levels, std::vector<Image*>* images) {
	stages->reserve(6);
	stages->push_back(new IntroStage((*images)[FONT], (*images)[MINIFONT]));
	stages->push_back(new Tutorial1Stage((*images)[FONT], (*images)[MINIFONT], (*images)[KEYS], (*images)[PLAYER_IMAGE], (*images)[TUTO_BACKGROUND]));
	stages->push_back(new Tutorial2Stage((*images)[FONT], (*images)[MINIFONT], (*images)[GOAL_IMAGE], (*images)[PLANET_IMAGE], (*images)[PARTICLE_IMAGE], (*images)[ROCK_IMAGE], (*images)[TUTO_BACKGROUND]));
	stages->push_back(new PlayStage(levels));
	stages->push_back(new EndStage((*images)[FONT], (*images)[MINIFONT], (*images)[FINAL_BACKGROUND]));
}

Stage* GetStage(std::vector<Stage*> stages, STAGE_ID id) {
	return stages[(int)id];
}

void SetStage(STAGE_ID id, STAGE_ID* currentStage) {
	*currentStage = id;
}