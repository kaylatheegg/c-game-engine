#include "engine.h"

void stub(){}

void processDeletes();
void deleteEntityInt(entity** entity);

int createEntity(const char* objName, SDL_Rect rect, int xOffset, int yOffset, float scale, int angle, SDL_Surface* texture, int collide, void (*entity_handler)(entity**), void* data, int dataSize) {
	int objectId = createObject(objName, rect, xOffset, yOffset, scale, angle, texture);
	
	entity* intEntity;
	intEntity = gmalloc(sizeof(entity));

	char buffer[18];
	itoa(objectId, buffer);
	dictionary intDict = findKey(objects, buffer);

	if (data == NULL) {
		dataSize = 0;
	}


	*intEntity = (entity) {
		.object = intDict->value,
		.entity_handler = entity_handler == NULL ? *stub : entity_handler,
		.collide = collide,
		.deleted = 0,
		.id = entityUID,
		.data = gmalloc(dataSize)
	};
	memcpy(intEntity->data, data, dataSize);

	itoa(entityUID, buffer);

	addToDictionary(entities, buffer, intEntity);
	entityCount++;
	
	return entityUID++;;
}

entity** getEntity(int UID) {
	char buffer[18];
	itoa(UID, buffer);

	dictionary entityDictionary = findKey(entities, buffer);
	return entityDictionary == NULL ? NULL : (entity**)entityDictionary->value; 
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
			logtofile("deleted entity still exists!", WRN, "Entity");
			continue;
		}

		internalEntity->entity_handler((entity**)&entityIterator->value);
	}
}

void deleteEntity(entity** intEntity) {
	(*intEntity)->deleted = 1;
	entityCount--;
	removeObject((*intEntity)->object->id);
	gfree((*intEntity)->data);
	char buffer[18];
	itoa((*intEntity)->id, buffer);

	dictionary entityDictionary = findKey(entities, buffer);
	gfree(entityDictionary->value);
	entityDictionary->value = NULL;
	
	removeKey(entities, buffer);

}

entity** AABBCollision(entity** a) {
	if ((*a) == NULL) {
		return NULL;
	}
	SDL_Rect rect1 = (*a)->object->rect;
	dictionary entityIterator = entities;
	for (int i = 0; i < entityUID; i++) {
		entityIterator = entityIterator->next;
		if (entityIterator == NULL) {
			break;
		}
		if (entityIterator->value == a || entityIterator->value == NULL) {
			continue;
		}

		entity intEntity = *(entity*)entityIterator->value;
		SDL_Rect rect2 = intEntity.object->rect;
		if (rect1.x < rect2.x + rect2.w &&
   		rect1.x + rect1.w > rect2.x &&
   		rect1.y < rect2.y + rect2.w &&
   		rect1.y + rect1.h > rect2.y) {
			//collision!
			return (entity**)&entityIterator->value;
   		}
	}	
	return NULL;
}

object* AABBCollisionObj(entity** a) {
	if ((*a) == NULL) {
		return NULL;
	}
	SDL_Rect rect1 = (*a)->object->rect;
	dictionary objectDict = objects;
	for (int i = 0; i < objectUID; i++) {
		objectDict = objectDict->next;
		if (objectDict == NULL) {
			break;
		}
		if (objectDict->value == (*a)->object || objectDict->value == NULL) {
			continue;
		}

		object intObject = *(object*)objectDict->value;
		SDL_Rect rect2 = intObject.rect;
		if (rect1.x < rect2.x + rect2.w &&
   		rect1.x + rect1.w > rect2.x &&
   		rect1.y < rect2.y + rect2.w &&
   		rect1.y + rect1.h > rect2.y) {
			//collision!
			return (object*)&objectDict->value;
   		}
	}	
	return NULL;
}
