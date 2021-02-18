#include "../../includes/engine.h"

//credit to hurubon in the c/c++ server for helping me realise what a shitty system this was

int createObject(char* objName, SDL_Rect rect, int xOffset, int yOffset, float scale, int angle, SDL_Texture* tx) {
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
        .name    = objName == NULL ? "NameProvidedWasNULL" : objName,
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
	//object* intObject = objectDict->value;
	//this causes a memory leak, but until i figure out how to stop myself freeing shit from .rodata, ill take the tiny hit
	//free(intObject->name);
	gfree(objectDict->value);
	objectDict->value = NULL;
	removeKey(objects, buffer);
	objectCount--;
}