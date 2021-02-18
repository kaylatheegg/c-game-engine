#include "../../includes/engine.h"

//note for later: once you're done w/ this, make sure to move it to a general entity handling area

entity* AABBCollision(entity* a) {
	SDL_Rect rect1 = a->object->rect;
	dictionary entityIterator = entities;
	for (int i = 0; i < entityUID; i++) {
		entityIterator = entityIterator->next;
		if (entityIterator == NULL) {
			break;
		}
		if (entityIterator->value == a) {
			continue;
		}

		entity intEntity = *(entity*)entityIterator->value;
		SDL_Rect rect2 = intEntity.object->rect;
		if (rect1.x < rect2.x + rect2.w &&
   		rect1.x + rect1.w > rect2.x &&
   		rect1.y < rect2.y + rect2.w &&
   		rect1.y + rect1.h > rect2.y) {
			//collision!
			return (entity*)entityIterator->value;
   		}
	}	
	return NULL;
}



void enemyHandler(entity* this) {
	//createEntity("Bullet", (SDL_Rect){this->object->rect.x + 24, this->object->rect.y, 8, 16}, 0, 0, 1, 0, getTexture("Bullet"), *bulletHandler);
	this->object->rect.x += 8 - rand() % 16;
	if (this->object->rect.x > 0) {
		this->object->rect.x -= 8;
	}
	if (this->object->rect.x + this->object->rect.w < SCREEN_WIDTH) {
		this->object->rect.x += 8;
	}
	//this->object->rect.w = 96 * sin(dtCount_enemy);
	//this->object->rect.y += 1;
}



void playerHandler(entity* this) {
	float speed = 960. * dt;
	//printf("%d\n", speed);
	dtCount_player += dt;
	dtCount_enemy += dt;
	if (keyPresses[SDL_SCANCODE_W]) {
		if (dtCount_player > 16. * dt) {
			createEntity("Bullet", (SDL_Rect){this->object->rect.x + 24, this->object->rect.y, 8, 16}, 0, 0, 1, 0, getTexture("Bullet"), 1, *bulletHandler);
			createEntity("Bullet", (SDL_Rect){this->object->rect.x + this->object->rect.w - 32, this->object->rect.y, 8, 16}, 0, 0, 1, 0, getTexture("Bullet"), 1, *bulletHandler);
			
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

	if (dtCount_enemy > 160. * dt) {
		createEntity("Enemy", (SDL_Rect){rand() % SCREEN_WIDTH, 96, 96, 96}, 0, 0, 1, 0, getTexture("Enemy"), 1, *enemyHandler);
		dtCount_enemy = 0;
	}
}

void playerInit() {
	dtCount_player = 0.;
	dtCount_enemy = 0.;
	createEntity("Player", (SDL_Rect){SCREEN_WIDTH / 2, SCREEN_HEIGHT - 128, 128, 128}, 0, 0, 1, 0, getTexture("Ship"), 2, *playerHandler);
}
