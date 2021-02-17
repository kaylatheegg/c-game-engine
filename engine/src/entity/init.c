#include "../../includes/engine.h"

void initEntities() {
	entities = createDictionary();
	entityUID = 0;
	entityCount = 0;
	deleteArray = malloc(sizeof(entity*));
	if (deleteArray == NULL) {
		logtofile("could not allocate memory for deleting entities, crashing!", SVR, "Entity");
		crash();
	}
	deleteThisFrame = 0;
}

void cleanEntities() {
	freeDictionary(entities);
}