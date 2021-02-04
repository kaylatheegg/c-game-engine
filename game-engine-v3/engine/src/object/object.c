#include "../../includes/engine.h"

int createObject(char* objName, int x, int y, int width, int height, int xOffset, int yOffset, float scale, int angle, SDL_Texture* texture) {
	object* intObject;
	intObject = malloc(sizeof(object));
	if (intObject == NULL) {
		logtofile("could not allocate memory for object, assuming OoM crashing!", SVR, "Object");
		crash();
	}

	if (objName == NULL) {
		intObject->name = "NameProvidedWasNULL";
	} else {
		intObject->name = malloc(sizeof(char) * strlen(objName) + 1);
		strcpy(intObject->name, objName);
	}

	SDL_Rect* rect;
	rect = malloc(sizeof(SDL_Rect));
	if (rect == NULL) {
		logtofile("could not allocate memory for object rect, assuming OoM crashing!", SVR, "Object");
		crash();
	}

	rect->x = x;
	rect->y = y;
	rect->w = width;
	rect->h = height;
	intObject->rect = rect;

	

	intObject->xOffset = xOffset;
	intObject->yOffset = yOffset;
	intObject->scale = scale;
	intObject->angle = angle;
	intObject->id = objectCount;

	if (texture == NULL) {
		intObject->texture = getTexture("DEFAULT");
	} else {
		intObject->texture = texture;
	}

	char buffer[128];
	itoa(intObject->id, buffer);

	addToDictionary(objects, buffer, intObject);
	return objectCount++;
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
	free(intObject->rect);
	free(intObject->name);
	free(intObject);
	removeKey(objects, buffer);
}