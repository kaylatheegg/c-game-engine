#include "engine.h"

void pickupHandler(entity** this) {
	pickupData* data = (pickupData*)(*this)->data;
	char* textureString = "DEFAULT";
	if (data->id == 0) {
		textureString = "HealthPickup";
	}
	if (data->id == 1) {
		textureString = "Shotgun";
	}
	if (data->id == 2) {
		textureString = "Pistol";
	}

	int_Texture* tx = getTexture(textureString);
	(*this)->object->texture = tx;
	updateObject((*this)->object);
}

void pickupCollisionHandler(entity** this, entity** collision, float distance) {
	UNUSED(distance);

	if (strcmp((*collision)->object->name, "Player") == 0) {
		pickupData* data = (pickupData*)(*this)->data;
		if (data->id == 0) {
			playerData* pData = (playerData*)(*collision)->data;
			pData->hp += 5 + randRange(5);
			deleteEntity(this);
		}
		if (data->id == 1) {
			playerData* pData = (playerData*)(*collision)->data;
			pData->gunID = 1;
			pData->gunDt = .5;
			deleteEntity(this);
		}
		if (data->id == 2) {
			playerData* pData = (playerData*)(*collision)->data;
			pData->gunID = 0;
			pData->gunDt = .3;
			deleteEntity(this);
		}
	}
}

