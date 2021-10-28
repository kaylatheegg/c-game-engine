#pragma once
#define OBJECT_INIT_H

typedef struct {
	char* name;
	int id;

	int xOffset;
	int yOffset;
	float scale;
	int angle;

	int_Texture* texture;
	SDL_Rect rect;

	int vertexID;

} object;

dictionary objects;
int* vertexPool;
int vertexPoolSize;


void initObjects();
void cleanObjects();