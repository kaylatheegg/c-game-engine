#pragma once
#define OBJECT_OBJECT_H

object* createObject(const char* objName, Rect rect, int xOffset, int yOffset, float scale, double angle, int_Texture* tx, int layer);
void removeObject(const char* key);

void updateObject(object* intObject);
object* getObject(const char* key);
object* getObjectID(int ID);

int objectUID;
int renderObjectSize[MAX_RENDER_LAYERS];
int objectCount;

