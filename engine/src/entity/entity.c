#include "engine.h"

void stub(){}

void processDeletes();
void deleteEntityInt(entity** entity);

int createEntity(char* objName, SDL_Rect rect, int xOffset, int yOffset, float scale, int angle, SDL_Texture* texture, int collide, void (*entity_handler)(entity**)) {
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
		.deleted = 0,
		.id = entityUID
	};

	itoa(entityUID, buffer);

	addToDictionary(entities, buffer, intEntity);
	entityCount++;
	return entityUID++;
}

void runEntities() {
	dictionary entityIterator = entities;
	for (int i = 0; i < entityCount; i++) {
		entityIterator = entityIterator->next;
		if (entityIterator == NULL) {
		    return;
		}
		entity* internalEntity = entityIterator->value;
		if (internalEntity == NULL) {
		    continue;
		}
		if (internalEntity->deleted == 1) {
			continue;
		}

		internalEntity->entity_handler((entity**)&entityIterator->value);
	}
}

void deleteEntity(entity** intEntity) {
	(*intEntity)->deleted = 1;
	entityCount--;
	removeObject((*intEntity)->object->id);

	char buffer[18];
	itoa((*intEntity)->id, buffer);

	dictionary entityDictionary = findKey(entities, buffer);
	gfree(entityDictionary->value);

	entityDictionary->value = NULL;
	removeKey(entities, buffer);

	*intEntity = NULL;
}