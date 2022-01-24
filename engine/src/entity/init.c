#include "engine.h"

/**
 * @brief      Initialises the entities
 */
void initEntities() {
	entities = createDictionary();
	entityUID = 0;
	entityCount = 0;
	deletedCount = 0;
}