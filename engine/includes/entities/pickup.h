#pragma once
#define ENTITIES_PICKUP_H

typedef struct {
	int id;
} pickupData;

void pickupCollisionHandler(entity** this, entity** collision, float distance);
void pickupHandler(entity** this);