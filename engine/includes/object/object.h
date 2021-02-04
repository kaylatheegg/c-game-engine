#pragma once
#define OBJECT_OBJECT_H

int createObject(char* objName, SDL_Rect rect, int xOffset, int yOffset, float scale, int angle, SDL_Texture* texture);
void removeObject(int id);



int objectCount;

