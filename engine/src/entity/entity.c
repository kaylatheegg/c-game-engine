#include "engine.h"

void stub(){}

void processDeletes();
void deleteEntityInt(entity** entity);

int createEntity(const char* objName, Rect rect, int xOffset, int yOffset, float scale, double angle, int_Texture* texture, int collide, void (*entity_handler)(entity**), void* data, int dataSize) {
	object* intObject = createObject(objName, rect, xOffset, yOffset, scale, angle, texture);
	
	entity* intEntity;
	intEntity = gmalloc(sizeof(entity));
	
	if (intObject == NULL) {
		logtofile("Object creation error, entity creation cannot continue!", ERR, "Entity");
		crash();
		return 0;
	}

	if (data == NULL) {
		dataSize = 0;
	}


	*intEntity = (entity) {
		.object = intObject,
		.entity_handler = entity_handler == NULL ? *stub : entity_handler,
		.collide = collide,
		.deleted = 0,
		.id = entityUID,
		.data = gmalloc(dataSize)
	};
	memcpy(intEntity->data, data, dataSize);

	addToDictionary(entities, objName, intEntity);
	entityCount++;
	
	return entityUID++;;
}

entity** getEntity(const char* key) {
	dictionary entityDictionary = findKey(entities, key);
	return entityDictionary == NULL ? NULL : (entity**)entityDictionary->value; 
}

void runEntities() {
	dictionary entityIterator = entities->next;
	for (int i = 0; i < entityCount; i++) {
		if (entityIterator == NULL) {
		    return;
		}
		entity* internalEntity = entityIterator->value;
		if (internalEntity == NULL) {
		    continue;
		}
		if (internalEntity->deleted == 1) {
			logtofile("deleted entity still exists!", WRN, "Entity");
			deleteEntity(&internalEntity);
			continue;
		}

		internalEntity->entity_handler((entity**)&entityIterator->value);
		entityIterator = entityIterator->next;
	}
	deleteEntities();
}

void deleteEntity(entity** intEntity) {
	(*intEntity)->deleted = 1;
	deletedCount++;
}

void deleteEntities() {
	dictionary entityIterator = entities->next;

	for (int i = 0; i < entityCount; i++) {
		if (entityIterator == NULL) {
		    return;
		}
		entity* internalEntity = entityIterator->value;
		if (internalEntity == NULL) {
		    continue;
		}
		entityIterator = entityIterator->next;
		if (internalEntity->deleted == 1) {
			entityCount--;
			removeObject(internalEntity->object->name);
			gfree(internalEntity->data);
			char buffer[18];
			itoa(internalEntity->id, buffer);

			dictionary entityDictionary = findKey(entities, buffer);
			gfree(entityDictionary->value);
			entityDictionary->value = NULL;
	
			removeKey(entities, buffer);
		}


	}

	deletedCount = 0;
}	

entity** AABBCollision(entity** a) {
	if ((*a) == NULL) {
		return NULL;
	}
	Rect rect1 = (*a)->object->rect;
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
		Rect rect2 = intEntity.object->rect;
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
	Rect rect1 = (*a)->object->rect;
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
		Rect rect2 = intObject.rect;
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
