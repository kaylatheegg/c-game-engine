#include "engine.h"

void initButtonWatchdog() {
	createEntity((object){.name = "Button Watchdog",
						  .rect = (Rect){0,0,0,0}, 
						  .xOffset = 0,
						  .yOffset = 0,
						  .scale = .0,
						  .angle = 0,
						  .texture = getTexture("DEFAULT"),
						  .layer = 0}, 0
						  ,buttonWatchdog, &BWdata, sizeof(BWdata), 
						  NULL, NULL);
	BWdata.buttons = createDynArray(sizeof(int));
}

void hello(entity** this, void* a) {
	int numData = *(int*)a;
	printf("hello from %d, %s!\n", numData, (*this)->object->name);
	printf("position x: %f, y: %f\n", (*this)->object->rect.x, (*this)->object->rect.y);
	updateObject((*this)->object);
}


/*	object* intObject = createObject(obj.name, obj.rect, obj.xOffset, obj.yOffset, obj.scale, obj.angle, obj.texture, obj.layer);

 every time a new texture is added, it shifts everything in the atlas over, causing a cascading shitfuck when the texture
 is not being correctly pointed to
 this is semi-easy to fix, its probably best just to rewrite the atlas though
 */

void initUI() {
	initText();
	initButtonWatchdog();
}


/*typedef struct {
	int_Texture* tx;
	RGBA colour;
	char* text;
	void (*trigger_func)(entity**, void*);
	void* argument;
	void (*handler_func)(entity**);
} buttonData;*/