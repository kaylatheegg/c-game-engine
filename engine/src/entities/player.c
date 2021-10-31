#include "engine.h"

void playerHandler(entity** this) {
	float speed = 15;
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
	SDL_GetMouseState(&x, &y);
	x -= SCREEN_WIDTH/2;
	y -= SCREEN_HEIGHT/2;
	(*this)->object->angle = vecAngle(VECCNT(x,y)) - 90;

	vec movement = vecRotate(VECCNT(0, speed), (*this)->object->angle);
	//vec movement = vecScale(vecNorm(VECCNT(x,y)), speed);
	//movement = vecNorm(movement);
	printf("x: %d, y: %d, deg: %f\n", x, y, (*this)->object->angle);
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
	return;
}

void initPlayer() {
	createEntity("Player", (Rect){WORLDWIDTH/2*32, WORLDHEIGHT/2*32, 64, 64}, 0, 0, 1.0, 0, getTexture("Player"), 1, playerHandler, NULL, 0);
}