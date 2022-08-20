#pragma once
#define ENTITIES_BULLET_H

typedef struct {
	entity** parent;
	float bulletDt;
	float aliveDt;
} bulletData;

void bulletCollisionHandler(entity** this, entity** a, float b);
void bulletHandler(entity** this);