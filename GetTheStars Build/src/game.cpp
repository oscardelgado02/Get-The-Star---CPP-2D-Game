#include "game.h"
#include "utils.h"
#include "input.h"
#include "image.h"
#include "player.h"
#include "planet.h"
#include "particle.h"
#include "framework.h"
#include "stage.h"
#include "sprite.h"
#include "level.h"

#include <cmath>

Game* Game::instance = NULL;

//images variables
Image font;
Image minifont;
Image background_image;
Image player_image;
Image goal_image;
Image redplanet_image;
Image particle_image;
Image rock_image;
Image keys;
Image tutorial_background;
Image final_background;

//images vectors variable
std::vector<Image*> images;
std::vector<Image*> images2;

//state variables
bool push; unsigned int player_directionx; unsigned int player_directiony; bool next;

//stages variables
std::vector<Stage*> stages;
STAGE_ID currentStage = STAGE_ID::INTRO;

//levels variables
Level* level1;
Level* level2;
Level* level3;
Level* level4;

//levels vector variable
std::vector<Level*> levels;

//animation variables
const float anim_speed = 3.0f;
int startx_player = NOMOVE; int starty_player = NOPUSH;

Game::Game(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;

	//Images initialization
	font.loadTGA("data/bitmap-font-white.tga"); //load bitmap-font image
	minifont.loadTGA("data/mini-font-white-4x6.tga"); //load bitmap-font image
	background_image.loadTGA("data/space_background.tga"); //background image
	player_image.loadTGA("data/spaceship.tga"); //player image
	goal_image.loadTGA("data/goal.tga"); //goal image
	redplanet_image.loadTGA("data/red_planet.tga"); //redplanet image
	particle_image.loadTGA("data/particle.tga"); //particle image
	rock_image.loadTGA("data/rock.tga"); //rock image
	keys.loadTGA("data/keys.tga"); //tutorial1 image
	tutorial_background.loadTGA("data/tutorial_background.tga"); //tutorial background
	final_background.loadTGA("data/final_background.tga"); //final background

	//Image Vector initialization
	images.push_back(&player_image);
	images.push_back(&goal_image);
	images.push_back(&redplanet_image);
	images.push_back(&particle_image);
	images.push_back(&rock_image);

	//Image Vector 2 initialization
	images2.push_back(&font);
	images2.push_back(&minifont);
	images2.push_back(&keys);
	images2.push_back(&player_image);
	images2.push_back(&goal_image);
	images2.push_back(&redplanet_image);
	images2.push_back(&particle_image);
	images2.push_back(&rock_image);
	images2.push_back(&tutorial_background);
	images2.push_back(&final_background);

	//Levels initialization
	level1 = new Level("data/levels/level1.txt", images);
	level2 = new Level("data/levels/level2.txt", images);
	level3 = new Level("data/levels/level3.txt", images);
	level4 = new Level("data/levels/level4.txt", images);

	//Levels Vector initialization
	levels.push_back(level1);
	levels.push_back(level2);
	levels.push_back(level3);
	levels.push_back(level4);

	//Stages initialization
	InitStages(&stages, &levels, &images2);

	//State variables initialization
	player_directionx = FREE;
	player_directiony = FREE;
	push = false;
	next = false;

	enableAudio(); //enable this line if you plan to add audio to your application
	synth.playSample("data/sounds/ambient.wav",0.5,true);
	//synth.osc1.amplitude = 0.5;
}

//what to do when the image has to be draw
void Game::render(void)
{
	//Create a new Image (or we could create a global one if we want to keep the previous frame)
	Image framebuffer(160, 120); //do not change framebuffer size

	//some new useful functions
		framebuffer.drawImage( background_image, 0, 0, framebuffer.width, framebuffer.height );

		//STAGE RENDER
		GetStage(stages, currentStage)->Render(&framebuffer, time, anim_speed, startx_player, starty_player);

	//send image to screen
	showFramebuffer(&framebuffer);
}

