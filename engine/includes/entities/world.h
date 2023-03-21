#pragma once
#define ENTITIES_WORLD_H

void worldInit();

typedef struct {
	float spawnDt;
	float enemySpawnDt;
	entity** player;
	float pickupDt;
	float pickupSpawnDt;
	float waveDt;
	int waveCount;
	int enemyCount;
	int killedCount;
} worldData;

vec vecCam;