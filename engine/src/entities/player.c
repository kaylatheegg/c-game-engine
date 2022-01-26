#include "engine.h"

#define FRAMECONSTANT 1/(60*dt)

void bulletCollisionHandler(entity** this, entity** collision) {
	if (strcmp((*collision)->object->name, "Enemy") == 0) {
		//printf("collision!\n");
		enemyData* data = (enemyData*)(*collision)->data;
		data->health -= 3 + rand() % 2;
		deleteEntity(this);
	}
}

void bulletHandler(entity** this) {
	bulletData* intData = (bulletData*)(*this)->data;
	vec movement = intData->movement;
	ENTRECT(x) += movement.x * FRAMECONSTANT;
	ENTRECT(y) += movement.y * FRAMECONSTANT;
	updateObject((*this)->object);
	if (intData->dt > 1500)  {
		deleteEntity(this);
		return;
	}

	intData->dt += dt * 1000;
}

void mouseHandler(entity** this) {
	int x,y;
	mouseData* intData = (mouseData*)(*this)->data;
	int playerX = (*intData->player)->object->rect.x;
	int playerY = (*intData->player)->object->rect.y;
	
	SDL_GetMouseState(&x, &y);
	ENTRECT(x) = playerX + x - SCREEN_WIDTH/2 - ENTRECT(w) / 2;// - x % 32;
	ENTRECT(y) = playerY - y + SCREEN_HEIGHT/2 - ENTRECT(h) / 2;// - y % 32;
	//(*this)->object->xOffset = -viewport.x;
	//(*this)->object->yOffset = -viewport.y;

	updateObject((*this)->object);

	//SDL_PumpEvents();
	/*if ((buttons & SDL_BUTTON_LMASK) != 0) {
		entity** intEntity = AABBCollision(this);
		if (intEntity != NULL && intEntity != this) {
			deleteEntity(intEntity);
			//printf("%d\n", intObject->id);
		}
	}*/
}


