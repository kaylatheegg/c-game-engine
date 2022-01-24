#include "engine.h"

/**
 * @brief      Initializes the objects.
 */
void initObjects() {
	objects = createDictionary();
	objectCount = 0;
	objectUID = 0;
	vertices = gmalloc(sizeof(*vertices) * 16 * objectCount + 16);
	elements = gmalloc(sizeof(*elements) * 6 * objectCount + 16);
	vertexPool = malloc(sizeof(int));
	vertexPoolSize = 0;
	renderObjectSize = 0;
}

/**
 * @brief      Destroys all objects in the engine
 */
void cleanObjects() {
	//dictionary current = textures;

	/*while (current->next != NULL) {
		
	}*/

	free(vertexPool);

	freeDictionary(objects);
}