void Game::update(double seconds_elapsed)
{
	player_directionx = FREE; player_directiony = FREE; push = false; next = false; Vector2 gvector;
	startx_player = NOMOVE; starty_player = NOPUSH;

	//Read the keyboard state, to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) //if key right
	{
		player_directionx = RIGHT;
		startx_player = EAST;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) //if key left
	{
		player_directionx = LEFT;
		startx_player = WEST;
	}

	if (Input::isKeyPressed(SDL_SCANCODE_UP)) //if key up
	{
		player_directiony = UP;
		if (startx_player == NOMOVE) { startx_player = NORTH; } else if (startx_player == EAST) { startx_player = NORTH_EAST; }
		else if (startx_player == WEST) { startx_player = NORTH_WEST; }
	}
	if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) //if key down
	{
		player_directiony = DOWN;
		if (startx_player == NOMOVE) { startx_player = SOUTH; } else if (startx_player == EAST) { startx_player = SOUTH_EAST; }
		else if (startx_player == WEST) { startx_player = SOUTH_WEST; }
	}

	//STOP KEY
	if (Input::isKeyPressed(SDL_SCANCODE_X)) //if key X
	{
		player_directionx = STOP; player_directiony = STOP;
		startx_player = NOMOVE;
	}

	//REVERSE KEY
	if (Input::isKeyPressed(SDL_SCANCODE_Z)) //if key Z
	{
		push = true;
		starty_player = PUSH;
	}

	//NEXT LEVEL KEY (DEBUG KEY)
	if (Input::wasKeyPressed(SDL_SCANCODE_K)) //if key K
	{
		next = true;
	}

	//to read the gamepad state
	if (Input::gamepads[0].isButtonPressed(A_BUTTON)) //if the A button is pressed
	{
	}

	if (Input::gamepads[0].direction & PAD_UP) //left stick pointing up
	{
		
	}

	//STAGE UPDATE
	GetStage(stages, currentStage)->Update(seconds_elapsed, player_directionx, player_directiony, push, &currentStage, next);
	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)) { //used to skip stages
		//synth.playSample("data/sounds/goal_sound.wav", 1);
		int nextStageIndex = (((int)currentStage) + 1);
		if (nextStageIndex >= stages.size()) must_exit = true; else SetStage((STAGE_ID)nextStageIndex, &currentStage);
	}
}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onMouseMove(SDL_MouseMotionEvent event)
{
}

void Game::onMouseButtonDown( SDL_MouseButtonEvent event )
{
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
}

void Game::onResize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport( 0,0, width, height );
	window_width = width;
	window_height = height;
}

//sends the image to the framebuffer of the GPU
void Game::showFramebuffer(Image* img)
{
	static GLuint texture_id = -1;
	static GLuint shader_id = -1;
	if (!texture_id)
		glGenTextures(1, &texture_id);

	//upload as texture
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);

	glDisable(GL_CULL_FACE); glDisable(GL_DEPTH_TEST); glEnable(GL_TEXTURE_2D);
	float startx = -1.0; float starty = -1.0;
	float width = 2.0; float height = 2.0;

	//center in window
	float real_aspect = window_width / (float)window_height;
	float desired_aspect = img->width / (float)img->height;
	float diff = desired_aspect / real_aspect;
	width *= diff;
	startx = -diff;

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex2f(startx, starty + height);
	glTexCoord2f(1.0, 0.0); glVertex2f(startx + width, starty + height);
	glTexCoord2f(1.0, 1.0); glVertex2f(startx + width, starty);
	glTexCoord2f(0.0, 1.0); glVertex2f(startx, starty);
	glEnd();

	/* this version resizes the image which is slower
	Image resized = *img;
	//resized.quantize(1); //change this line to have a more retro look
	resized.scale(window_width, window_height);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	if (1) //flip
	{
	glRasterPos2f(-1, 1);
	glPixelZoom(1, -1);
	}
	glDrawPixels( resized.width, resized.height, GL_RGBA, GL_UNSIGNED_BYTE, resized.pixels );
	*/
}

//AUDIO STUFF ********************

SDL_AudioSpec audio_spec;

void AudioCallback(void*  userdata,
	Uint8* stream,
	int    len)
{
	static double audio_time = 0;

	memset(stream, 0, len);//clear
	if (!Game::instance)
		return;

	Game::instance->onAudio((float*)stream, len / sizeof(float), audio_time, audio_spec);
	audio_time += len / (double)audio_spec.freq;
}

void Game::enableAudio()
{
	SDL_memset(&audio_spec, 0, sizeof(audio_spec)); /* or SDL_zero(want) */
	audio_spec.freq = 48000;
	audio_spec.format = AUDIO_F32;
	audio_spec.channels = 1;
	audio_spec.samples = 1024;
	audio_spec.callback = AudioCallback; /* you wrote this function elsewhere. */
	if (SDL_OpenAudio(&audio_spec, &audio_spec) < 0) {
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		exit(-1);
	}
	SDL_PauseAudio(0);
}

void Game::onAudio(float *buffer, unsigned int len, double time, SDL_AudioSpec& audio_spec)
{
	//fill the audio buffer using our custom retro synth
	synth.generateAudio(buffer, len, audio_spec);
}
