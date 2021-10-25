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

		addToDictionary(textures, "DEFAULT", surface);
		textureCount++;
	}

	SDL_Surface* surface = IMG_Load(textureDir);
	if (surface == NULL) {
		char error[512];
		sprintf(error, "Image \"%.128s\" could not be loaded, error: %.256s", textureDir, IMG_GetError());
		logtofile(error, ERR, "Texture");
		return 0;
	}

	addToDictionary(textures, textureName, surface);

	textureCount++;
	return 1;
}	

void cleanTexture() {
	dictionary current = textures;

	while (current != NULL) {
		if (current->value != NULL) {
			SDL_FreeSurface(current->value);
		}

		current = current->next;
	}
	
	freeDictionary(textures);
}

SDL_Surface* getTexture(const char* key) {
	SDL_Surface* texture;

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