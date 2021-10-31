#include "engine.h"

typedef struct {
	float dt;
	float speed;
} playerData;

void bulletHandler(entity** this) {
	vec* movement = (vec*)(*this)->data;
	ENTRECT(x) += movement->x;
	ENTRECT(y) += movement->y;
	updateObject((*this)->object);
}

void playerHandler(entity** this) {
	playerData* intData = (playerData*)(*this)->data;
	float speed = intData->speed * 60 * dt;
	/*float angleSpeed = 5;
	if (keyPresses[SDL_SCANCODE_A]) {
		(*this)->object->angle += angleSpeed;
	} if (keyPresses[SDL_SCANCODE_D]) {
		(*this)->object->angle -= angleSpeed;
	}
*/
	if ((*this)->object->angle < 0) {
		(*this)->object->angle = 360;
	}
	if ((*this)->object->angle > 360) {
		(*this)->object->angle = fmod((*this)->object->angle, 360.);
	}

	int x,y;
	Uint32 buttons = SDL_GetMouseState(&x, &y);
	x -= SCREEN_WIDTH/2;
	y -= SCREEN_HEIGHT/2;
	(*this)->object->angle = vecAngle(VECCNT(x,y)) - 90;

	vec movement = vecRotate(VECCNT(0, speed), (*this)->object->angle);
	//vec movement = vecScale(vecNorm(VECCNT(x,y)), speed);
	//movement = vecNorm(movement);
	//printf("x: %d, y: %d, deg: %f\n", x, y, (*this)->object->angle);
	if (keyPresses[SDL_SCANCODE_W]) {
		ENTRECT(x) += movement.x;
		ENTRECT(y) += movement.y;
		viewport.x -= movement.x;
		viewport.y -= movement.y;
	} if (keyPresses[SDL_SCANCODE_S]) {
		ENTRECT(x) -= movement.x;
		ENTRECT(y) -= movement.y;
		viewport.x += movement.x;
		viewport.y += movement.y;
	} 
	updateObject((*this)->object);
	if ((buttons & SDL_BUTTON_LMASK) != 0 && intData->dt > 0) {
		intData->dt = 0;
		vec bulletMovement = vecRotate(VECCNT(0, speed*3 * 60 * dt), (*this)->object->angle);
		vec rotationOrigin = VECCNT(ENTRECT(x) + ENTRECT(w)/2, ENTRECT(y) + ENTRECT(h)/2);
		vec bulletPosition = vecRotateAroundOrigin(VECCNT(ENTRECT(x)+32, ENTRECT(y)), rotationOrigin, (*this)->object->angle);
		createEntity("Bullet", (Rect){bulletPosition.x, bulletPosition.y, 8, 16}, 0, 0, 1.0, (*this)->object->angle, getTexture("Bullet"), 1, bulletHandler, &(vec){bulletMovement.x, bulletMovement.y}, sizeof(vec));
	} 
	intData->dt += dt * 1000;

	return;
}

void initPlayer() {
	createEntity("Player", (Rect){WORLDWIDTH/2*32, WORLDHEIGHT/2*32, 64, 64}, 0, 0, 1.0, 0, getTexture("Player"), 1, playerHandler, &(playerData){0, 15}, sizeof(playerData));
}