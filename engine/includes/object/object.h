#pragma once
#define OBJECT_OBJECT_H

object* createObject(const char* objName, Rect rect, int xOffset, int yOffset, float scale, double angle, int_Texture* texture);
void removeObject(const char* key);

void updateObject(object* intObject);
object* getObject(const char* key);
object* getObjectID(int ID);

int objectUID;
int renderObjectSize;
int objectCount;