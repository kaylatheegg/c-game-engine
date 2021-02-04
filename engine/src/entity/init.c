#include "../../includes/engine.h"

void initEntities() {
	entities = createDictionary();
	entityCount = 0;
}

void cleanEntities() {
	freeDictionary(entities);
}