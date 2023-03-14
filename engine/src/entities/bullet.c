#include "engine.h"

void bulletHandler(entity** this) {
	bulletData* data = (bulletData*)(*this)->data;
	data->aliveDt += dt;
	if (data->aliveDt >= data->bulletDt) {
		deleteEntity(this);
	}

	(*this)->object->angle += 15;
	//rotate the movement vector each frame for funny
	//setVelocity(this, vecRotate((*this)->body->velocity, 1));
	updateObject((*this)->object);
}

void bulletCollisionHandler(entity** this, entity** a, float b) {
	bulletData* data = (bulletData*)(*this)->data;
	if ((*a) == (*data->parent)) {
		return;
	} else if (strcmp((*a)->object->name, (*this)->object->name) != 0) {
		deleteEntity(this);
	} 

	UNUSED(this);
	UNUSED(a);
	UNUSED(b);
}
