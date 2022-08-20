#pragma once
#define ENTITIES_ENEMY_H

void enemyCollisionHandler(entity** this, entity** a, float b);
void enemyHandler(entity** this);

typedef struct {
	int hp;
	entity** player;
	float gunDt;
	float enemyDt;
	entity** healthBar;
	entity** world;
} enemyData;