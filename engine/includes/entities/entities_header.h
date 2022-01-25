#pragma once
#define ENTITIES_H

int worldWidth;
int worldHeight;

typedef struct {
	float dt;
	float speed;
	float maxSpeed;
	float health;
	entity** healthBar;
	int killCount;
	int gunID;
	float gunDt;
	float aliveDt;
} playerData;

typedef struct {
	float dt;
	vec movement;
} bulletData;

typedef struct {
	float speedChange;
	float bulletDTChange;
	float healthChange;
	int gunChange;
} powerupData;

void powerupCollideHandler(entity** this, entity** collision);

#include "camera.h"
#include "player.h"

entity** createHealthBar(float max, float health, entity** owner);
void deleteHealthBar(entity** bar);
void updateHealthBar(float health, entity** bar);

int initWorld();
void initAnimals();
void enemyHandler(entity** this);
void enemyCollisionHandler(entity** this, entity** collision);

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
	vec velocity;
	vec acceleration;
} enemyData;

enum enemyTypes {
	ENEMY_SNARK = 0,
	ENEMY_CHASE = 1,
	ENEMY_ACCELERATE = 2
};


enum tileTypes {
	NOTHING,
	DIRT,
	GRASS,
	FIRE,
	BURNT,
	FLOOR,
	TILECOUNT
};

typedef struct {
	entity** player;
} mouseData;

tileData world[1200][1200];