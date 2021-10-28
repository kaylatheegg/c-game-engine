#pragma once
#define OBJECT_OBJECT_H

int createObject(const char* objName, SDL_Rect rect, int xOffset, int yOffset, float scale, int angle, int_Texture* texture);
void removeObject(int id);

void updateObject(object* intObject);

int objectUID;

int objectCount;