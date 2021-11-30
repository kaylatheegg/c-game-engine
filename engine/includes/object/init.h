#pragma once
#define OBJECT_INIT_H


typedef struct {
	const char* name;
	int id;

	int xOffset;
	int yOffset;
	float scale;
	double angle;

	int_Texture* texture;
	Rect rect;

	int vertexID;

} object;

dictionary objects;
int* vertexPool;
int vertexPoolSize;


void initObjects();
void cleanObjects();