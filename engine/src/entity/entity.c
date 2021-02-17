#include "../../includes/engine.h"

void stub(){}

void processDeletes();
void deleteEntityInt(entity* entity);

int createEntity(char* objName, SDL_Rect rect, int xOffset, int yOffset, float scale, int angle, SDL_Texture* texture, int collide, void (*entity_handler)(entity*)) {
	int objectId = createObject(objName, rect, xOffset, yOffset, scale, angle, texture);
	
	entity* intEntity;
	intEntity = malloc(sizeof(entity));
	if (intEntity == NULL) {
		logtofile("could not allocate memory for entity, assuming OoM!", SVR, "Entity");
		crash();
	}

	char buffer[18];
	itoa(objectId, buffer);
	dictionary intDict = findKey(objects, buffer);
	if (intDict == NULL) {
		logtofile("could not find an object we LITERALLY JUST MADE, assuming unsafe env.", SVR, "Entity");
		crash();
	}


	*intEntity = (entity) {
		.object = intDict->value,
		.entity_handler = entity_handler == NULL ? *stub : entity_handler,
		.collide = collide
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
	if (deleteThisFrame == 0) {
		return;
	}
	for (int i = 0; i < deleteThisFrame; i++) {
		deleteEntityInt(deleteArray[i]);
	}
	free(deleteArray);
	deleteArray = malloc(sizeof(entity*));
	if (deleteArray == NULL) {
		logtofile("could not allocate memory for deleting entities, crashing!", SVR, "Entity");
		crash();
	}
	deleteThisFrame = 0;
}

void deleteEntity(entity* entity) {
	deleteArray = realloc(deleteArray, sizeof(struct entity*) * (deleteThisFrame + 1));
	if (deleteArray == NULL) {
		logtofile("could not allocate memory for deleting entities, crashing!", SVR, "Entity");
		crash();
	}
	deleteArray[deleteThisFrame] = entity; 
	deleteThisFrame++;
}	

void deleteEntityInt(entity* entity) {
	char buffer[18];
	itoa(entity->object->id, buffer);
	dictionary intDict = findKey(entities, buffer);
	if (intDict == NULL) {
		logtofile("could not find an entity, assuming that it was destroyed unsafely; assuming unsafe env.", SVR, "Entity");
		crash();
	}
	
	removeObject(entity->object->id);
	removeKey(entities, buffer);
	free(entity);
	entityCount--;
}