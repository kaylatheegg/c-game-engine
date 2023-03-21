#include "engine.h"

void bulletHandler(entity** this) {
	bulletData* data = (bulletData*)(*this)->data;
	data->aliveDt += dt;
	if (data->aliveDt >= data->bulletDt) {
		deleteEntity(this);
	}

	cpShapeFilter filter;
	filter.categories = BIT(1);
	filter.mask = BIT(1) ^ BIT(2) ^ BIT(4);
	cpShapeSetFilter((*this)->body->shape, filter);

	//(*this)->object->angle += 15;
	//rotate the movement vector each frame for funny
	//setVelocity(this, vecRotate((*this)->body->velocity, 1));
	updateObject((*this)->object);
}

void bulletCollisionHandler(entity** this, entity** a, float b) {
	//bulletData* data = (bulletData*)(*this)->data;
	printf("we dead\n");
	deleteEntity(this);

	UNUSED(this);
	UNUSED(a);
	UNUSED(b);
}
