#pragma once
#define ENTITIES_H

void worldInit();

entity** createHealthBar(float max, float health, entity** owner);
void deleteHealthBar(entity** bar);
void updateHealthBar(float health, entity** bar);

typedef struct {
	int id;
} pickupData;

void pickupCollisionHandler(entity** this, entity** collision, float distance);
void pickupHandler(entity** this);

typedef struct {
	float gunDt;
	float playerDt;
	int kills;
	int gunID;
	int hp;
	int maxHp;
	entity** healthBar;
} playerData;