#include "../../includes/engine.h"

void initObjects() {
	objects = createDictionary();
	objectCount = 0;
}

void cleanObjects() {
	//dictionary current = textures;

	/*while (current->next != NULL) {
		
	}*/

	freeDictionary(objects);
}

