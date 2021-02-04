#include "../../includes/engine.h"

void printTextures() {
	printDictionary(textures);
}

int loadTexture(char *textureDir, char* textureName) {
	if (textures == NULL) {
		textures = createDictionary();

		SDL_Texture* texture = IMG_LoadTexture(renderer, "engine/data/images/default.png");
		if (texture == NULL) {
			char error[512];
			sprintf(error, "Default image \"%.128s\" could not be loaded, error: %.256s", textureDir, IMG_GetError());
			logtofile(error, SVR, "Texture");
			crash();
		}

		addToDictionary(textures, "DEFAULT", texture);
		textureCount++;
	}

	SDL_Texture* texture = IMG_LoadTexture(renderer, textureDir);
	if (texture == NULL) {
		char error[512];
		sprintf(error, "Image \"%.128s\" could not be loaded, error: %.256s", textureDir, IMG_GetError());
		logtofile(error, ERR, "Texture");
		return 0;
	}

	addToDictionary(textures, textureName, texture);

	textureCount++;
	return 1;
}	

void cleanTexture() {
	dictionary current = textures;

	while (current != NULL) {
		if (current->value != NULL) {
			SDL_DestroyTexture(current->value);
		}

		current = current->next;
	}
	
	freeDictionary(textures);
}

SDL_Texture* getTexture(char* key) {
	SDL_Texture* texture;

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