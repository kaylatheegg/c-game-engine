#include "engine.h"

// positive y = up
// positive x = right

void initPhysics() {
	physicsData = (universe){
		.maxForce = 2000,
		.maxVelocity = 20,
		.maxAcceleration = 2000
	};
	space = cpSpaceNew();
	cpSpaceSetDamping(space, 0.3);
}

void processPhysics() {
	cpFloat timestep = 1./60.;
	cpSpaceStep(space, timestep);
	//printf("new frame\n");
	for (size_t i = 0; i < entities->key->arraySize; i++) {
		entity** intEntity = *(entity***)getElement(entities->value, i);
		if ((*intEntity)->body == NULL) {
			continue;
		}

		cpVect pos = cpBodyGetPosition((*intEntity)->body->body);
		//printf("%f, %f : %s, %s\n", pos.x, pos.y, (*intEntity)->object->name, cpBodyIsSleeping((*intEntity)->body->body) == true ? "zzzz" : "awake");

		(*intEntity)->object->rect.x = pos.x;
		(*intEntity)->object->rect.y = pos.y;
		
		updateObject((*intEntity)->object);
	}
}

void addVelocity(entity** a, vec velocity) {
	cpVect vel = cpBodyGetVelocity((*a)->body->body);
	vec newVel = vecAdd(VECCNT(vel.x, vel.y), velocity);
	printf("%f, %f\n", newVel.x, newVel.y);
	cpBodySetVelocity((*a)->body->body, cpv(newVel.x, newVel.y));
}

void setVelocity(entity** a, vec velocity) {
	cpBodySetVelocity((*a)->body->body, cpv(velocity.x, velocity.y));
}


float circleCircleCollision(entity** a, entity** intEntity) {
	Rect entityRect = (*a)->object->rect;
	Rect intRect = (*intEntity)->object->rect;

	float radius1 = (entityRect.w+entityRect.h)/4;
	vec circleCenter1 = VECCNT(entityRect.x+(entityRect.w/2), entityRect.y+(entityRect.h/2));

	
	float radius2 = (intRect.w+intRect.h)/4;
	vec circleCenter2 = VECCNT(intRect.x+(intRect.w/2), intRect.y+(intRect.h/2));

	vec distance = vecSub(circleCenter1, circleCenter2);
	//printf("d: %f\n", (-vecLength(distance) + (radius1 + radius2)));
	if ((vecLength(distance) - (radius1 + radius2)) <= -COLLISION_EPSILON) {
		return (vecLength(distance));
	}
	return 0.;
}

float circleBoxCollision(entity** a, entity** intEntity) {
	//search box on this, finding a circle collision
	//radius is the iterated entities' width, divided by 2
	//center is at the center of the object
	//god i fucking hate collision detection
	Rect entityRect = (*a)->object->rect;
		//impl from http://www.jeffreythompson.org/collision-detection/circle-rect.php
	Rect intRect = (*intEntity)->object->rect;
	float circleX = intRect.x + intRect.w/2;
	float circleY = intRect.y + intRect.h/2;
	float testX = circleX;
	float testY = circleY;
	float radius = (intRect.w+intRect.h)/4;

	if (circleX < entityRect.x) { 				      testX = entityRect.x;}        // left edge
	else if (circleX > entityRect.x + entityRect.w) { testX = entityRect.x + entityRect.w;}     // right edge

	if (circleY < entityRect.y) {        		      testY = entityRect.y;}       // top edge
	else if (circleY > entityRect.y + entityRect.h) { testY = entityRect.y + entityRect.h;}     // bottom edge

	vec distance = VECCNT(circleX - testX, circleY - testY);
	if ((vecLength(distance) - radius) <= -COLLISION_EPSILON) {
		return vecLength(distance);
	}

	return 0;
}

float AABBCollision(entity** a, entity** intEntity) {
	Rect rect1 = (*a)->object->rect;
	Rect rect2 = (*intEntity)->object->rect;
	if (rect1.x - rect2.x - rect2.w < -COLLISION_EPSILON &&
   		rect1.x + rect1.w - rect2.x > -COLLISION_EPSILON &&
   		rect1.y - rect2.y - rect2.w < -COLLISION_EPSILON &&
   		rect1.y + rect1.h - rect2.y > -COLLISION_EPSILON) {
			//collision!
		return 1;
   	}
		
	return 0;
}