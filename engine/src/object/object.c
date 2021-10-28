#include "engine.h"

//credit to hurubon in the c/c++ server for helping me realise what a shitty system this was

int createObject(const char* objName, SDL_Rect rect, int xOffset, int yOffset, float scale, int angle, int_Texture* tx) {
	vertices = grealloc(vertices, sizeof(*vertices) * 16 * (objectUID + 1));
	elements = grealloc(elements, sizeof(*elements) * 6 * (objectUID + 1));

	object* intObject;
	intObject = gmalloc(sizeof(object));

	int intPoolID = 0;

	if (vertexPoolSize != 0) {
		intPoolID = vertexPool[vertexPoolSize-1];
		vertexPoolSize--;
	}

	/*if (vertexPoolSize > FRAGMENTATION_LIMIT) {
		dictionary intObjDict = findTail(objects);
		for (int i = 0; i < FRAGMENTATION_LIMIT; i++) {
			object* intObject = (object*)intObjDict->value;
			int storedID = intObject->vertexID;
			intObject->vertexID = vertexPool[vertexPoolSize - 1 - i];
			vertexPool[vertexPoolSize-1-i] = storedID;
			dictionary intObjDict = findPrevKey(objects, intObjDict->key);			
		}
	}*/

	*intObject = (object) {
        .rect    = rect,
        .xOffset  = xOffset,
        .yOffset = yOffset,
        .scale   = scale,
        .angle   = angle,
        .id      = objectUID,
        .texture = tx == NULL ? getTexture("DEFAULT") : tx,
        .name    = objName == NULL ? strdup("NameProvidedWasNULL") : strdup(objName),
        .vertexID = vertexPoolSize != 0 ? intPoolID : objectCount
    };

	float textureX = intObject->texture->x;
	float textureY = intObject->texture->y;
	float textureW = intObject->texture->surface->w;
	float textureH = intObject->texture->surface->h;
	float atlasH = textureAtlas->h;
	float atlasW = textureAtlas->w;

	int count = intObject->vertexID;
	SDL_Rect intRect = rect;
	vertices[count * 16 + 0] = intRect.x * 2.0 / SCREEN_WIDTH - 1.0;  // top left
	vertices[count * 16 + 1] = intRect.y * 2.0 / SCREEN_HEIGHT - 1.0;
	vertices[count * 16 + 2] = textureX / atlasW; // top left texcoord
	vertices[count * 16 + 3] = (textureY + textureH) /atlasH;

	vertices[count * 16 + 4] = (intRect.x + intRect.w) * 2.0 / SCREEN_WIDTH - 1.0; //top right
	vertices[count * 16 + 5] = intRect.y * 2.0 / SCREEN_HEIGHT - 1.0;
	vertices[count * 16 + 6] = (textureX + textureW) / atlasW; // top right texcoord
	vertices[count * 16 + 7] = (textureY + textureH) / atlasH;

	vertices[count * 16 + 8] = (intRect.x + intRect.w) * 2.0 / SCREEN_WIDTH - 1.0; //bottom right
	vertices[count * 16 + 9] = (intRect.y + intRect.h) * 2.0 / SCREEN_HEIGHT - 1.0;
	vertices[count * 16 + 10] = (textureX + textureW) / atlasW; // bottom right texcoord
	vertices[count * 16 + 11] = textureY / atlasH; 

	vertices[count * 16 + 12] = intRect.x * 2.0 / SCREEN_WIDTH - 1.0; //bottom left
	vertices[count * 16 + 13] = (intRect.y + intRect.h) * 2.0 / SCREEN_HEIGHT - 1.0;
	vertices[count * 16 + 14] = textureX / atlasW; // bottom left texcoord
	vertices[count * 16 + 15] = textureY / atlasH;

	char buffer[128];
	itoa(intObject->id, buffer);

	addToDictionary(objects, buffer, intObject);
	objectCount++;
	return objectUID++;
}

void updateObject(object* intObject) {
	if (intObject == NULL) {
		return;
	}

	float textureX = intObject->texture->x;
	float textureY = intObject->texture->y;
	float textureW = intObject->texture->surface->w;
	float textureH = intObject->texture->surface->h;
	float atlasH = textureAtlas->h;
	float atlasW = textureAtlas->w;

	int count = intObject->vertexID;
	SDL_Rect intRect = intObject->rect;
	vertices[count * 16 + 0] = intRect.x * 2.0 / SCREEN_WIDTH - 1.0;  // top left
	vertices[count * 16 + 1] = intRect.y * 2.0 / SCREEN_HEIGHT - 1.0;
	vertices[count * 16 + 2] = textureX / atlasW; // top left texcoord
	vertices[count * 16 + 3] = (textureY + textureH) /atlasH;

	vertices[count * 16 + 4] = (intRect.x + intRect.w) * 2.0 / SCREEN_WIDTH - 1.0; //top right
	vertices[count * 16 + 5] = intRect.y * 2.0 / SCREEN_HEIGHT - 1.0;
	vertices[count * 16 + 6] = (textureX + textureW) / atlasW; // top right texcoord
	vertices[count * 16 + 7] = (textureY + textureH) / atlasH;

	vertices[count * 16 + 8] = (intRect.x + intRect.w) * 2.0 / SCREEN_WIDTH - 1.0; //bottom right
	vertices[count * 16 + 9] = (intRect.y + intRect.h) * 2.0 / SCREEN_HEIGHT - 1.0;
	vertices[count * 16 + 10] = (textureX + textureW) / atlasW; // bottom right texcoord
	vertices[count * 16 + 11] = textureY / atlasH; 

	vertices[count * 16 + 12] = intRect.x * 2.0 / SCREEN_WIDTH - 1.0; //bottom left
	vertices[count * 16 + 13] = (intRect.y + intRect.h) * 2.0 / SCREEN_HEIGHT - 1.0;
	vertices[count * 16 + 14] = textureX / atlasW; // bottom left texcoord
	vertices[count * 16 + 15] = textureY / atlasH;
}

void removeObject(int id) {
	char buffer[128];
	itoa(id, buffer);

	dictionary objectDict = findKey(objects, buffer);
	if (objectDict == NULL) {
		logtofile("object could not be found, returning early", WRN, "Object");
		return;
	}
	object* intObject = objectDict->value;
	for (int i = 0; i < 16; i++) {
		vertices[intObject->vertexID * 16 + i] = 0.0f;
	}

	vertexPool = realloc(vertexPool, sizeof(int) * (vertexPoolSize + 1));
	vertexPool[vertexPoolSize] = intObject->vertexID;
	vertexPoolSize++;

	gfree(intObject->name);
	gfree(objectDict->value);
	objectDict->value = NULL;
	removeKey(objects, buffer);
	objectCount--;
}