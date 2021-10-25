#include "engine.h"

//credit to hurubon in the c/c++ server for helping me realise what a shitty system this was

int createObject(const char* objName, SDL_Rect rect, int xOffset, int yOffset, float scale, int angle, int_Texture* tx) {
	object* intObject;
	intObject = gmalloc(sizeof(object));

	*intObject = (object) {
        .rect    = rect,
        .xOffset  = xOffset,
        .yOffset = yOffset,
        .scale   = scale,
        .angle   = angle,
        .id      = objectUID,
        .texture = tx == NULL ? getTexture("DEFAULT") : tx,
        .name    = objName == NULL ? strdup("NameProvidedWasNULL") : strdup(objName),
    };

	char buffer[128];
	itoa(intObject->id, buffer);

	addToDictionary(objects, buffer, intObject);
	objectCount++;
	return objectUID++;
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
	gfree(intObject->name);
	gfree(objectDict->value);
	objectDict->value = NULL;
	removeKey(objects, buffer);
	objectCount--;
}