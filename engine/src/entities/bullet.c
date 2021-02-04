#include "../../includes/engine.h"

void bulletHandler(entity* this) {
	if (this->object->rect.y > SCREEN_HEIGHT) {
		deleteEntity(this);
	} else {
		this->object->rect.y-= 960. * dt;
	}
}