void playerHandler(entity** this) {
	//printf("x: %f, y: %f, x: %f, y: %f\n", ENTRECT(x), ENTRECT(y), ENTRECT(x) - WORLDWIDTH*48/2, ENTRECT(y) - WORLDHEIGHT*48/2);
	playerData* intData = (playerData*)(*this)->data;
	float speed = intData->speed * 60 * dt;

	//-(e^0.01x) + 50
	//int enemyChance = ceil(-1 * powf(2.718281828, intData->aliveDt*0.1) + 50);

	//-0.2x + 50
	int enemyChance = ceil(-0.2 * intData->aliveDt) + 50;
		//printf("%d\n", enemyChance);
	if (enemyChance <= 0) {
		enemyChance = 1;
	}
	//printf("%d\n", enemyChance);

	if (rand() % enemyChance == 0) {
		vec spawnLoc = vecRotate(vecScale(VECCNT(0,1), 400 + rand()%500), rand() % 360);
		createEntity("Enemy", (Rect){ENTRECT(x) + spawnLoc.x, ENTRECT(y) + spawnLoc.y, 32, 32}, 0, 0, 1.0, 0, getTexture("Enemy"), 1, 
			enemyHandler, &(enemyData){this, VECCNT(ENTRECT(x),ENTRECT(y)), 5+rand()%4, 0., NULL, rand()%4, VECCNT(0,0),VECCNT(0,0),VECCNT(0,0)}, sizeof(enemyData), enemyCollisionHandler,
			&(body){10, VECCNT(0,0), VECCNT(0,0), VECCNT(0,0)});
	}

	if (rand() % 50 == 0) {
		vec position = vecRotate(vecScale(VECCNT(0,1), 300), rand() % 360);
		createEntity("Powerup", (Rect){ENTRECT(x) + position.x + rand()%64, ENTRECT(y) + position.y + rand()%64, 32, 32}, 0, 0, 1.0, 0, getTexture("DEFAULT"), 1, 
			NULL, &(powerupData){rand() % 2, 1 + rand() % 999, rand() % 50, rand()%4}, sizeof(powerupData), powerupCollideHandler,
			&(body){10, VECCNT(0,0), VECCNT(0,0), VECCNT(0,0)});
	}





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
	if ((buttons & SDL_BUTTON_LMASK) != 0 && intData->dt > intData->gunDt) {
		intData->dt = 0;
		/*for (int i = 0; i < 5; i++) {
		float angleOffset = -15 + rand()%30;
		vec bulletMovement = vecRotate(VECCNT(0, 32), (*this)->object->angle + angleOffset);
		vec rotationOrigin = VECCNT(ENTRECT(x) + ENTRECT(w)/2, ENTRECT(y) + ENTRECT(h)/2);
		vec bulletPosition = vecRotateAroundOrigin(VECCNT(ENTRECT(x)+32, ENTRECT(y) + 32), rotationOrigin, (*this)->object->angle + angleOffset);
		createEntity("Bullet", (Rect){bulletPosition.x, bulletPosition.y, 4, 16}, 0, 0, 1.0, (*this)->object->angle + angleOffset, getTexture("Bullet"), 1, bulletHandler, &(bulletData){0.0f, (vec){bulletMovement.x, bulletMovement.y}}, sizeof(bulletData));	
		}*/
		if (intData->gunID == 0) {
			vec bulletMovement = vecRotate(VECCNT(0, 32), (*this)->object->angle);
			vec rotationOrigin = VECCNT(ENTRECT(x) + ENTRECT(w)/2, ENTRECT(y) + ENTRECT(h)/2);
			vec bulletPosition = vecRotateAroundOrigin(VECCNT(ENTRECT(x)+32, ENTRECT(y) + 32), rotationOrigin, (*this)->object->angle);
			createEntity("Bullet", (Rect){bulletPosition.x, bulletPosition.y, 4, 16}, 0, 0, 1.0, (*this)->object->angle, getTexture("Bullet"), COLLIDE_CIRCLE, 
				bulletHandler, &(bulletData){0.0f, (vec){0,0}}, sizeof(bulletData), bulletCollisionHandler,
				&(body){0.1, bulletMovement, VECCNT(0,0), VECCNT(0,0)});

		} else if (intData->gunID == 1) {
			for (int i = 0; i < 5; i++) {
				float angleOffset = (-300 + rand() % 600)/10;
				vec bulletMovement = vecRotate(VECCNT(0, 32), (*this)->object->angle + angleOffset);
				vec rotationOrigin = VECCNT(ENTRECT(x) + ENTRECT(w)/2, ENTRECT(y) + ENTRECT(h)/2);
				vec bulletPosition = vecRotateAroundOrigin(VECCNT(ENTRECT(x)+32, ENTRECT(y) + 32), rotationOrigin, (*this)->object->angle + angleOffset);
				createEntity("Bullet", (Rect){bulletPosition.x, bulletPosition.y, 4, 16}, 0, 0, 1.0, (*this)->object->angle + angleOffset, getTexture("Bullet"), COLLIDE_CIRCLE, 
					bulletHandler, &(bulletData){0.0f, (vec){0,0}}, sizeof(bulletData), bulletCollisionHandler,
					&(body){0.1, bulletMovement, VECCNT(0,0), VECCNT(0,0)});	
			}
		} else if (intData->gunID == 2) {
			for (int i = 0; i < 4; i++) {
				float angleOffset = 90 * i;
				vec bulletMovement = vecRotate(VECCNT(0, 32), (*this)->object->angle + angleOffset);
				vec rotationOrigin = VECCNT(ENTRECT(x) + ENTRECT(w)/2, ENTRECT(y) + ENTRECT(h)/2);
				vec bulletPosition = vecRotateAroundOrigin(VECCNT(ENTRECT(x)+32, ENTRECT(y) + 32), rotationOrigin, (*this)->object->angle + angleOffset);
				createEntity("Bullet", (Rect){bulletPosition.x, bulletPosition.y, 4, 16}, 0, 0, 1.0, (*this)->object->angle + angleOffset, getTexture("Bullet"), COLLIDE_CIRCLE, 
					bulletHandler, &(bulletData){0.0f, (vec){0,0}}, sizeof(bulletData), bulletCollisionHandler,
					&(body){0.1, bulletMovement, VECCNT(0,0), VECCNT(0,0)});	
			}
		} else if (intData->gunID == 3) {
			for (int i = 0; i < 90; i++) {
				float angleOffset = i;
				vec bulletMovement = vecRotate(VECCNT(0, 32), (*this)->object->angle + angleOffset * 4);
				vec rotationOrigin = VECCNT(ENTRECT(x) + ENTRECT(w)/2, ENTRECT(y) + ENTRECT(h)/2);
				vec bulletPosition = vecRotateAroundOrigin(VECCNT(ENTRECT(x)+32, ENTRECT(y) + 32), rotationOrigin, (*this)->object->angle + angleOffset * 4);
				createEntity("Bullet", (Rect){bulletPosition.x, bulletPosition.y, 4, 16}, 0, 0, 1.0, (*this)->object->angle + angleOffset * 4, getTexture("Bullet"), COLLIDE_CIRCLE, 
					bulletHandler, &(bulletData){0.0f, (vec){0,0}}, sizeof(bulletData), bulletCollisionHandler,
					&(body){0.1, bulletMovement, VECCNT(0,0), VECCNT(0,0)});	
			}
		}

	}



	if (intData->healthBar == NULL) {
		intData->healthBar = createHealthBar(intData->health, intData->health, this);

		createEntity("mouse", (Rect){0, 0, 64, 64}, 0, 0, 1.0, 0, getTexture("Crosshair"), 0, 
			mouseHandler, &(mouseData){this}, sizeof(mouseData), NULL,
			&(body){0, VECCNT(0,0), VECCNT(0,0), VECCNT(0,0)});
	}

	if (intData->health < 1.) {
		printf("kill count:%d\n", intData->killCount);
		deleteHealthBar(intData->healthBar);
		deleteEntity(this);
		return;
	}
	updateHealthBar(intData->health, intData->healthBar);
	intData->dt += dt * 1000;
	intData->aliveDt += dt;

	return;
}

void initPlayer() {
	createEntity("Player", (Rect){800, 800, 64, 64}, 0, 0, 1.0, 0, getTexture("Player"), COLLIDE_CIRCLE, 
		playerHandler, &(playerData){0, 8, 25, 16000, NULL, 0, 1, 500, 0}, sizeof(playerData), NULL,
		&(body){10, VECCNT(0,0), VECCNT(0,0), VECCNT(0,0)});
	//createEntity("Enemy", (Rect){rand() % WORLDWIDTH * 48, rand() % WORLDHEIGHT * 48, 64, 64}, 0, 0, 1.0, 0, getTexture("Enemy"), 1, enemyHandler, NULL, 0);
}