#pragma once
#define ENTITIES_H

#define WORLDWIDTH 50
#define WORLDHEIGHT 50

#include "camera.h"
#include "player.h"

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
	entity* player;
	vec playerPos;
	int health;
	float dt;
} enemyData;


enum tileTypes{
	DIRT = 0,
	GRASS = 1,
	FIRE = 2,
	BURNT = 3
};

tileData world[WORLDWIDTH][WORLDHEIGHT];