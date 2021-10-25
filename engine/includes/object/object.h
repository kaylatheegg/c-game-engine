#pragma once
#define OBJECT_OBJECT_H

int createObject(const char* objName, SDL_Rect rect, int xOffset, int yOffset, float scale, int angle, SDL_Surface* texture);
void removeObject(int id);



int objectUID;

int objectCount;