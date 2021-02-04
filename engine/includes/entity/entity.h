#pragma once
#define ENTITY_ENTITY_H

int createEntity(char* objName, int x, int y, int width, int height, int xOffset, int yOffset, float scale, int angle, SDL_Texture* texture, void (*entity_handler)(entity*));
void runEntities();
void deleteEntity(entity* entity);