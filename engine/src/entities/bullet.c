#include "engine.h"

void bulletHandler(entity** this) {
	if ((*this)->object->rect.y < -32) {
		deleteEntity(this);
	} else {
		(*this)->object->rect.y-= 960. * dt;
	}
	entity** intEntity = AABBCollision(this);
	if (intEntity != NULL && (*intEntity)->collide == 1 && this != intEntity) {
		deleteEntity(intEntity);
		deleteEntity(this);
	}
	intEntity = NULL;
}
