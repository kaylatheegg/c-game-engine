#include "../../includes/engine.h"

void initEntities() {
	entities = createDictionary();
	entityUID = 0;
	entityCount = 0;
	deleteArray = gmalloc(sizeof(entity*));
	deletedCount = 0;
}

void cleanEntities() {
	freeDictionary(entities);
}