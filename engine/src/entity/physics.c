#include "engine.h"

// positive y = up
// positive x = right

void processPhysics() {
	float physicsTimeStep = 0.01;
	
	for (size_t i = 0; i < entities->key->arraySize; i++) {
		entity** intEntity = (*(entity***)getElement(entities->value, i));
		//v = u + at

		if (vecLength((*intEntity)->body->acceleration) > physicsData.maxAcceleration) {
			(*intEntity)->body->acceleration = vecScale(vecNorm((*intEntity)->body->acceleration), physicsData.maxAcceleration);
		}

		vec velocity = vecAdd(vecScale((*intEntity)->body->acceleration, physicsTimeStep), (*intEntity)->body->velocity);
		if (vecLength(velocity) > physicsData.maxVelocity) {
			velocity = vecScale(vecNorm(velocity), physicsData.maxVelocity);
		}

		//printf("%f\n", velocityMagnitude);
		(*intEntity)->body->velocity = velocity;
		(*intEntity)->object->rect.x += velocity.x;
		(*intEntity)->object->rect.y += velocity.y;
		if (vecLength(velocity) > 0) {
			//printf("%s\n", (*intEntity)->object->name);
			updateObject((*intEntity)->object);
		}

	}
	testCollision();
	for (size_t i = 0; i < collideArray->arraySize; i++) {

		collidePair* intPair = getElement(collideArray, i);
		entity* entityA = intPair->a;
		
		entity* entityB = intPair->b;

		vec entityAPos = VECCNT(entityA->object->rect.x, entityA->object->rect.y);
		vec entityBPos = VECCNT(entityB->object->rect.x, entityB->object->rect.y);

		vec distance = vecSub(entityAPos, entityBPos);
		UNUSED(distance);
		//addForce(&entityA, vecScale(distance, 1));
		//addForce(&entityB, vecScale(distance, -1));
		//entityA->body->velocity = vecAdd(vecScale(distance, 0.5), entityA->body->velocity);
		//entityB->body->velocity = vecAdd(vecScale(distance, -0.5), entityB->body->velocity);
	}
}

void addForce(entity** a, vec force) {
	(*a)->body->acceleration = vecAdd(vecScale(force, 1/(*a)->body->mass), (*a)->body->acceleration);
}

void initPhysics() {
	physicsData = (universe){
		.maxForce = 2000,
		.maxVelocity = 2000,
		.maxAcceleration = 2000
	};
}

