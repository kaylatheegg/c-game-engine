#pragma once
#define ENTITY_ENTITY_H

int createEntity(const char* objName, SDL_Rect rect, int xOffset, int yOffset, float scale, int angle, int_Texture* texture, int collide, void (*entity_handler)(entity**), void* data, int dataSize);
void runEntities();
void deleteEntity(entity** entity);

entity** AABBCollision(entity** a);
object* AABBCollisionObj(entity** a);

entity** getEntity(int UID);

#define ENTRECT(a) (*this)->object->rect.a