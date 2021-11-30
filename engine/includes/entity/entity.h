#pragma once
#define ENTITY_ENTITY_H

int createEntity(const char* objName, Rect rect, int xOffset, int yOffset, float scale, double angle, int_Texture* texture, int collide, void (*entity_handler)(entity**), void* data, int dataSize);
void runEntities();
void deleteEntity(entity** entity);
void deleteEntities();

entity** AABBCollision(entity** a);
object* AABBCollisionObj(entity** a);
entity** circleBoxCollision(entity** a);

entity** getEntityByID(int ID);

#define ENTRECT(a) (*this)->object->rect.a