#include "../../includes/engine.h"

void starHandler(entity* this);

void starInit() {
	for (int i = 0; i < 512; i++) {
		createEntity("Star", (SDL_Rect){rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT, 4, 4} , 0, 0, 1, 0, getTexture("Star"), 0, *starHandler);
	}
}

void starHandler(entity* this) {
	this->object->rect.x += rand() % 2;
	if (this->object->rect.x > SCREEN_WIDTH) {
		this->object->rect.x = 0;
	}
	this->object->rect.y += 600 * dt;
	if (this->object->rect.y > SCREEN_WIDTH) {
		this->object->rect.y = 0;
	}
}