#pragma once
#define ENTITY_ENTITY_H

int createEntity(char* objName, SDL_Rect rect, int xOffset, int yOffset, float scale, int angle, SDL_Texture* texture, int collide, void (*entity_handler)(entity*));
void runEntities();
void deleteEntity(entity* entity);

