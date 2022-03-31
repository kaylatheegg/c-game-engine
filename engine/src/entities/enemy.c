#include "engine.h"

void enemyBulletCollisionHandler(entity** this, entity** collision, float distance) {
	UNUSED(distance);
		if (strcmp((*collision)->object->name, "Player") == 0) {
			//printf("collision!\n");
			playerData* data = (playerData*)(*collision)->data;
			data->health -= rand() % 3;
			deleteEntity(this);
		}
		if (strcmp((*collision)->object->name, "Bullet") == 0) {
			deleteEntity(this);
			deleteEntity(collision);
		}
}

void enemyBulletHandler(entity** this) {
	bulletData* intData = (bulletData*)(*this)->data;

	if (intData->dt > 1500)  {
		deleteEntity(this);
		return;
	}
	
	intData->dt += dt * 1000;
}

void snarkHandler(entity** this) {
	enemyData* data = (enemyData*)(*this)->data;
	object* playerObject = (*data->player)->object;
	
	if (rand() % 50 == 0) {
		vec bulletMovement = vecRotate(VECCNT(0, 32), (*this)->object->angle - 180);
		vec rotationOrigin = VECCNT(ENTRECT(x) + ENTRECT(w)/2, ENTRECT(y) + ENTRECT(h)/2);
		vec bulletPosition = vecRotateAroundOrigin(VECCNT(ENTRECT(x)+ENTRECT(w), ENTRECT(y) + ENTRECT(h)), rotationOrigin, (*this)->object->angle);
		createEntity("Enemy Bullet", (Rect){bulletPosition.x, bulletPosition.y, rand() % 4 + 4, 16}, 0, 0, 1.0, (*this)->object->angle, getTexture("Fire"), COLLIDE_CIRCLE, 
			enemyBulletHandler, &(bulletData){0.0f, (vec){0,0}}, sizeof(bulletData), enemyBulletCollisionHandler,
			&(body){0.1, bulletMovement, VECCNT(0,0), VECCNT(0,0)});
	
	}

	if (data->dt > 200.) {
		data->playerPos = vecSub(VECCNT(playerObject->rect.x, -playerObject->rect.y), 
							  VECCNT(ENTRECT(x), -ENTRECT(y)));
		data->dt = 0;
	}

	vec intVec = data->playerPos;
	int maxSpeed = 12;
	vec movement = vecScale(vecNorm(intVec), vecLength(intVec) * 0.1 > maxSpeed ? maxSpeed : 3 * vecLength(intVec) * 0.01);
	movement.y *= -1;
	(*this)->object->angle = vecAngle(movement) + 90;
	(*this)->body->velocity = movement;
	data->movement = movement;
}

void chaseHandler(entity** this) {
	enemyData* data = (enemyData*)(*this)->data;
	object* playerObject = (*data->player)->object;
	data->playerPos = vecSub(VECCNT(playerObject->rect.x, -playerObject->rect.y), 
							  VECCNT(ENTRECT(x), -ENTRECT(y)));
	
	vec intVec = data->playerPos;
	int maxSpeed = 12;
	vec movement = vecScale(vecNorm(intVec), vecLength(intVec) * 0.1 > maxSpeed ? maxSpeed : 3 * vecLength(intVec) * 0.01);
	(*this)->object->angle = vecAngle(movement) + 90;
	movement.y *= -1;
	(*this)->body->velocity = movement;
	ENTRECT(w) = 64;
	ENTRECT(h) = 64;
	data->movement = movement;
}

void acceleratorHandler(entity** this) {
	enemyData* data = (enemyData*)(*this)->data;
	object* playerObject = (*data->player)->object;
	//(*this)->collide = COLLIDE_NONE;
	
	if (data->dt > 100.) {
		data->playerPos = vecSub(VECCNT(playerObject->rect.x, -playerObject->rect.y), 
							  VECCNT(ENTRECT(x), -ENTRECT(y)));
		data->dt = 0;
	}

	vec intVec = data->playerPos;
	int maxSpeed = 3;
	data->acceleration = vecScale(vecNorm(intVec), vecLength(intVec) * 0.1 > maxSpeed ? maxSpeed : 3 * vecLength(intVec) * 0.01);
	

	data->velocity = vecScale(vecAdd(data->acceleration, data->velocity), 0.8);
	if (vecLength(data->velocity) > 10) {
		data->velocity = vecScale(vecNorm(data->velocity), 10);
	
	}
	data->movement = data->velocity;
	data->movement.y *= -1;
	(*this)->body->velocity = data->movement;
	ENTRECT(w) = 100;
	ENTRECT(h) = 48;
	vec angleVec = vecSub(VECCNT(playerObject->rect.x, -playerObject->rect.y), 
							  VECCNT(ENTRECT(x), -ENTRECT(y)));
	(*this)->object->angle = vecAngle(angleVec) + 90;
}

void sparkCollideHandler(entity** this, entity** collision) {
	UNUSED(this);
	//enemyData* data = (enemyData*)(*this)->data;
	if (strcmp((*collision)->object->name, "Player") == 0) {
		//printf("collision!\n");
		playerData* plData = (playerData*)(*collision)->data;
		plData->health -= 1 + rand() % 4;
		//data->health -= rand() % 3;
	}
}

void sparkHandler(entity** this) {
	enemyData* data = (enemyData*)(*this)->data;
	object* playerObject = (*data->player)->object;
	data->movement = VECCNT(0,0);

	data->playerPos = vecSub(VECCNT(playerObject->rect.x, -playerObject->rect.y), 
							  VECCNT(ENTRECT(x), -ENTRECT(y)));
	
	vec intVec = data->playerPos;
	int maxSpeed = 2;
	vec movement = vecScale(vecNorm(intVec), vecLength(intVec) * 0.1 > maxSpeed ? maxSpeed : 3 * vecLength(intVec) * 0.01);
	movement.y *= -1;
	(*this)->body->velocity = movement;

	ENTRECT(w) = 96;
	ENTRECT(h) = 96;
	(*this)->object->angle += 30;
	data->movement = movement;
}

void enemyCollisionHandler(entity** this, entity** collision, float distance) {
	UNUSED(distance);
	enemyData* data = (enemyData*)(*this)->data;
	switch(data->enemyType) {
		default:
			sparkCollideHandler(this, collision);
	}
	if (strcmp((*collision)->object->name, "Enemy") == 0) {
		if ((((enemyData*)(*collision)->data)->enemyType != ENEMY_ACCELERATE)) {
			//printf("collision!\n");
			(*this)->body->velocity = data->movement;
		}
	}
}

void enemyHandler(entity** this) {
	enemyData* data = (enemyData*)(*this)->data;
	if ((*data->player) == NULL) {
		ENTRECT(x) += -4 + rand() % 8;
		ENTRECT(y) += -4 + rand() % 8;
		(*this)->object->angle -= 25;
		updateObject((*this)->object);
		return;
	}

	if (data->healthBar == NULL) {
		data->healthBar = createHealthBar(data->health, data->health, this);
	}

	if (data->health < 1) {
		((playerData*)(*data->player)->data)->killCount++;
		if (data->healthBar != NULL) {
			deleteHealthBar(data->healthBar);
		}
		deleteEntity(this);
		return;
	}
	updateHealthBar(data->health, data->healthBar);

	switch(data->enemyType) {
		case ENEMY_SNARK:
			snarkHandler(this);
			break;
		case ENEMY_CHASE:
			chaseHandler(this);
			break;
		case ENEMY_ACCELERATE:
			acceleratorHandler(this);
			break;
		default:
			sparkHandler(this);
	}

	data->dt += dt * 1000;

}

