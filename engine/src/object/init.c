#include "engine.h"

/**
 * @brief      Initializes the objects.
 */
void initObjects() {
	objects = createDictionary();
	objectCount = 0;
	objectUID = 0;

	vertices = gmalloc(sizeof(*vertices) * MAX_RENDER_LAYERS);
	elements = gmalloc(sizeof(*elements) * MAX_RENDER_LAYERS);
	for (int i = 0; i < MAX_RENDER_LAYERS; i++) {
		vertices[i] = gmalloc(sizeof(**vertices) * 16 * objectCount + 16);
		elements[i] = gmalloc(sizeof(**elements) * 6 * objectCount + 16);
		vertexPools[i] = gmalloc(sizeof(int));
		vertexPoolSize[i] = 0;
		renderObjectSize[i] = 0;
	}
}

/**
 * @brief      Destroys all objects in the engine
 */
void cleanObjects() {
	//dictionary current = textures;

	/*while (current->next != NULL) {
		
	}*/

	//free(vertexPool);

	freeDictionary(objects);
}

