#include "engine.h"

object* createObject(const char* objName, Rect rect, int xOffset, int yOffset, float scale, double angle, int_Texture* tx) {
	vertices = grealloc(vertices, sizeof(*vertices) * 16 * (objectUID + 1));
	elements = grealloc(elements, sizeof(*elements) * 6 * (objectUID + 1));


	object* intObject;
	intObject = gmalloc(sizeof(object));

	int objectVertexID = objectUID;

	if (vertexPoolSize >= 1) {
		printf("using vertex pool!\n");
		objectVertexID = vertexPool[vertexPoolSize - 1];
		vertexPoolSize--;
	} else {
		vertices = grealloc(vertices, sizeof(*vertices) * 16 * (objectUID + 1));
		elements = grealloc(\elements, sizeof(*elements) * 6 * (objectUID + 1));
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
        .vertexID = objectVertexID
    };

    updateObject(intObject);

    char buffer[18];
    itoa(intObject->id, buffer);
	addToDictionary(objects, buffer, intObject);
	objectCount++;
	objectUID++;
	return intObject;
}

/*object* getObjectID(int ID) {
	char buffer[18];
	itoa(ID, buffer);

	dictionary current = objects->next;
	while (current != NULL && current->key != NULL) {
		//printf("key: %s\n", current->key);
		if (strcmp(current->key, buffer) == 0) {
			return (object*)current->value;
		}
		current = current->next;
	}

	return NULL;
}*/

object* getObject(const char* key) {
	dictionary intDict = findKey(objects, key);
	return intDict == NULL ? NULL : (object*)intDict->value;
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
	Rect intRect = intObject->rect;
	intRect.x += intObject->xOffset;
	intRect.y += intObject->yOffset;


	vertices[count * 16 + 2] = (textureX + 0.5) / atlasW; // top left texcoord
	vertices[count * 16 + 3] = (textureY - 0.5 + textureH) /atlasH;

	vertices[count * 16 + 6] = (textureX - 0.5 + textureW) / atlasW; // top right texcoord
	vertices[count * 16 + 7] = (textureY - 0.5 + textureH) / atlasH;

	vertices[count * 16 + 10] = (textureX - 0.5 + textureW) / atlasW; // bottom right texcoord
	vertices[count * 16 + 11] = (textureY + 0.5) / atlasH; 

	vertices[count * 16 + 14] = (textureX + 0.5) / atlasW; // bottom left texcoord
	vertices[count * 16 + 15] = (textureY + 0.5) / atlasH;

	if (intObject->angle != 0) {
		double angle = intObject->angle;
		//find point of rotation
		vec rotationOrigin = VECCNT(intObject->rect.x + intObject->rect.w/2, intObject->rect.y + intObject->rect.h/2);
		vec rotation1 = vecRotateAroundOrigin(VECCNT(intRect.x, intRect.y), rotationOrigin, angle);
		vec rotation2 = vecRotateAroundOrigin(VECCNT(intRect.x + intRect.w, intRect.y), rotationOrigin, angle);
		vec rotation3 = vecRotateAroundOrigin(VECCNT(intRect.x + intRect.w, intRect.y + intRect.h), rotationOrigin, angle);
		vec rotation4 = vecRotateAroundOrigin(VECCNT(intRect.x, intRect.y + intRect.h), rotationOrigin, angle);

		vertices[count * 16 + 0] = rotation1.x * 2.0 / SCREEN_WIDTH - 1.0;	
		vertices[count * 16 + 1] = rotation1.y * 2.0 / SCREEN_HEIGHT - 1.0;

		vertices[count * 16 + 4] = rotation2.x * 2.0 / SCREEN_WIDTH - 1.0;	
		vertices[count * 16 + 5] = rotation2.y * 2.0 / SCREEN_HEIGHT - 1.0;

		vertices[count * 16 + 8] = rotation3.x * 2.0 / SCREEN_WIDTH - 1.0;	
		vertices[count * 16 + 9] = rotation3.y * 2.0 / SCREEN_HEIGHT - 1.0;

		vertices[count * 16 + 12] = rotation4.x * 2.0 / SCREEN_WIDTH - 1.0;	
		vertices[count * 16 + 13] = rotation4.y * 2.0 / SCREEN_HEIGHT - 1.0;
		//(object - object center) rotated by angle
		//angle rotated + object center = rotation
		return;
	}
	vertices[count * 16 + 0] = intRect.x * 2.0 / SCREEN_WIDTH - 1.0;  // top left
	vertices[count * 16 + 1] = intRect.y * 2.0 / SCREEN_HEIGHT - 1.0;

	vertices[count * 16 + 4] = (intRect.x + intRect.w) * 2.0 / SCREEN_WIDTH - 1.0; //top right
	vertices[count * 16 + 5] = intRect.y * 2.0 / SCREEN_HEIGHT - 1.0;

	vertices[count * 16 + 8] = (intRect.x + intRect.w) * 2.0 / SCREEN_WIDTH - 1.0; //bottom right
	vertices[count * 16 + 9] = (intRect.y + intRect.h) * 2.0 / SCREEN_HEIGHT - 1.0;

	vertices[count * 16 + 12] = intRect.x * 2.0 / SCREEN_WIDTH - 1.0; //bottom left
	vertices[count * 16 + 13] = (intRect.y + intRect.h) * 2.0 / SCREEN_HEIGHT - 1.0;
}

void removeObject(const char* key) {
	dictionary objectDict = findKey(objects, key);
	if (objectDict == NULL) {
		char error[256];
		sprintf(error, "object \"%s\" could not be found, returning early", key);
		logtofile(error, WRN, "Object");
		return;
	}

	object* intObject = (object*)objectDict->value;
	if (intObject == NULL) {
		logtofile("Unable to delete object, crashing!", ERR, "Entities");
		crash();
	}

	for (int i = 0; i < 16; i++) {
		vertices[intObject->vertexID * 16 + i] = 0.0f;
	}

	vertexPool = realloc(vertexPool, sizeof(int) * (vertexPoolSize + 1));
	vertexPool[vertexPoolSize] = intObject->vertexID;
	vertexPoolSize++;

	
	gfree(objectDict->value);
	objectDict->value = NULL;
	removeKey(objects, key);
	//gfree(intObject->name);
	objectCount--;
}