#pragma once
#define ENTITY_ENTITY_H

int createEntity(const char* objName, Rect rect, int xOffset, int yOffset, float scale, double angle, int_Texture* texture, int collide, void (*entity_handler)(entity**), void* data, int dataSize, void (*collide_handler)(entity**, entity**, float), body* bodyData);
void runEntities();
void deleteEntity(entity** entity);
void deleteEntities();
void cleanEntities();

object* AABBCollisionObj(entity** a);
void testCollision();
entity** getEntityByID(int ID);

#define ENTRECT(a) (*this)->object->rect.a

enum colliders {
	COLLIDE_NONE = 0,
	COLLIDE_BOX = 1,
	COLLIDE_CIRCLE = 2
};

typedef struct {
	entity* a;
	entity* b;
} collidePair;

dynArray* collideArray;

void collision_stub (entity**, entity**, float);