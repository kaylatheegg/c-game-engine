#include "engine.h"

void powerupCollideHandler(entity** this, entity** collision, float distance) {
	UNUSED(distance);
	powerupData* data = (*this)->data;
	if (strcmp((*collision)->object->name, "Player") == 0) {
			//printf("collision!\n");
		playerData* plData = (playerData*)(*collision)->data;
		plData->health += data->healthChange;
		if (data->gunChange != 0) {
			plData->gunID = data->gunChange;
		}
		if (data->speedChange != 0) {
			plData->speed += data->speedChange;
			if (plData->speed > plData->maxSpeed) {
				plData->speed = plData->maxSpeed;
			}
		}
		if (data->bulletDTChange != 0) {
			plData->gunDt = data->bulletDTChange;
		}
		deleteEntity(this);
	}
	
}

