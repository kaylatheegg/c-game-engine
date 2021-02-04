#include "../../includes/engine.h"



void playerHandler(entity* this) {
	float speed = 960. * dt;
	//printf("%d\n", speed);
	dtCount_player += dt;
	if (keyPresses[SDL_SCANCODE_W]) {
		if (dtCount_player > 16. * dt) {
			createEntity("Bullet", (SDL_Rect){this->object->rect.x + 24, this->object->rect.y, 8, 16}, 0, 0, 1, 0, getTexture("Bullet"), *bulletHandler);
			createEntity("Bullet", (SDL_Rect){this->object->rect.x + this->object->rect.w - 32, this->object->rect.y, 8, 16}, 0, 0, 1, 0, getTexture("Bullet"), *bulletHandler);
			
			dtCount_player = 0.;
		}

	} if (keyPresses[SDL_SCANCODE_A]) {
		if (this->object->rect.x + speed > 0) {
			this->object->rect.x -= speed; 
		}

	} if (keyPresses[SDL_SCANCODE_D]) {
		if (this->object->rect.x + this->object->rect.w - speed < SCREEN_WIDTH) {
			this->object->rect.x += speed; 
		}
	}
}

void playerInit() {
	dtCount_player = 0.;
	createEntity("Player", (SDL_Rect){SCREEN_WIDTH / 2, SCREEN_HEIGHT - 128, 128, 128}, 0, 0, 1, 0, getTexture("Ship"), *playerHandler);
}
