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
		//velocity = vecAdd(velocity, vecScale(velocity, -0.001));
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

		entityA->object->rect.x += entityA->body->velocity.x;
		entityA->object->rect.y += entityA->body->velocity.y;
		entityB->object->rect.x += entityB->body->velocity.x;
		entityB->object->rect.y += entityB->body->velocity.y;
		//setVelocity(&entityA, vecScale(entityA->body->velocity, -1));
		//setVelocity(&entityB, vecScale(entityB->body->velocity, -1));
		//addForce(&entityA, vecScale(distance, 1));
		//addForce(&entityB, vecScale(distance, -1));
		//entityA->body->velocity = vecAdd(vecScale(distance, 0.5), entityA->body->velocity);
		//entityB->body->velocity = vecAdd(vecScale(distance, -0.5), entityB->body->velocity);
	}
}

void addVelocity(entity** a, vec velocity) {
	(*a)->body->velocity = vecAdd(velocity, (*a)->body->velocity);
}

void setVelocity(entity** a, vec velocity) {
	(*a)->body->velocity = velocity;
}
void initPhysics() {
	physicsData = (universe){
		.maxForce = 2000,
		.maxVelocity = 2000,
		.maxAcceleration = 2000
	};
}

