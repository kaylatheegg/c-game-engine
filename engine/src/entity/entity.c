#include "../../includes/engine.h"

void stub(){}

void processDeletes();
void deleteEntityInt(entity* entity);

int createEntity(char* objName, SDL_Rect rect, int xOffset, int yOffset, float scale, int angle, SDL_Texture* texture, int collide, void (*entity_handler)(entity*)) {
	int objectId = createObject(objName, rect, xOffset, yOffset, scale, angle, texture);
	
	entity* intEntity;
	intEntity = gmalloc(sizeof(entity));

	char buffer[18];
	itoa(objectId, buffer);
	dictionary intDict = findKey(objects, buffer);

	*intEntity = (entity) {
		.object = intDict->value,
		.entity_handler = entity_handler == NULL ? *stub : entity_handler,
		.collide = collide,
		.deleted = 0
	};

	itoa(entityUID, buffer);

	addToDictionary(entities, buffer, intEntity);
	entityCount++;
	return entityUID++;
}

void runEntities() {
	dictionary entityIterator = entities;
	for (int i = 0; i < entityUID; i++) {
		entityIterator = entityIterator->next;
		if (entityIterator == NULL) {
			break;
		}
		entity* internalEntity = entityIterator->value;
		internalEntity->entity_handler(entityIterator->value);
	}
	processDeletes();
}

void processDeletes() {
	if (deletedCount == 0) {
		return;
	}
	for (int i = 0; i < deletedCount - 1; i++) {
		deleteEntityInt(deleteArray[i]);
	}
	gfree(deleteArray);
	deleteArray = gmalloc(sizeof(entity*));
	deletedCount = 0;
}

void deleteEntity(entity* entity) {
	if (entity == NULL) {
		return;
	}
	deleteArray = grealloc(deleteArray, sizeof(struct entity*) * (deletedCount + 1));
	deleteArray[deletedCount] = entity;
	deletedCount++;
}



void deleteEntityInt(entity* entity) {
	char buffer[18];
	if (entity->deleted == 1) {
	    return;
	}

	//printf("entity name: %s\n", entity->object->name);
	itoa(entity->object->id, buffer);
	dictionary intDict = findKey(entities, buffer);
	if (intDict == NULL) {
		logtofile("could not find an entity, assuming that it was destroyed unsafely; assuming unsafe env.", SVR, "Entity");
		crash();
	}
	
	removeObject(entity->object->id);
	//HUGE MEMORY LEAK WARNING!!! THIS CAN AND WILL LEAK MEMORY, BUT THERES NOTHING I CAN FUCKING DO ABOUT IT
	//gfree(intDict->value);
	entity->deleted = true;
	removeKey(entities, buffer);
	entityCount--;
}