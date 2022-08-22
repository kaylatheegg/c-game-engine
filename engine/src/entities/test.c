#include "engine.h"

void objHandler(entity** this) {
	UNUSED(this);
	return;
}

void objCollisionHandler(entity** this, entity** collision, float distance) {
	UNUSED(this);
	UNUSED(collision);
	UNUSED(distance);
}

void worldInit1() {
	//gonna be doing some physics testing stuff so we need this
	loadTexture("engine/data/images/balle.png", "Ball");
	for (int i = 0; i < 1; i++) {
		vec pos = VECCNT(randRange(800), randRange(800));
		createEntity((object){.name = "ball",
    	   					 	   .rect = (Rect){pos.x, pos.y, 64, 64}, 
    	   						   .xOffset = 0,
    	   						   .yOffset = 0,
    	   						   .scale = 1.0,
    	   						   .angle = 0,
    	   						   .texture = getTexture("Ball"),
    	   						   .layer = 1}, COLLIDE_CIRCLE,
		objHandler, NULL, 0,
		objCollisionHandler, &(body){.mass = 10, 
									 .velocity = VECCNT(-5 + randRange(10),-5 + randRange(10)), 
									 .acceleration = VECCNT(0,-9.81)});
	}
	for (int i = 0; i < 15; i++) {
		vec pos = VECCNT(randRange(800), randRange(800));
		createEntity((object){.name = "box",
    	   					 	   .rect = (Rect){pos.x, pos.y, 64, 64}, 
    	   						   .xOffset = 0,
    	   						   .yOffset = 0,
    	   						   .scale = 1.0,
    	   						   .angle = 0,
    	   						   .texture = getTexture("DEFAULT"),
    	   						   .layer = 1}, COLLIDE_CIRCLE,
		objHandler, NULL, 0,
		objCollisionHandler, &(body){.mass = 10, 
									 .velocity = VECCNT(0,0), 
									 .acceleration = VECCNT(0,0)});
	}
}