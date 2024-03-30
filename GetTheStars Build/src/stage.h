#pragma once
#include "image.h"
#include "level.h"
#include "synth.h"

//images variables
#define FONT 0
#define MINIFONT 1
#define KEYS 2
#define PLAYER_IMAGE 3
#define GOAL_IMAGE 4
#define PLANET_IMAGE 5
#define PARTICLE_IMAGE 6
#define ROCK_IMAGE 7
#define TUTO_BACKGROUND 8
#define FINAL_BACKGROUND 9

//key variables
#define LEFT_KEY_IMAGE 0
#define UP_KEY_IMAGE 1
#define DOWN_KEY_IMAGE 2
#define RIGHT_KEY_IMAGE 3
#define Z_KEY_IMAGE 4
#define X_KEY_IMAGE 5
#define NUMBER_KEY_IMAGE 6

enum class STAGE_ID {
	INTRO = 0,
	TUTORIAL1 = 1,
	TUTORIAL2 = 2,
	PLAY = 3,
	END = 4
};

class Stage
{
public:
	//fonts variables
	Image* font; Image* minifont;

	//constructors
	Stage();
	Stage(Image* font, Image* minifont);

	//methods
	virtual STAGE_ID GetId() = 0;
	virtual void Render(Image* framebuffer, float time, float anim_speed, int startx_player, int starty_player) = 0;
	virtual void Update(float seconds_elapsed, int player_directionx, int player_directiony, bool push, STAGE_ID* currentStage, bool next) = 0;
};

void InitStages(std::vector<Stage*>* stages, std::vector<Level*>* levels, std::vector<Image*>* images);
Stage* GetStage(std::vector<Stage*> stages, STAGE_ID id);
void SetStage(STAGE_ID id, STAGE_ID* currentStage);

class IntroStage : public Stage {
public:
	//constructor
	IntroStage(Image* font, Image* minifont);

	//methods
	STAGE_ID GetId();
	void Render(Image* framebuffer, float time, float anim_speed, int startx_player, int starty_player);
	void Update(float seconds_elapsed, int player_directionx, int player_directiony, bool push, STAGE_ID* currentStage, bool next);
};

class Tutorial1Stage : public Stage {
public:
	//tutorial image variable
	Sprite* keys; Sprite* player; Image* tutorial_background;

	//constructor
	Tutorial1Stage(Image* font, Image* minifont, Image* keys, Image* player,  Image* tutorial_background);

	//methods
	STAGE_ID GetId();
	void Render(Image* framebuffer, float time, float anim_speed, int startx_player, int starty_player);
	void Update(float seconds_elapsed, int player_directionx, int player_directiony, bool push, STAGE_ID* currentStage, bool next);
};

class Tutorial2Stage : public Stage {
public:
	//tutorial image variable
	Sprite* goal; Sprite* planet; Sprite* particle; Sprite* rock; Image* tutorial_background;

	//constructor
	Tutorial2Stage(Image* font, Image* minifont, Image* goal, Image* planet, Image* particle, Image* rock, Image* tutorial_background);

	//methods
	STAGE_ID GetId();
	void Render(Image* framebuffer, float time, float anim_speed, int startx_player, int starty_player);
	void Update(float seconds_elapsed, int player_directionx, int player_directiony, bool push, STAGE_ID* currentStage, bool next);
};

class PlayStage : public Stage {
public:
	//levels variables
	std::vector<Level*>* levels;
	LEVEL_ID currentLevel = LEVEL_ID::LEVEL1;

	//constructor
	PlayStage(std::vector<Level*>* levels);

	//methods
	STAGE_ID GetId();
	void Render(Image* framebuffer, float time, float anim_speed, int startx_player, int starty_player);
	void Update(float seconds_elapsed, int player_directionx, int player_directiony, bool push, STAGE_ID* currentStage, bool next);
};

class EndStage : public Stage {
public:
	//background image
	Image* final_background;

	//constructor
	EndStage(Image* font, Image* minifont, Image* final_background);

	//methods
	STAGE_ID GetId();
	void Render(Image* framebuffer, float time, float anim_speed, int startx_player, int starty_player);
	void Update(float seconds_elapsed, int player_directionx, int player_directiony, bool push, STAGE_ID* currentStage, bool next);
};