#include "engine.h"

/**
 * @brief      Prints the texture dictionary.
 */
void printTextures() {
	for (size_t i = 0; i < textures->key->arraySize; i++) {
		printf("%ld: %s\n", i, *(char**)getElement(textures->key, i));
	}	
}

/**
 * @brief      Loads a texture.
 *
 * @param[in]  textureDir   The texture directory
 * @param[in]  textureName  The texture name
 *
 * @return     Texture loading status.
 */
int loadTexture(const char *textureDir, const char* textureName) {
	if (window == NULL) {
		char buffer[1024];
		sprintf(buffer, "Attempted to load texture \"%s\" at \"%s\" before renderer init!", textureName == NULL ? "NULL" : textureName, textureDir == NULL ? "NULL" : textureDir);
		logtofile(buffer, ERR, "Render");
		return 1;
	}

	if (textureDir == NULL || textureName == NULL) {
		char buffer[1024];
		sprintf(buffer, "Attempted to load texture \"%s\" at \"%s\" but one of the arguments was NULL!", textureName == NULL ? "NULL" : textureName, textureDir == NULL ? "NULL" : textureDir);
		logtofile(buffer, ERR, "Render");
		return 1;
	}

	Uint32 rmask, gmask, bmask, amask;

	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    	rmask = 0xff000000;
    	gmask = 0x00ff0000;
    	bmask = 0x0000ff00;
    	amask = 0x000000ff;
	#else
    	rmask = 0x000000ff;
    	gmask = 0x0000ff00;
    	bmask = 0x00ff0000;
    	amask = 0xff000000;
	#endif

	if (textures == NULL) {
		textures = createDictionary();
		textureDir = "engine/data/images/default.png";
		SDL_Surface* surface = IMG_Load(textureDir);
		if (surface == NULL) {
			char error[512];
			sprintf(error, "Default image \"%.128s\" could not be loaded, error: %.256s", textureDir, IMG_GetError());
			logtofile(error, SVR, "Texture");
			crash();
		}
		
		int_Texture* intTx = malloc(sizeof(*intTx));
		*intTx = (int_Texture){surface, textureCount, 0, 0};
		addToDictionary(textures, "DEFAULT", intTx);
		textureCount++;
		textureAtlas = SDL_CreateRGBSurface(0, surface->w, surface->h, 32, rmask, gmask, bmask, amask);
		SDL_BlitSurface(surface, NULL, textureAtlas, &(SDL_Rect){0, 0, surface->w, surface->h});

		goto afterInit;
	}

	SDL_Surface* surface = IMG_Load(textureDir);
	if (surface == NULL) {
		char error[512];
		sprintf(error, "Image \"%.128s\" could not be loaded, error: %.256s", textureDir, IMG_GetError());
		logtofile(error, ERR, "Texture");
		return 1;
	}
	SDL_Surface* intTextureAtlas = SDL_CreateRGBSurface(0, textureAtlas->w + surface->w, surface->h > textureAtlas->h ? surface->h : textureAtlas->w, 
														32, rmask, gmask, bmask, amask);
	SDL_BlitSurface(textureAtlas, NULL, intTextureAtlas, &(SDL_Rect){0, 0, textureAtlas->w, textureAtlas->h});
	SDL_BlitSurface(surface, NULL, intTextureAtlas, &(SDL_Rect){textureAtlas->w, 0, surface->w, surface->h});

	int_Texture* intTx = malloc(sizeof(*intTx));
	*intTx = (int_Texture){surface, textureCount, textureAtlas->w, 0};
	addToDictionary(textures, textureName, intTx);

	SDL_FreeSurface(textureAtlas);
	textureAtlas = intTextureAtlas;

	afterInit:
	if (window != NULL) {
		glGenTextures(1, &txAtlasID);
		glBindTexture(GL_TEXTURE_2D, txAtlasID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureAtlas->w, textureAtlas->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureAtlas->pixels);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	}
	textureCount++;
	return 0;

}	

/**
 * @brief      Destroys all textures in the engine
 */
