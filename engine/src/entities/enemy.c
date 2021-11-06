#include "engine.h"


void enemyHandler(entity** this) {
	enemyData* data = (enemyData*)(*this)->data;
	object* playerObject = data->player->object;
	if (playerObject == NULL) {
		return;
	}
	vec movement = vecScale(vecNorm(vecSub(VECCNT(playerObject->rect.x, -playerObject->rect.y), 
							  VECCNT(ENTRECT(x), -ENTRECT(y)))), 3);
	(*this)->object->angle = vecAngle(movement) + 90;
	ENTRECT(x) += movement.x;
	ENTRECT(y) -= movement.y;

	updateObject((*this)->object);
}