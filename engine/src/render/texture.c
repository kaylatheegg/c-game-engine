#include "engine.h"

void printTextures() {
	printDictionary(textures);
}

int loadTexture(const char *textureDir, const char* textureName) {
	if (textures == NULL) {
		textures = createDictionary();

		SDL_Surface* surface = IMG_Load("engine/data/images/default.png");
		if (surface == NULL) {
			char error[512];
			sprintf(error, "Default image \"%.128s\" could not be loaded, error: %.256s", textureDir, IMG_GetError());
			logtofile(error, SVR, "Texture");
			crash();
		}
		int_Texture* intTx = malloc(sizeof(*intTx));
		*intTx = (int_Texture){surface, textureCount};
		addToDictionary(textures, "DEFAULT", intTx);
		textureCount++;
		return 0;
	}

	SDL_Surface* surface = IMG_Load(textureDir);
	if (surface == NULL) {
		char error[512];
		sprintf(error, "Image \"%.128s\" could not be loaded, error: %.256s", textureDir, IMG_GetError());
		logtofile(error, ERR, "Texture");
		return 0;
	}

	int_Texture* intTx = malloc(sizeof(*intTx));
	*intTx = (int_Texture){surface, textureCount};
	addToDictionary(textures, textureName, intTx);

	textureCount++;
	return 1;
}	

void cleanTexture() {
	dictionary current = textures;

	while (current != NULL) {
		int_Texture* intTx = (int_Texture*)current->value;
		if (current->value != NULL) {
			SDL_FreeSurface(intTx->surface);
			free(current->value);
		}

		current = current->next;
	}
	
	freeDictionary(textures);
}

int_Texture* getTexture(const char* key) {
	int_Texture* texture;

	dictionary texture_dict = findKey(textures, key);
	if (texture_dict == NULL) {
		//logtofile("Warning: findKey in getTexture has returned NULL. Using default texture!", WRN, "Texture");
		texture_dict = findKey(textures, "DEFAULT");
		if (texture_dict == NULL) {
			logtofile("Default texture was not found, crashing!", SVR, "Texture");
			crash();
		}
		texture = texture_dict->value;
	}
	texture = texture_dict->value;

	return texture;
}