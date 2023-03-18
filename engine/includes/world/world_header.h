#pragma once
#define WORLD_H 

int generateWorld();
int loadStructure(char* filename, int x, int y);

dictionary chunks;

typedef struct {
	int X;
	int Y;
	int** ids;
} chunk;

int createChunk(int X, int Y);
chunk* findChunk(int x, int y);
int checkTile(int x, int y);
int addTile(int x, int y, int id);
void updateWalls();
