#pragma once
#define ENTITIES_H

#define WORLDWIDTH 50
#define WORLDHEIGHT 50

typedef struct {
	float dt;
	float speed;
	float maxSpeed;
	float health;
	entity** healthBar;
	int killCount;
	int gunID;
	float gunDt;
} playerData;

typedef struct {
	float dt;
	vec movement;
} bulletData;




#include "camera.h"
#include "player.h"

entity** createHealthBar(float max, float health, entity** owner);
void deleteHealthBar(entity** bar);
void updateHealthBar(float health, entity** bar);

int initWorld();
void initAnimals();
void enemyHandler(entity** this);

typedef struct  {
	int x;
	int y;
	int type;
	int tileUpdate;
	int ID;
} tileData;

typedef struct {
	entity** player;
	vec playerPos;
	int health;
	float dt;
	entity** healthBar;
	int enemyType;
	vec movement;
} enemyData;

enum enemyTypes {
	ENEMY_SNARK = 0,
	ENEMY_CHASE = 1
};


enum tileTypes {
	DIRT = 0,
	GRASS = 1,
	FIRE = 2,
	BURNT = 3
};

tileData world[WORLDWIDTH][WORLDHEIGHT];