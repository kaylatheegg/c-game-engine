#include "engine.h"

#define FRAMECONSTANT 1/(60*dt)

void bulletHandler(entity** this) {
	bulletData* intData = (bulletData*)(*this)->data;
	vec movement = intData->movement;
	ENTRECT(x) += movement.x;
	ENTRECT(y) += movement.y;
	updateObject((*this)->object);
	if (intData->dt > 1500)  {
		deleteEntity(this);
		intData->dt = 0.;
	}
	entity** intEntity = circleBoxCollision(this);
	if (intEntity != NULL) {
		if (strcmp((*intEntity)->object->name, "Enemy") == 0) {
			//printf("collision!\n");
			enemyData* data = (enemyData*)(*intEntity)->data;
			data->health -= rand() % 3;
			deleteEntity(this);
		}
	}
	intData->dt += dt * 1000;
}


void playerHandler(entity** this) {
	//printf("x: %f, y: %f, x: %f, y: %f\n", ENTRECT(x), ENTRECT(y), ENTRECT(x) - WORLDWIDTH*48/2, ENTRECT(y) - WORLDHEIGHT*48/2);


	if (rand() % 100 == 0) {
		createEntity("Enemy", (Rect){rand() % WORLDWIDTH * 48, rand() % WORLDHEIGHT * 48, 32, 32}, 0, 0, 1.0, 0, getTexture("Enemy"), 1, enemyHandler, &(enemyData){*this, VECCNT(ENTRECT(x),ENTRECT(y)), 2, 0.}, sizeof(enemyData));
	}



	playerData* intData = (playerData*)(*this)->data;
	float speed = intData->speed * 60 * dt;
	/*float angleSpeed = 5;
	if (keyPresses[SDL_SCANCODE_A]) {
		(*this)->object->angle += angleSpeed;
	} if (keyPresses[SDL_SCANCODE_D]) {
		(*this)->object->angle -= angleSpeed;
	}
*/
	if (keyPresses[SDL_SCANCODE_W]) {
		ENTRECT(y) += speed;
		viewport.y -= speed;
	} if (keyPresses[SDL_SCANCODE_A]) {
		ENTRECT(x) -= speed;
		viewport.x += speed;
	} if (keyPresses[SDL_SCANCODE_S]) {
		ENTRECT(y) -= speed;
		viewport.y += speed;
	} if (keyPresses[SDL_SCANCODE_D]) {
		ENTRECT(x) += speed;
		viewport.x -= speed;
	}

	/*if ((*this)->object->angle < 0) {
		(*this)->object->angle = 360;
	}
	if ((*this)->object->angle > 360) {
		(*this)->object->angle = fmod((*this)->object->angle, 360.);
	}*/

	int x,y;
	Uint32 buttons = SDL_GetMouseState(&x, &y);
	x -= SCREEN_WIDTH/2;
	y -= SCREEN_HEIGHT/2;
	(*this)->object->angle = vecAngle(VECCNT(x,y)) - 90;

	//vec movement = vecRotate(VECCNT(0, speed), (*this)->object->angle);
	//vec movement = vecScale(vecNorm(VECCNT(x,y)), speed);
	//movement = vecNorm(movement);
	//printf("x: %d, y: %d, deg: %f\n", x, y, (*this)->object->angle);
	/*if (keyPresses[SDL_SCANCODE_W]) {
		ENTRECT(x) += movement.x;
		ENTRECT(y) += movement.y;
		viewport.x -= movement.x;
		viewport.y -= movement.y;
	} if (keyPresses[SDL_SCANCODE_S]) {
		ENTRECT(x) -= movement.x;
		ENTRECT(y) -= movement.y;
		viewport.x += movement.x;
		viewport.y += movement.y;
	} */
	updateObject((*this)->object);
	if ((buttons & SDL_BUTTON_LMASK) != 0 && intData->dt > 100) {
		intData->dt = 0;
		vec bulletMovement = vecRotate(VECCNT(0, 32), (*this)->object->angle);
		vec rotationOrigin = VECCNT(ENTRECT(x) + ENTRECT(w)/2, ENTRECT(y) + ENTRECT(h)/2);
		vec bulletPosition = vecRotateAroundOrigin(VECCNT(ENTRECT(x)+32, ENTRECT(y) + 32), rotationOrigin, (*this)->object->angle);
		createEntity("Bullet", (Rect){bulletPosition.x, bulletPosition.y, 4, 16}, 0, 0, 1.0, (*this)->object->angle, getTexture("Bullet"), 1, bulletHandler, &(bulletData){0.0f, (vec){bulletMovement.x, bulletMovement.y}}, sizeof(bulletData));
	}

	if (intData->healthBar == NULL) {
		createHealthBar(intData->health, intData->health, this);
	}

	if (intData->health < 1.) {
		deleteEntity(this);
	}
	updateHealthBar(intData->health, intData->healthBar);
	intData->dt += dt * 1000;


	return;
}

void initPlayer() {
	createEntity("Player", (Rect){WORLDWIDTH/2*32, WORLDHEIGHT/2*32, 64, 64}, 0, 0, 1.0, 0, getTexture("Player"), 1, playerHandler, &(playerData){0, 8, 16}, sizeof(playerData));
	//createEntity("Enemy", (Rect){rand() % WORLDWIDTH * 48, rand() % WORLDHEIGHT * 48, 64, 64}, 0, 0, 1.0, 0, getTexture("Enemy"), 1, enemyHandler, NULL, 0);
}