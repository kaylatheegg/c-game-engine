#include "engine.h"

void initObjects() {
	objects = createDictionary();
	objectCount = 0;
	objectUID = 0;
	vertices = gmalloc(sizeof(*vertices) * 16 * objectCount + 16);
	elements = gmalloc(sizeof(*elements) * 6 * objectCount + 16);
}

void cleanObjects() {
	//dictionary current = textures;

	/*while (current->next != NULL) {
		
	}*/

	freeDictionary(objects);
}

