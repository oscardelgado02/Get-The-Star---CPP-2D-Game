#include "sprite.h"

Sprite::Sprite(double* posx, double* posy, Image* image, unsigned int s, unsigned int t) {
	x = posx; y = posy; im = image; size = s; startx = 0; starty = 0; type = t;
}

void Sprite::render(Image* framebuffer) {
	framebuffer->drawImage(*im, *x - (size/2), *y - (size / 2), Area(startx*size, starty*size, size, size));
}

void Sprite::SetPos(unsigned int posx, unsigned int posy) {
	*x = posx; *y = posy;
}

void Sprite::SetStart(unsigned int sx, unsigned int sy) {
	startx = sx; starty = sy;
}

int Sprite::GetType() {
	return type;
}

void AddSprite(std::vector<Sprite*>* sprites, double* posx, double* posy, Image* image, unsigned int s, unsigned int t) {
	sprites->push_back(new Sprite(posx, posy, image, s, t));
}