void cleanTexture() {
	if (textures == NULL) {
		return;
	}
	for (size_t i = 0; i < textures->key->arraySize; i++) {
		int_Texture* intTx = *(int_Texture**)getElement(textures->value, i);
		if (intTx != NULL) {
			SDL_FreeSurface(intTx->surface);
			free(intTx);
		}

	}
	SDL_FreeSurface(textureAtlas);
	freeDictionary(textures);
}

/**
 * @brief      Gets the texture.
 *
 * @param[in]  key   The name of the texture
 *
 * @return     The texture.
 */
int_Texture* getTexture(const char* key) {
	int_Texture* texture;

	if (textures == NULL) {
		loadTexture("engine/data/images/default.png", "DEFAULT");
	}

	size_t textureDictIndex = findKey(textures, key);
	if (textureDictIndex == NOVALUE) {
		//logtofile("Warning: findKey in getTexture has returned NULL. Using default texture!", WRN, "Texture");
		textureDictIndex = findKey(textures, "DEFAULT");
		if (textureDictIndex == NOVALUE) {
			logtofile("Default texture was not found, crashing!", SVR, "Texture");
			crash();
		}
		texture = *(int_Texture**)getElement(textures->value, textureDictIndex);
	}
	texture = *(int_Texture**)getElement(textures->value, textureDictIndex);

	return texture;
}

int_Texture* colourTexture(RGBA colour) {
	
	Uint32 rmask, gmask, bmask, amask;
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    	rmask = 0xff000000;
    	gmask = 0x00ff0000;
    	bmask = 0x0000ff00;
    	amask = 0x000000ff;
	#else
    	rmask = 0x000000ff;
    	gmask = 0x0000ff00;
    	bmask = 0x00ff0000;
    	amask = 0xff000000;
	#endif

    
    /*char testbuf[32];
	strcpy(testbuf, "Colour: 0x");
	sprintf((testbuf + 10), "%08x", colour.rgba);
	size_t keyIndex = findKey(textures, testbuf);
	if (keyIndex != NOVALUE) {
		return (int_Texture*)getElement(textures->value, keyIndex - 1) == NULL ? getTexture("DEFAULT"): (int_Texture*)getElement(textures->value, keyIndex - 1);
	}*/

    SDL_Surface* surface = SDL_CreateRGBSurface(0, 10, 10, 32, rmask, gmask, bmask, amask);
	if (surface == NULL) {
		char error[512];
		sprintf(error, "Could not generate monochromatic colour: R:%d G:%d B:%d A:%d", colour.r, colour.g, colour.b ,colour.a);
		logtofile(error, ERR, "Texture");
		return NULL;
	}
	SDL_FillRect(surface, &(SDL_Rect){0,0,10,10}, (uint32_t)colour.rgba);
	SDL_Surface* intTextureAtlas = SDL_CreateRGBSurface(0, textureAtlas->w + surface->w, surface->h > textureAtlas->h ? surface->h : textureAtlas->w, 
														32, rmask, gmask, bmask, amask);
	SDL_BlitSurface(textureAtlas, NULL, intTextureAtlas, &(SDL_Rect){0, 0, textureAtlas->w, textureAtlas->h});
	SDL_BlitSurface(surface, NULL, intTextureAtlas, &(SDL_Rect){textureAtlas->w, 0, surface->w, surface->h});

	int_Texture* intTx = malloc(sizeof(*intTx));
	*intTx = (int_Texture){surface, textureCount, textureAtlas->w, 0};
	//printf("w: %d, h: %d, id:%d\n", intTx->surface->w, intTx->surface->h, intTx->textureID);
	
	char buf[32];
	strcpy(buf, "Colour: 0x");
	sprintf((buf + 10), "%08x", colour.rgba);
	//creates a tex name "Colour: 0xrrggbbaa"
	addToDictionary(textures, buf, intTx);

	SDL_FreeSurface(textureAtlas);
	textureAtlas = intTextureAtlas;

	if (window != NULL) {
		glGenTextures(1, &txAtlasID);
		glBindTexture(GL_TEXTURE_2D, txAtlasID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureAtlas->w, textureAtlas->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureAtlas->pixels);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	}
	textureCount++;
	return intTx;
}