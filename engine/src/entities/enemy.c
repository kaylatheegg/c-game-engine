#include "engine.h"


void enemyHandler(entity** this) {
	enemyData* data = (enemyData*)(*this)->data;
	object* playerObject = data->player->object;
	if (playerObject == NULL) {
		return;
	}

	if (data->health < 0) {
		deleteEntity(this);
	}

	if (data->dt > 200.) {
		data->playerPos = vecSub(VECCNT(playerObject->rect.x, -playerObject->rect.y), 
							  VECCNT(ENTRECT(x), -ENTRECT(y)));
		data->dt = 0;
	}

	vec intVec = data->playerPos;

	vec movement = vecScale(vecNorm(intVec), vecLength(intVec) * 0.1 > 15 ? 15 : 3 * vecLength(intVec) * 0.01);
	(*this)->object->angle = vecAngle(movement) + 90;
	ENTRECT(x) += movement.x;
	ENTRECT(y) -= movement.y;

	updateObject((*this)->object);
	data->dt += dt * 1000;

}