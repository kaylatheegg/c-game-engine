#include "engine.h"

void enemyBulletHandler(entity** this) {
	bulletData* intData = (bulletData*)(*this)->data;
	vec movement = intData->movement;
	ENTRECT(x) += movement.x;
	ENTRECT(y) += movement.y;
	updateObject((*this)->object);
	if (intData->dt > 1500)  {
		deleteEntity(this);
		return;
	}
	entity** intEntity = circleBoxCollision(this);
	if (intEntity != NULL) {
		if (strcmp((*intEntity)->object->name, "Player") == 0) {
			//printf("collision!\n");
			playerData* data = (playerData*)(*intEntity)->data;
			data->health -= rand() % 3;
			deleteEntity(this);
		}
		if (strcmp((*intEntity)->object->name, "Bullet") == 0) {
			deleteEntity(this);
			deleteEntity(intEntity);
		}
	}
	intData->dt += dt * 1000;
}

void snarkHandler(entity** this) {
	enemyData* data = (enemyData*)(*this)->data;
	object* playerObject = data->player->object;
	
	if (rand() % 50 == 0) {
		vec bulletMovement = vecRotate(VECCNT(0, 32), (*this)->object->angle - 180);
		vec rotationOrigin = VECCNT(ENTRECT(x) + ENTRECT(w)/2, ENTRECT(y) + ENTRECT(h)/2);
		vec bulletPosition = vecRotateAroundOrigin(VECCNT(ENTRECT(x)+ENTRECT(w), ENTRECT(y) + ENTRECT(h)), rotationOrigin, (*this)->object->angle);
		createEntity("Enemy Bullet", (Rect){bulletPosition.x, bulletPosition.y, rand() % 4 + 4, 16}, 0, 0, 1.0, (*this)->object->angle, getTexture("Fire"), 1, enemyBulletHandler, &(bulletData){0.0f, (vec){bulletMovement.x, bulletMovement.y}}, sizeof(bulletData));
	
	}

	if (data->dt > 200.) {
		data->playerPos = vecSub(VECCNT(playerObject->rect.x, -playerObject->rect.y), 
							  VECCNT(ENTRECT(x), -ENTRECT(y)));
		data->dt = 0;
	}

	vec intVec = data->playerPos;
	int maxSpeed = 12;
	vec movement = vecScale(vecNorm(intVec), vecLength(intVec) * 0.1 > maxSpeed ? maxSpeed : 3 * vecLength(intVec) * 0.01);
	(*this)->object->angle = vecAngle(movement) + 90;
	ENTRECT(x) += movement.x;
	ENTRECT(y) -= movement.y;
}

void chaseHandler(entity** this) {
	enemyData* data = (enemyData*)(*this)->data;
	object* playerObject = data->player->object;
	data->playerPos = vecSub(VECCNT(playerObject->rect.x, -playerObject->rect.y), 
							  VECCNT(ENTRECT(x), -ENTRECT(y)));
	
	vec intVec = data->playerPos;
	int maxSpeed = 12;
	vec movement = vecScale(vecNorm(intVec), vecLength(intVec) * 0.1 > maxSpeed ? maxSpeed : 3 * vecLength(intVec) * 0.01);
	(*this)->object->angle = vecAngle(movement) + 90;
	ENTRECT(x) += movement.x;
	ENTRECT(y) -= movement.y;
	ENTRECT(w) = 64;
	ENTRECT(h) = 64;
}

void sparkHandler(entity** this) {
	enemyData* data = (enemyData*)(*this)->data;
	object* playerObject = data->player->object;

	entity** intEntity = circleBoxCollision(this);
	if (intEntity != NULL) {
		if (strcmp((*intEntity)->object->name, "Player") == 0) {
			//printf("collision!\n");
			playerData* plData = (playerData*)(*intEntity)->data;
			plData->health -= 1 + rand() % 4;
			data->health -= rand() % 3;
		}
	}

	data->playerPos = vecSub(VECCNT(playerObject->rect.x, -playerObject->rect.y), 
							  VECCNT(ENTRECT(x), -ENTRECT(y)));
	
	vec intVec = data->playerPos;
	int maxSpeed = 2;
	vec movement = vecScale(vecNorm(intVec), vecLength(intVec) * 0.1 > maxSpeed ? maxSpeed : 3 * vecLength(intVec) * 0.01);
	ENTRECT(x) += movement.x;
	ENTRECT(y) -= movement.y;
	ENTRECT(w) = 96;
	ENTRECT(h) = 96;
	(*this)->object->angle = rand() % 360;
}


void enemyHandler(entity** this) {
	enemyData* data = (enemyData*)(*this)->data;
	object* playerObject = data->player->object;
	if (playerObject == NULL) {
		return;
	}

	if (data->healthBar == NULL) {
		data->healthBar = createHealthBar(data->health, data->health, this);
	}

	if (data->health < 1) {
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
		default:
			sparkHandler(this);
	}

	updateObject((*this)->object);
	data->dt += dt * 1000;

}

