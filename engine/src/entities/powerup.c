#include "engine.h"

typedef struct {
	float speedChange;
	float bulletDTChange;
	float healthChange;
	int gunChange;
} powerupData;

void powerupHandler(entity** this) {
	testCollision(this);

	powerupData* data = (*this)->data;

	for (int i = 0; i < COLLIDE_SIZE && collideArray[i] != NULL; i++) {
		if (strcmp((*collideArray[i])->object->name, "Player") == 0) {
			//printf("collision!\n");
			playerData* plData = (playerData*)(*collideArray[i])->data;
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
}