#pragma once
#define OBJECT_INIT_H

typedef struct {
	char* name;
	int id;

	int xOffset;
	int yOffset;
	float scale;
	int angle;

	SDL_Texture *texture;
	SDL_Rect rect;

} object;

dictionary objects;

void initObjects();
void cleanObjects();